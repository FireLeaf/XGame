/****************************************************
 *		FileName: XLuaPubFunc.cpp
 *		FileDesc: Lua 扩展库 公用函数
 *		FileAuthor:
 *		CreateTime: 2015/01/11 22:09:40
 *		CopyRight:
 ***************************************************/

#include "XLuaPubFunc.h"

#ifdef WIN32
#pragma warning( push )
#pragma warning( disable : 4996 )
#endif

void OutputDebug( char* format, ... )
{
#ifdef _DEBUG
	char szStr[8 * 1024] = {0};

	va_list argPtr;

	va_start(argPtr, format);
	vsprintf(szStr, format, argPtr);
	va_end(argPtr);

	// 针对不同平台特殊处理
#ifdef WIN32
	OutputDebugStr(szStr);
#endif
#endif
}

#ifdef WIN32
#pragma warning( pop )
#endif
