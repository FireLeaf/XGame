/**************************************************************************
/*		FileName: XRenderEntity.h
/*		FileDesc: 渲染单元
/*		FileAuthor: YangCao
/*		CreateTime: 2014/08/04 22/08/56
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XRENDERENTITY__H
#define __XRENDERENTITY__H
#include "XRII.h"
#include "XMath.h"
#include "XVector.h"
#include "XMatrix.h"

struct CommonVertexPos 
{
	float x,y,z;
};

struct CommonVertexPosDiffuse
{
	float x,y,z;
	xuint32 color;
};

struct CommonVertexPosTex
{
	float x, y, z;
	float u, v;
};

class XRenderViewPort
{

};

struct XRenderArgs
{
	//float mat_view_proj[16];
	XMatrix mat_view;
	XMatrix mat_proj;
	XMatrix mat_view_proj;
	XCamera* ptr_camera;
	XFrustum* ptr_frustum;
	xint32 cur_render_flag;
};

enum X_RENDER_FLAG
{
	X_RENDER_OPAQUE = 1 << 0,//不透明的
	X_RENDER_TRANSPARENCY = 1 << 1,//透明的，需要blend的
	X_RENDER_POST_PROCESS = 1 << 2,//需要参与post process的
	X_RENDER_SHADOW = 1 << 3,//需要参与阴影的
};

enum X_RENDER_ACTOR
{
	X_ACTOR_NORMAL = 1 << 16,
	X_ACTOR_SHADOW_CAST = 1  << 17,
	X_ACTOR_SHADOW_RECV = 1 << 18,
	X_ACTOR_TRANSLUCENT = 1 << 19,
	X_ACTOR_ALPHATEST = 1 << 20,
	X_ACTOR_POST_PROGRESS = 1 << 21,
	X_ACTOR_FOREGROUND = 1 << 22,
	X_ACTOR_BACKGROUND = 1 << 23,
	X_ACTOR_OCCLUSION = 1 << 24,
};

class XAbstract
{
public:
	const XAABB& GetAABB() const {return aabb;}
protected:
	XAABB aabb;
	XVector3 abs_pos;//绝对位置
	XVector3 loc_pos;//相对位置
};

class XRenderEntity : public XAbstract
{
public:
	virtual void Render(XRII* rii, XRenderArgs* args) = 0;
	xint32 GetRenderFlag(){return X_RENDER_OPAQUE;}
protected:
	xint32 render_flag;
	
};

#endif // XRenderEntity