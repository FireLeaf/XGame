/*******************************************************************************
	FILE:		XFilePackManager.h
	
	DESCRIPTTION:	
	
	CREATED BY: YangCao, 2015/01/16

	Copyright (C) - All Rights Reserved with Coconat
*******************************************************************************/

#ifndef __COCONAT_XFILEPACKMANAGER_H_
#define __COCONAT_XFILEPACKMANAGER_H_

#include "XFilePackage.h"
#include <string>

class XFilePackManage
{
public:
	struct FilePackItem 
	{
		std::string pack_name;//包名
		XFilePackageEasy* file_pack_easy;
	};
public:
	XFilePackManage();
public:
	static XFilePackManage& Get();
public:
	bool AddPack(const char* pack_path);//增加一个包
	//
	bool AddFile(const char* pack_name, const char* file_path, const void* buffer, int length);
	bool DelFile(const char* pack_name, const char* file_path);
	bool ModFile(const char* pack_name, const char* file_path, const void* buffer, int length);
	//bool RepFile(const char* pack_name, const char*);
	XFilePackageEasy* FindPack(const char* pack_name);
protected:
	std::vector<FilePackItem*> file_pack_items;
};

#endif