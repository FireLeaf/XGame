/**************************************************************************
/*		FileName: XThreadLoader.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/10/16 20:33:32
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XTHREADLOADER__H
#define __XTHREADLOADER__H

#include "XSys.h"

class XThreadLoader
{
public:
	XThreadLoader() : ptr_pool(NULL){}
	~XThreadLoader();
	bool Init();
	void Release();
	static XThreadLoader& GetInstance();
public:
	void PushJob(const XJob& job){if(ptr_pool) ptr_pool->DoJob(job);}
protected:
	XThreadPool* ptr_pool;
};

#endif // XThreadLoader