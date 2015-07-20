/*******************************************************************************
	FILE:		XFilePackManager.cpp
	
	DESCRIPTTION:	
	
	CREATED BY: YangCao, 2015/01/16

	Copyright (C) - All Rights Reserved with Coconat
*******************************************************************************/

#include "XFilePackManager.h"

XFilePackManage::XFilePackManage()
{

}

XFilePackManage& XFilePackManage::Get()
{
	static XFilePackManage inst;
	return inst;
}

XFilePackageEasy* XFilePackManage::FindPack(const char* pack_name)
{
	if (!pack_name)
	{
		return NULL;
	}

	for (int i = 0; i < file_pack_items.size(); i++)
	{
		if (0 == strcmp(pack_name, file_pack_items[i]->pack_name.c_str()))
		{
			return file_pack_items[i]->file_pack_easy;
		}
	}
	return NULL;
}

bool XFilePackManage::AddPack(const char* pack_path)
{
	XFilePackageEasy* new_fpe = new XFilePackageEasy;
	if (new_fpe->InitPackage(pack_path))
	{
		FilePackItem* item = new FilePackItem;
		item->pack_name = pack_path;
		item->file_pack_easy = new_fpe;
		file_pack_items.push_back(item);
		return true;
	}
	else
	{
		delete new_fpe;
		return false;
	}
	return false;
}

bool XFilePackManage::AddFile(const char* pack_name, const char* file_path, const void* buffer, int length)
{
	XFilePackageEasy* fpe = FindPack(pack_name);
	if (!fpe)
	{
		return false;
	}

	return fpe->AppendFileFromData(file_path, (const unsigned char*)buffer, length);
}

bool XFilePackManage::DelFile(const char* pack_name, const char* file_path)
{
	XFilePackageEasy* fpe = FindPack(pack_name);
	if (!fpe)
	{
		return false;
	}

	return fpe->RemoveFile(file_path);
}

bool XFilePackManage::ModFile(const char* pack_name, const char* file_path, const void* buffer, int length)
{
	XFilePackageEasy* fpe = FindPack(pack_name);
	if (!fpe)
	{
		return false;
	}

	return fpe->RewriteFileFromData(file_path, (const unsigned char*)buffer, length);
}