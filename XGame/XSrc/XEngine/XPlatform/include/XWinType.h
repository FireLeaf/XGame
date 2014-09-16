/**************************************************************************
/*		FileName: XWinType.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/07/29 19/45/22
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XWINTYPE__H
#define __XWINTYPE__H

#ifdef _WIN_PLATFORM
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
#include <string>
#include <vector>
#endif

#endif // XWinType