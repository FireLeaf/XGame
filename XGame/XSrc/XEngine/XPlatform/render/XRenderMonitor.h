/**************************************************************************
/*		FileName: XRenderMonitor.h
/*		FileDesc: ������Ⱦ��
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/02 20/57/09
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XRENDERMONITOR__H
#define __XRENDERMONITOR__H

#include "XRenderEntity.h"

enum X_RENDER_FLAG
{
	X_RENDER_OPAQUE = 1 << 0,//��͸����
	X_RENDER_TRANSPARENCY = 1 << 1,//͸���ģ���Ҫblend��
	X_RENDER_POST_PROCESS = 1 << 2,//��Ҫ����post process��
	X_RENDER_SHADOW = 1 << 3,//��Ҫ������Ӱ��
};

enum X_RENDER_ACTOR
{
	X_ACTOR_NORMAL = 1 << 0,
	X_ACTOR_SHADOW_CAST = 1  << 1,
	X_ACTOR_SHADOW_RECV = 1 << 2,
	X_ACTOR_TRANSLUCENT = 1 << 3,
	X_ACTOR_POST_PROGRESS = 1 << 4,
	X_ACTOR_UI = 1 << 5,
};

class XRenderGroup
{

protected:
	XRenderFlag render_flag;
	XStl::vector<XRenderEntity*> render_entities;
};

class XRenderMonitor
{
public:
	virtual void BeginRender() = 0;
	virtual void EndRender() = 0;
	virtual void MonitorRender() = 0;
};

extern XRenderMonitor* x_ptr_render_monitor;

#endif // XRenderMonitor 