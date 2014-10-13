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
	virtual void MonitorRender(XRenderScene* scene);
	virtual void BeginRender(const XSceneDesc*);
	virtual void EndRender(const XSceneDesc*);
};

#endif // XD3D9RenderMonitor