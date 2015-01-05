/*******************************************************************************
	FILE:		XUnPacker.h
	
	DESCRIPTTION:	
	
	CREATED BY: YangCao, 2015/01/05

	Copyright (C) - All Rights Reserved with Coconat
*******************************************************************************/

#ifndef __COCONAT_XUNPACKER_H_
#define __COCONAT_XUNPACKER_H_

#include "qt/qstring.h"
class XUnPacker
{
public:
	XUnPacker();

	static XUnPacker& Get();
public:
	bool UnPack(const QString& fpk, const QString& dest_dir);
};

#endif