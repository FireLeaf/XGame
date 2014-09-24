/**************************************************************************
/*		FileName: XD3D9RenderMonitor.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/15 21/27/31
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XD3D9RENDERMONITOR__H
#define __XD3D9RENDERMONITOR__H

#include "XRenderMonitor.h"

class XD3D9RenderMonitor : public XRenderMonitor
{
public:
	virtual void AddToRenderList(XRenderEntity* pRenderEntity);
	virtual void MonitorRender();
	virtual void BeginRender();
	virtual void EndRender();
};

#endif // XD3D9RenderMonitor