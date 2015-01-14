/**************************************************************************
/*		FileName: XD3D9RenderContext.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/15 21/47/51
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XD3D9RENDERCONTEXT__H
#define __XD3D9RENDERCONTEXT__H

#include "XRenderContext.h"
#include <d3dx9.h>

class XD3D9RenderContext : public XRenderContext
{
public:
	XD3D9RenderContext();

	virtual bool Init(xint32 width, xint32 height, xbool windowed);
	virtual void Resize(xint32 width, xint32 height, xbool windowed);

	virtual void* GetRenderContext()
	{
		//return (void*)ptr_d3d9_device;
		return NULL;
	}
protected:
	void BuildDefaultSceneDesc();
protected:
	//IDirect3DDevice9*	ptr_d3d9_device;
	D3DPRESENT_PARAMETERS d3dpp;
};
extern IDirect3DDevice9* x_ptr_d3ddevice;
#endif // XD3D9RenderContext