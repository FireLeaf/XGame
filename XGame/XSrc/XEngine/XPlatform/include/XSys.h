/**************************************************************************
/*		FileName: XSys.h
/*		FileDesc: 
/*		FileAuthor: YangCaoϵͳ
/*		CreateTime: 2014/10/14 20:28:23
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XSYS__H
#define __XSYS__H

namespace XSys
{
	class XMutex
	{
	public:
		virtual void Lock() = 0;
		virtual void Unlock() = 0;
	};
	class XLock;
}

#endif // XSys