/**************************************************************************
/*		FileName: XWinSys.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/10/14 20:30:22
/*		CopyRight: yc 
/*************************************************************************/

#include "XSys.h"
#include <windows.h>

class XWinMutex : public XMutex
{
public:
	XWinMutex(){InitializeCriticalSection(&cs);}
	virtual ~XWinMutex(){DeleteCriticalSection(&cs);}
	virtual void Lock(){EnterCriticalSection(&cs);}
	virtual void Unlock(){LeaveCriticalSection(&cs);}
protected:
	CRITICAL_SECTION cs;
};

namespace XSys
{
	XMutex* XCreateMutex()
	{
		return (new XWinMutex());
	}
}