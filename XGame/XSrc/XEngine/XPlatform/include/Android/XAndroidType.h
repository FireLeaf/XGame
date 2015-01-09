/*******************************************************************************
	FILE:		XAndroidType.h
	
	DESCRIPTTION:	
	
	CREATED BY: YangCao, 2015/01/08

	Copyright (C) - All Rights Reserved with Coconat
*******************************************************************************/

#ifndef __COCONAT_XANDROIDTYPE_H_
#define __COCONAT_XANDROIDTYPE_H_

#ifdef _ANDROID_PLATFORM
typedef char xchar;
typedef unsigned char xbyte;
typedef short xshort;
typedef unsigned short xushort;
typedef int xint32;
typedef unsigned int xuint32;
typedef __int64 xint64;
typedef unsigned __int64 xuint64;
typedef long xlong;
typedef unsigned long xulong;
typedef unsigned char xbool;
#endif

#endif