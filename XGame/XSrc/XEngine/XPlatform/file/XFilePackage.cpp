/**************************************************************************
/*		FileName: XFilePackage.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/11/16 13:42:39
/*		CopyRight: yc 
/*************************************************************************/

#include "XFilePackage.h"
#include "zlib.h"
#include "XMD5.h"
#include "XBufferStream.h"

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

	if(0 != Seek(records_offset, SEEK_SET))
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
		QuickReadValue(epp.compress_type);
		QuickReadValue(epp.buf_len);
		QuickReadValue(epp.org_len);
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
		QuickWriteValue(package_records.size());
		for (int i = 0; i < package_records.size(); i++)
		{
			QuickWriteValue(package_records[i].path.length());
			Write(package_records[i].path.c_str(), 1, package_records[i].path.length());
			QuickWriteValue(package_records[i].compress_type);
			QuickWriteValue(package_records[i].buf_len);
			QuickWriteValue(package_records[i].org_len);
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

bool XFilePackageEasy::RewriteFile(const char* path, const unsigned char* buffer, int length)
{
	XEasyPackageRecord* cur_record = FindRecord(path);
	if (cur_record)
	{
		return AddBufferZlib(cur_record, buffer, length);
	}
	return true;
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

bool XFilePackageEasy::AddFile(const char* full_path, const char* path)
{
	if (!full_path || !path)
	{
		return false;
	}

	if (FindRecord(path))//如果有那么直接剃掉
	{
		return false;
	}
	XEasyPackageRecord* record = AddRecord(path);//增加记录
	if (!record)
	{
		return false;
	}
	unsigned char digest[16] = {0};
	XFile file;
	MD5_CTX context;
	int len;
	unsigned char buffer[1024];
	XSimpleVectorStream<unsigned char> isvs;

	if ((file.OpenFile (full_path, "rb")) == NULL)
		return false;
	else 
	{
		int path_len = strlen(path);
		QuickWriteValue(path_len);//写入文件名长度
		if(path_len != Write(path, 1, path_len))//写入文件名
		{
			return false;
		}
		MD5Init (&context);
		long file_len = file.Length();
		isvs.ReserveLen((int)file_len);
		while (len = file.Read (buffer, 1, 1024))
		{
			MD5Update (&context, buffer, len);
			isvs.PushStream((unsigned char*)buffer, len);
		}
		MD5Final (digest, &context);
		file.CloseFile();
	}
	AddBufferZlib(record, (const unsigned char*)isvs.GetData(), isvs.Length());
	if (16 != Write(digest, 1, sizeof(digest)))//写入md5
	{
		return false;
	}
	cur_offset = Tell();//重置当前偏移
	return true;
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
	record->org_len = length;
	int ret = compress(dest_buffer, &dest_len, buffer, length);
	switch(ret)
	{
	case Z_OK:
		{
			if (dest_len >= length)//如果压缩后比当前大那么直接弄原数据
			{
				record->compress_type = NONE_COMPRESS;
				record->offset = cur_offset;
				record->buf_len = length;
				SeekSet(cur_offset);
				if(length!= SafeWrite(buffer,1, length, XFILE_PACKAGE_SAFE_SIZE))
				{
					delete dest_buffer;
					//未能完全写入
					return false;
				}
				cur_offset += length;
			}
			else//写入压缩的数据
			{
				record->compress_type = Z_LIB_COMPRESS;
				record->offset = cur_offset;
				record->buf_len = dest_len;
				SeekSet(cur_offset);
				if(dest_len!= SafeWrite(dest_buffer,1, dest_len, XFILE_PACKAGE_SAFE_SIZE))
				{
					Assert(0);
					delete dest_buffer;
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
			record->buf_len = length;
			SeekSet(cur_offset);
			if(length!= SafeWrite(buffer,1, length, XFILE_PACKAGE_SAFE_SIZE))
			{
				delete dest_buffer;
				Assert(0);
				//未能完全写入
				return false;
			}
			cur_offset += length;
			delete dest_buffer;
			return true;
		}
		break;
	case Z_STREAM_ERROR:
		//
		break;
	default:
		delete dest_buffer;
		return false;
		break;
	}

	delete dest_buffer;
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

XFilePackageEasy::XEasyPackageRecord* XFilePackageEasy::AddRecord(const char* path)
{
	XEasyPackageRecord* epr = FindRecord(path);
	if (epr)
	{
		return NULL;
	}
	
	XEasyPackageRecord record;
	record.path = path;
	package_records.push_back(record);
	return &(package_records[package_records.size() - 1]);
}

bool XFilePackageEasy::ReadFileContent(const char* path, void** buff, int* len)
{
	XEasyPackageRecord* record = FindRecord(path);
	if (!record)
	{
		return false;
	}
	return true;
}

bool XFilePackageEasy::ReadFileContent(const XEasyPackageRecord* record, void** buff, int* len)
{
	if (!record)
	{
		return false;
	}
	xulong file_len = Length();
	if (record->offset < 0 || record->offset > file_len || record->buf_len < 0 || record->offset + record->buf_len > file_len)
	{
		return false;
	}
	SeekSet(record->offset);
	
	*buff = (void*)(malloc(record->buf_len));
	if (!*buff)
	{
		return false;
	}
	if(record->buf_len != SafeRead(*buff,1, record->buf_len, XFILE_PACKAGE_SAFE_SIZE))
	{
		delete *buff;
		*buff = NULL;
		Assert(0);
		//未能完全读取
		return false;
	}
	switch(record->compress_type)
	{
	case NONE_COMPRESS:
		{
			if (len)
			{
				*len = record->buf_len;
			}
		}
		break;
	case Z_LIB_COMPRESS:
		{
			void* uncomp_buff = (void*)(malloc(record->org_len));
			if (!uncomp_buff)
			{
				Assert(0);
				delete *buff;
				*buff = NULL;
				return false;
			}
			uLongf uncomp_len = record->org_len;
			int ret = uncompress((unsigned char*)uncomp_buff, &uncomp_len, (const unsigned char*)*buff, record->buf_len);
			switch(ret)
			{
			case Z_OK:
				{
					if (len)
					{
						*len = record->org_len;
					}
					delete *buff;
					*buff = uncomp_buff;
				}
				break;
			default:
				Assert(0);
				break;
			}
		}
		break;
	case _7Z_COMPRESS:
		break;
	default:
		break;
	}
	return true;
}