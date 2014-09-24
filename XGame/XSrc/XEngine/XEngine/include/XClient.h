/**************************************************************************
/*		FileName: XClient.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/15 22:27:19
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XCLIENT__H
#define __XCLIENT__H

#include "XType.h"
#include "XRenderMonitor.h"

class XClient
{
public:
	virtual xbool Init() = 0;
	virtual void Tick() = 0;
	virtual void Render(XRenderMonitor* ptr_render_monitor) = 0;
};

extern XClient* x_ptr_client;
#endif // XClient