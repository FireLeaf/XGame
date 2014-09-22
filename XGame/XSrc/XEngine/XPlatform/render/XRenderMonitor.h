/**************************************************************************
/*		FileName: XRenderMonitor.h
/*		FileDesc: 场景渲染器
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/02 20/57/09
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XRENDERMONITOR__H
#define __XRENDERMONITOR__H

#include "XRenderEntity.h"

enum X_RENDER_FLAG
{
	X_RENDER_OPAQUE = 1 << 0,//不透明的
	X_RENDER_TRANSPARENCY = 1 << 1,//透明的，需要blend的
	X_RENDER_POST_PROCESS = 1 << 2,//需要参与post process的
	X_RENDER_SHADOW = 1 << 3,//需要参与阴影的
};

enum X_RENDER_ACTOR
{
	X_ACTOR_NORMAL = 1 << 0;
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
	void AddToRenderList(XRenderEntity* pRenderEntity);
	void MonitorRender();
public:
	//隔离层
	XStl::vector<XRenderEntity*> all_entities;//所有的渲染体
	XStl::vector<XRenderEntity*> shadow_cast_entities;//阴影投射者
	XStl::vector<XRenderEntity*> shadow_recv_entities;//阴影接受者
	XStl::vector<XRenderEntity*> opaque_entities;//不透明渲染体
	XStl::vector<XRenderEntity*> translucent_entities;//半透明
	XStl::vector<XRenderEntity*> post_progress_entities;//post progress

	//辅助层
	XStl::vector<XRenderEntity*> ui_entities;//UI层
};

#endif // XRenderMonitor 