/**************************************************************************
/*		FileName: XFilePackage.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/11/16 13:42:39
/*		CopyRight: yc 
/*************************************************************************/

#include "XFilePackage.h"
#include "zlib.h"

bool XFilePackageEasy::InitPackage(const char* fpk_file)
{
	CloseFile();
	if (!OpenFile(fpk_file, "rb+"))
	{
		return false;
	}
	
	XEasyFPKHeader header;
	if(sizeof(header) != Read((void*)&header, 1, sizeof(header)))
	{
		CloseFile();
		return false;
	}
	
	if (XFILE_PACKAGE_MAGIC1 != header.magic1
		||
		XFILE_PACKAGE_MAGIC2 != header.magic2
		)
	{
		CloseFile();
		return false;
	}
	
	return LoadPackage(header.version);
}

bool XFilePackageEasy::LoadPackage(int version)
{
	long length = Length();
	Seek(length - sizeof(int), SEEK_SET);
	int records_offset = 0;
	QuickReadValue(records_offset);
	if (records_offset == 0)//表示没有文件
	{
		return true;//
	}

	if(records_offset != Seek(records_offset, SEEK_SET))
	{
		return false;
	}

	int records = 0;
	QuickReadValue(records);
	for (int i = 0; i < records; i++)
	{
		XEasyPackageRecord epp;
		int path_length = 0;
		QuickReadValue(path_length);
		Assert(path_length > 0);
		epp.path.resize(path_length + 1);
		Read(&epp.path[0], 1, path_length);
		epp.path[path_length] = '\0';
		QuickReadValue(epp.offset);
		package_records.push_back(epp);
	}
	return true;
}

bool XFilePackageEasy::CreatePackage(const char* fpk_file)
{
	CloseFile();
	if (!OpenFile(fpk_file, "wb"))
	{
		return false;
	}
	
	if (!SavePackageHeader())
	{
		return false;
	}
	return true;
}

bool XFilePackageEasy::SavePackageHeader()
{
	XEasyFPKHeader header;
	header.magic1 = XFILE_PACKAGE_MAGIC1;
	header.magic2 = XFILE_PACKAGE_MAGIC2;
	header.version = 0;

	Write(&header, 1 , sizeof(header));
	
	cur_offset = Tell();
	return true;
}

bool XFilePackageEasy::SavePackageRecords()
{
	int records_offset = 0;
	Seek(cur_offset, SEEK_SET);
	if (package_records.size())
	{
		records_offset = cur_offset;
		for (int i = 0; i < package_records.size(); i++)
		{
			QuickWriteValue(package_records[i].path.length());
			Write(package_records[i].path.c_str(), 1, package_records[i].path.length());
			QuickWriteValue(package_records[i].offset);
		}
	}
	QuickWriteValue(records_offset);
	return true;
}

bool XFilePackageEasy::AppendFile(const char* path, const unsigned char* buffer, int length)
{
	XEasyPackageRecord* record = FindRecord(path);
	if (record)
	{
		return RewriteFile(path, buffer, length);
	}
	record = AddRecord(path);
	if (!record)
	{
		return false;
	}
	return AddBufferZlib(record, buffer, length);
}

bool XFilePackageEasy::ReplaceFile(const char* old_path, const char* cur_path, const unsigned char* buffer, int length)
{
	if (0 == strcmp(old_path, cur_path))
	{
		return RewriteFile(cur_path, buffer, length);
	}
	XEasyPackageRecord* record = FindRecord(old_path);
	if (!record)//如果老记录没有
	{
// 		record = FindRecord(cur_path);
// 		if (record)
// 		{
// 			
// 		}
// 		else
// 		{
// 			return AppendFile(cur_path, buffer, length);
// 		}
		return false;
	}
	else
	{
		XEasyPackageRecord* cur_record = FindRecord(cur_path);
		if (cur_record)
		{
			//如果由当前记录，那么直接重写
			return RewriteFile(cur_path, buffer, length);
		}
		else
		{
			//如果没有当前记录，那么添加一个
			record->path = cur_path;
			AppendFile(cur_path, buffer, length);
		}
	}
	return false;
}

bool XFilePackageEasy::RemoveFile(const char* path)
{
	XEasyPackageRecord* record = FindRecord(path);
	if (record)
	{
		package_records.erase(*(PackageRecords::iterator*)&record);
		Assert(NULL == FindRecord(path));
		return true;
	}
	return false;
}

bool XFilePackageEasy::AddBufferZlib(XEasyPackageRecord* record, const unsigned char* buffer, int length)
{
	if (!record)
	{
		return false;
	}
	uLongf dest_len = (length + 12 ) * 2;
	unsigned char* dest_buffer = new unsigned char[dest_len];
	if (!dest_buffer)
	{
		Assert(0);
		return false;
	}

	int ret = compress(dest_buffer, &dest_len, buffer, length);
	switch(ret)
	{
	case Z_OK:
		{
			if (dest_len >= length)//如果压缩后比当前大那么直接弄原数据
			{
				record->compress_type = NONE_COMPRESS;
				record->offset = cur_offset;
				SeekSet(cur_offset);
				if(length!= SafeWrite(buffer,1, length, XFILE_PACKAGE_SAFE_SIZE))
				{
					//未能完全写入
					return false;
				}
				cur_offset += length;
			}
			else//写入压缩的数据
			{
				record->compress_type = Z_LIB_COMPRESS;
				record->offset = cur_offset;
				SeekSet(cur_offset);
				if(length!= SafeWrite(dest_buffer,1, dest_len, XFILE_PACKAGE_SAFE_SIZE))
				{
					//未能完全写入
					return false;
				}
				cur_offset += dest_len;
			}
			delete dest_buffer;
			return true;
		}
		break;
	case Z_MEM_ERROR:
		//内存不够
		break;
	case Z_BUF_ERROR:
		//dest_buffer不够长
		{
			record->compress_type = NONE_COMPRESS;
			record->offset = cur_offset;
			SeekSet(cur_offset);
			if(length!= SafeWrite(buffer,1, length, XFILE_PACKAGE_SAFE_SIZE))
			{
				//未能完全写入
				return false;
			}
			cur_offset += length;
		}
		break;
	case Z_STREAM_ERROR:
		//
		break;
	default:
		return false;
		break;
	}
	return false;
}

XFilePackageEasy::XEasyPackageRecord* XFilePackageEasy::FindRecord(const char* path)
{
	struct FindRecordCmp 
	{
		FindRecordCmp(const char* path):str_cmp(path){}
		bool operator()(const XEasyPackageRecord& epr)
		{
			return str_cmp == epr.path;
		}
	private:
		std::string str_cmp;
	};
	PackageRecords::iterator iter = std::find_if(package_records.begin(), package_records.end(), FindRecordCmp(path));

	if (iter == package_records.end())
	{
		return NULL;
	}

	return *(XEasyPackageRecord**)&iter;
}