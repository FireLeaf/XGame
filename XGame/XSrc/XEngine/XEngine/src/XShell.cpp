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
#include "XRenderMonitor.h"
#include "XRenderScene.h"
#include "XThreadLoader.h"

XShell::XShell()
{
	ptr_render_scene = NULL;
}

bool XShell::Init(const XInitContext* ptr_ini_context)
{
	x_ptr_render_context->Init(ptr_ini_context->width, ptr_ini_context->height, ptr_ini_context->windowed);
	if(!XThreadLoader::GetInstance().Init())
	{
		return false;
	}

	if (!x_ptr_client)
	{
		return false;
	}
	ptr_render_scene = new XRenderScene;
	ptr_render_scene->SetSceneDesc(*(x_ptr_render_monitor->GetDefaultSceneDesc()));
	return (bool)x_ptr_client->Init();
}

void XShell::Reset(const XInitContext* ptr_rest_context)
{
	x_ptr_render_context->Resize(ptr_rest_context->width, ptr_rest_context->height, ptr_rest_context->windowed);
	ptr_render_scene->SetSceneDesc(*(x_ptr_render_monitor->GetDefaultSceneDesc()));
}

void XShell::Frame()
{
	if (x_ptr_client)
	{
		
		ptr_render_scene->BeginScene();
		x_ptr_client->Tick();
		x_ptr_client->Render(ptr_render_scene);
		x_ptr_render_monitor->MonitorRender(ptr_render_scene);
	}
}