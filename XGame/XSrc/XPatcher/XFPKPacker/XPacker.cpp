/*******************************************************************************
	FILE:		XPacker.cpp
	
	DESCRIPTTION:	
	
	CREATED BY: YangCao, 2014/12/05

	Copyright (C) - All Rights Reserved with Coconat
*******************************************************************************/

#include "XPacker.h"
#include "XMD5.h"

XPacker::XPacker()
{

}

XPacker& XPacker::Get()
{
	static XPacker pack;
	return pack;
}

bool XPacker::Init(const char* src_dir, const char* fpk)
{
	asset_dir = src_dir;
	if(!file_package.CreatePackage(fpk))
	{
		return false;
	}
}

bool XPacker::AddFile(const char* path)
{
	unsigned char digest[16] = {0};
	FILE *file;
	MD5_CTX context;
	int len;
	unsigned char buffer[1024];
	
	std::string full_path = asset_dir + "/";
	full_path += path;
	if ((file = fopen (full_path, "rb")) == NULL)
		return false;
	int len = strlen(path);

	file_package.QuickWriteValue(len);
	else {
		MD5Init (&context);
		while (len = fread (buffer, 1, 1024, file))
		{
			MD5Update (&context, buffer, len);
			if(len != file_package.Write(buffer, 1, len))
			{
				return false;
			}
		}
		MD5Final (digest, &context);
		fclose (file);
	}
	if (16 != file_package.Write(digest, 1, sizeof(digest)))
	{
		return false;
	}

}

bool XPacker::Fllush()
{
	file_package.SavePackageRecords();
	file_package.Flush();
	file_package.CloseFile();
	return true;
}