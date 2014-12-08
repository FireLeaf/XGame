/*******************************************************************************
	FILE:		XPacker.cpp
	
	DESCRIPTTION:	
	
	CREATED BY: YangCao, 2014/12/05

	Copyright (C) - All Rights Reserved with Coconat
*******************************************************************************/

#include "XPacker.h"
#include "XMD5.h"
#include "XBufferStream.h"

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
	return true;
}

bool XPacker::AddFile(const char* path)
{
	std::string full_path = asset_dir + "/";
	full_path += path;
	return file_package.AddFile(full_path.c_str(), path);
}

bool XPacker::Fllush()
{
	file_package.SavePackageRecords();
	file_package.Flush();
	file_package.CloseFile();
	return true;
}