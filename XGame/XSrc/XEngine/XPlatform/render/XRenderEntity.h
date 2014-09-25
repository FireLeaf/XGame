/**************************************************************************
/*		FileName: XRenderEntity.h
/*		FileDesc: äÖÈ¾µ¥Ôª
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

struct XRenderArgs
{
	float mat_view_proj[16];
};

class XRenderEntity
{
public:
	virtual void Render(XRII* rii, XRenderArgs* args) = 0;
};

#endif // XRenderEntity