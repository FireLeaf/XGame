/**************************************************************************
/*		FileName: XShell.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/15 22/03/46
/*		CopyRight: yc 
/*************************************************************************/

#include "XClient.h"
#include "XShell.h"
#include "XRenderContext.h"

bool XShell::Init(const XInitContext* ptr_ini_context)
{
	x_ptr_render_context->Init(ptr_ini_context->width, ptr_ini_context->height, ptr_ini_context->windowed);

	if (!x_ptr_client)
	{
		return false;
	}

	return x_ptr_client->Init();
}

void XShell::Frame()
{
	if (x_ptr_client)
	{
		x_ptr_render_monitor->BeginRender();
		x_ptr_client->Tick();
		x_ptr_client->Render(x_ptr_render_monitor);
		x_ptr_render_monitor->MonitorRender();
		x_ptr_render_monitor->EndRender();
	}
}