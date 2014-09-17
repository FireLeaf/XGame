/**************************************************************************
/*		FileName: XShell.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/15 22/03/26
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XSHELL__H
#define __XSHELL__H

#include "XType.h"
#include "XTemplate.h"

struct XInitContext 
{
	xint32 width;
	xint32 height;
	xbool windowed;
	void* hwnd;
	XStl::string work_dir;
	//XStl::string asset_dir;
	//XStl::string 
};

class XShell;

class XShell : public Singleton<XShell>
{
public:
	bool Init(const XInitContext* ptr_ini_context);
	void Frame();
};


#endif // XShell