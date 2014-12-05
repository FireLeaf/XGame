/*******************************************************************************
	FILE:		XPacker.h
	
	DESCRIPTTION:	
	
	CREATED BY: YangCao, 2014/12/05

	Copyright (C) - All Rights Reserved with Coconat
*******************************************************************************/

#ifndef __COCONAT_XPACKER_H_
#define __COCONAT_XPACKER_H_

#include "XFilePackage.h"

class XPacker
{
public:
	XPacker();
	static XPacker& Get();

	bool Init(const char* src_dir, const char* fpk);
	bool Fllush();
	bool AddFile(const char* path);
protected:
	XFilePackageEasy file_package;
	std::string asset_dir;
};

#endif