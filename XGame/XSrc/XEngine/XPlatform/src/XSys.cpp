/**************************************************************************
/*		FileName: XSys.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/10/15 22:12:46
/*		CopyRight: yc 
/*************************************************************************/

#include "XSys.h"

namespace XSys
{
	void XDeleteMutex(XMutex* mutex)
	{
		if (mutex)
		{
			delete mutex;
		}
	}
}