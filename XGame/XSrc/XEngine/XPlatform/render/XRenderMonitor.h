/**************************************************************************
/*		FileName: XRenderMonitor.h
/*		FileDesc: ³¡¾°äÖÈ¾Æ÷
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/02 20/57/09
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XRENDERMONITOR__H
#define __XRENDERMONITOR__H

#include "XRenderEntity.h"

class XRenderScene;

class XRenderGroup
{
protected:
	XRenderFlag render_flag;
	XStl::vector<XRenderEntity*> render_entities;
};

class XRenderMonitor
{
public:
	virtual void BeginRender(const XSceneDesc&);
	virtual void EndRender(const XSceneDesc&);
	virtual void MonitorRender(XRenderScene* scene) = 0;
public:
//	virtual void AddToRenderList(XRenderEntity* pRenderEntity) = 0;
};

extern XRenderMonitor* x_ptr_render_monitor;

#endif // XRenderMonitor 