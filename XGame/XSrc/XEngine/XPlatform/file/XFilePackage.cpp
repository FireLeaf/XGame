/**************************************************************************
/*		FileName: XFilePackage.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/11/16 13:42:39
/*		CopyRight: yc 
/*************************************************************************/

#include "XFilePackage.h"

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
}

bool XFilePackageEasy::AppendFile(const char* path, const unsigned char* buffer, int length)
{

	compress()
}