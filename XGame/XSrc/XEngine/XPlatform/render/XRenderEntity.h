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

struct XAABB
{
	XVector3 left_up;
	XVector3 right_down;
};

struct XRenderArgs
{
	//float mat_view_proj[16];
	XMatrix mat_view;
	XMatrix mat_proj;
	XMatrix mat_view_proj;
	xint32 cur_render_flag;
};

class XAbstract
{
public:
	void GetAABB(){return aabb;}
protected:
	XAABB aabb;
	XVector3 abs_pos;//绝对位置
	XVector3 loc_pos;//相对位置
};

class XRenderEntity : public XAbstract
{
public:
	virtual void Render(XRII* rii, XRenderArgs* args) = 0;
	xint32 GetRenderFlag();
protected:
	xint32 render_flag;
	
};

#endif // XRenderEntity