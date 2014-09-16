/**************************************************************************
/*		FileName: XRenderEntity.h
/*		FileDesc: äÖÈ¾µ¥Ôª
/*		FileAuthor: YangCao
/*		CreateTime: 2014/08/04 22/08/56
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XRENDERENTITY__H
#define __XRENDERENTITY__H

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

class XRenderEntity
{
public:
	virtual void Render(XRenderSetting* pSetting);
};

#endif // XRenderEntity