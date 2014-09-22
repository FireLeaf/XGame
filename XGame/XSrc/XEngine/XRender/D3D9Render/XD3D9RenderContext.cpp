/**************************************************************************
/*		FileName: XD3D9RenderContext.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/15 21/52/59
/*		CopyRight: yc 
/*************************************************************************/

#include "XD3D9RenderContext.h"

XD3D9RenderContext d3d9_render_context;
XRenderContext* x_ptr_render_context = &d3d9_render_context;

XD3D9RenderContext::XD3D9RenderContext()
{
	ptr_d3d9_device = NULL;
}

