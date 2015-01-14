/****************************************************
 *		FileName: XLuaPubFunc.cpp
 *		FileDesc: Lua 扩展库 公用函数
 *		FileAuthor:
 *		CreateTime: 2015/01/11 22:09:40
 *		CopyRight:
 ***************************************************/

#include "XLuaPubFunc.h"

#pragma warning( push )
#pragma warning( disable : 4996 )

void OutputDebug( char* format, ... )
{
#ifdef _DEBUG
	char szStr[8 * 1024] = {0};

	va_list argPtr;

	va_start(argPtr, format);
	vsprintf(szStr, format, argPtr);
	va_end(argPtr);

	OutputDebugStr(szStr);
#endif
}

#pragma warning( pop )
