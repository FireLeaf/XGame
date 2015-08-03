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

#define FILE_CLIP_MIN_SIZE 1024

bool XFilePackageEasy::InitPackage(const char* fpk_file)
{
	CloseFile();
	if (!OpenFile(fpk_file, "r+b"))
	{
		if (!OpenFile(fpk_file, "rb"))
		{
			return false;
		}
	}
	SeekSet(0);
	
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
	//return true;
	
	bool bRet = LoadPackage(header.version);
	return bRet;
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
	file_tail = records_offset;
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
		package_tail.package_records.push_back(epp);
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
	
	file_tail = Tell();
	return true;
}

bool XFilePackageEasy::SavePackageRecords()
{
	int records_offset = 0;
	Seek(file_tail, SEEK_SET);
	QuickWriteValue(package_tail.package_records.size());
	records_offset = file_tail;
	for (int i = 0; i < package_tail.package_records.size(); i++)
	{
		QuickWriteValue(package_tail.package_records[i].path.length());
		Write(package_tail.package_records[i].path.c_str(), 1, package_tail.package_records[i].path.length());
		QuickWriteValue(package_tail.package_records[i].compress_type);
		QuickWriteValue(package_tail.package_records[i].buf_len);
		QuickWriteValue(package_tail.package_records[i].org_len);
		QuickWriteValue(package_tail.package_records[i].offset);
	}
	QuickWriteValue(records_offset);
	return true;
}

struct FindRecordCmp 
{
	FindRecordCmp(const char* path):str_cmp(path){}
	bool operator()(const XFilePackageEasy::XEasyPackageRecord& epr)
	{
		return strcmp(str_cmp.c_str(), epr.path.c_str()) == 0;
	}
private:
	std::string str_cmp;
};

bool XFilePackageEasy::AppendFileFromData(const char* path, const unsigned char* buffer, int length)
{
	XEasyPackageRecord* record = FindRecord(path);
	if (record)
	{
		RemoveFile(path);
		//return RewriteFileFromData(path, buffer, length);
	}
	record = AddRecord(path);
	if (!record)
	{
		return false;
	}
	bool bAddBuffer = AddBufferZlib(record, buffer, length);
	if(!bAddBuffer)
	{
		RemoveFile(path);
		Assert(0);
		return false;
	}
	return true;
}

// bool XFilePackageEasy::ReplaceFileFromData(const char* old_path, const char* cur_path, const unsigned char* buffer, int length)
// {
// 	RemoveFile(old_path);
// 	if (0 == strcmp(old_path, cur_path))
// 	{
// 		return RewriteFileFromData(cur_path, buffer, length);
// 	}
// 	XEasyPackageRecord* record = FindRecord(old_path);
// 	if (!record)//如果老记录没有
// 	{
// // 		record = FindRecord(cur_path);
// // 		if (record)
// // 		{
// // 			
// // 		}
// // 		else
// // 		{
// // 			return AppendFile(cur_path, buffer, length);
// // 		}
// 		return false;
// 	}
// 	else
// 	{
// 		XEasyPackageRecord* cur_record = FindRecord(cur_path);
// 		if (cur_record)
// 		{
// 			//如果由当前记录，那么直接重写
// 			return RewriteFileFromData(cur_path, buffer, length);
// 		}
// 		else
// 		{
// 			//如果没有当前记录，那么添加一个
// 			record->path = cur_path;
// 			AppendFileFromData(cur_path, buffer, length);
// 		}
// 	}
// 	return false;
// }

bool XFilePackageEasy::RewriteFileFromData(const char* path, const unsigned char* buffer, int length)
{
	XEasyPackageRecord* cur_record = FindRecord(path);
	if (cur_record)
	{
		RemoveFile(path);
		return AppendFileFromData(path, buffer, length);//AddBufferZlib(cur_record, buffer, length);
	}
	return true;
}

bool XFilePackageEasy::RemoveFile(const char* path)
{
	XEasyPackageRecord* record = FindRecord(path);
	if (record)
	{
		//仅记录超过FILE_CLIP_MIN_SIZE大小的碎片
		if(record->buf_len > FILE_CLIP_MIN_SIZE)
			package_tail.chip_records.insert(ChipRecord( record->buf_len, record->offset));

		PackageRecords::iterator iter = std::find_if(package_tail.package_records.begin(), package_tail.package_records.end(), FindRecordCmp(path));
		package_tail.package_records.erase(iter);
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
// 		int path_len = strlen(path);
// 		QuickWriteValue(path_len);//写入文件名长度
// 		if(path_len != Write(path, 1, path_len))//写入文件名
// 		{
// 			return false;
// 		}
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
	bool bAddBuffer = AddBufferZlib(record, (const unsigned char*)isvs.GetData(), isvs.Length());
	if (!bAddBuffer)
	{
		RemoveFile(path);
		return false;
	}
// 	if (16 != Write(digest, 1, sizeof(digest)))//写入md5
// 	{
// 		return false;
// 	}
	//cur_offset = Tell();//重置当前偏移
	return true;
}

bool XFilePackageEasy::FindMatchRecord(XFilePackageEasy::ChipRecord& chip_record, int length)
{
	for (ChipRecords::iterator iter = package_tail.chip_records.begin(); iter != package_tail.chip_records.end(); iter++)
	{
		if (length <= iter->reserve_len)
		{
			int new_reserve = iter->reserve_len - length;
			if (new_reserve > FILE_CLIP_MIN_SIZE)
			{
				package_tail.chip_records.insert(ChipRecord( new_reserve, iter->offset + length));
			}
			chip_record = *iter;
			package_tail.chip_records.erase(iter);
			return false;
		}
	}
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
			bool bAppend = false;
			bool bCompress = false;
			if (dest_len >= length)//如果压缩后比当前大那么直接弄原数据
			{
				record->compress_type = NONE_COMPRESS;
				record->buf_len = length;
			}
			else//写入压缩的数据
			{
				record->compress_type = Z_LIB_COMPRESS;
				record->buf_len = dest_len;
				bCompress = true;
			}
			ChipRecord chip_record(0, 0);
			if( !(bAppend = FindMatchRecord(chip_record, record->buf_len)) )
			{
				record->offset = chip_record.offset;
			}
			else
			{
				record->offset = file_tail;
			}
			SeekSet(record->offset);
			if(record->buf_len != SafeWrite(bCompress ? dest_buffer : buffer,1, record->buf_len, XFILE_PACKAGE_SAFE_SIZE))
			{
				delete dest_buffer;
				//未能完全写入
				return false;
			}
			if (bAppend)
			{
				file_tail += record->buf_len;
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
			record->buf_len = length;
			ChipRecord chip_record(0, 0);
			if(FindMatchRecord(chip_record, record->buf_len))
			{
				record->offset = chip_record.offset;
			}
			else
			{
				record->offset = file_tail;
			}
			SeekSet(record->offset);
			if(length!= SafeWrite(buffer,1, record->buf_len, XFILE_PACKAGE_SAFE_SIZE))
			{
				delete dest_buffer;
				Assert(0);
				//未能完全写入
				return false;
			}
			file_tail += record->buf_len;
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
	
	PackageRecords::iterator iter = std::find_if(package_tail.package_records.begin(), package_tail.package_records.end(), FindRecordCmp(path));

	if (iter == package_tail.package_records.end())
	{
		return NULL;
	}

	return &(*iter);
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
	package_tail.package_records.push_back(record);
	return &(package_tail.package_records[package_tail.package_records.size() - 1]);
}

bool XFilePackageEasy::ReadFileContent(const char* path, void** buff, int* len)
{
	XEasyPackageRecord* record = FindRecord(path);
	if (!record)
	{
		return false;
	}
	return ReadFileContent(record, buff, len);
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