/**************************************************************************
/*		FileName: XRenderContext.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/15 21/44/44
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XRENDERCONTEXT__H
#define __XRENDERCONTEXT__H

#include "XType.h"

class XRenderContext 
{
public:
	virtual bool Init(xint32 width, xint32 height, xbool windowed) = 0;
	virtual void* GetRenderContext() = 0;
};
extern XRenderContext* x_ptr_render_context;
#endif // XRenderContext
