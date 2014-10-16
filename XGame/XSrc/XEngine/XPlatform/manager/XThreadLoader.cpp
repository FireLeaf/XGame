/**************************************************************************
/*		FileName: XThreadLoader.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/10/16 20:22:58
/*		CopyRight: yc 
/*************************************************************************/

#include "XThreadLoader.h"

XThreadLoader::~XThreadLoader()
{
	Release();
}

XThreadLoader& XThreadLoader::GetInstance()
{
	static XThreadLoader inst;
	return inst;
}

bool XThreadLoader::Init()
{
	if (ptr_pool)
	{
		Release();
	}

	ptr_pool = XSys::XCreateThreadPool(1);
	if (!ptr_pool)
	{
		Assert(0);
		return false;
	}

	return true;
}

void XThreadLoader::Release()
{
	if (ptr_pool)
	{
		XSys::XReleaseThreadPool(ptr_pool);
	}
}