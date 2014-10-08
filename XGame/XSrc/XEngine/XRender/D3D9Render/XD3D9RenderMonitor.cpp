/**************************************************************************
/*		FileName: XD3D9RenderMonitor.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/23 21:21:52
/*		CopyRight: yc 
/*************************************************************************/

#include "XD3D9RenderMonitor.h"
#include "XD3D9RenderContext.h"
#include "XD3D9RII.h"

XD3D9RenderMonitor d3d9_render_monitor;
XRenderMonitor* x_ptr_render_monitor = &d3d9_render_monitor;


void XD3D9RenderMonitor::MonitorRender(XRenderScene* scene)
{
	if (!scene)
	{
		return;
	}

	XD3D9RII d3d9rii;
	XSceneDesc& scene_desc = scene->GetSceneDesc();
	XRenderArgs ra;
	
	BeginRender(scene_desc);
	
	ra.mat_view = scene_desc.camera.GetViewMatrix();
	ra.mat_proj = scene_desc.view_port.GetProjMatrix();
	ra.mat_view_proj = ra.mat_view * ra.mat_proj;
	
// 	for (int i = 0; i < scene->all_entities.size(); i++)
// 	{
// 		scene->all_entities[i]->Render(&d3d9rii, &ra);
// 	}

	//back_ground
	for (int i = 0; i < scene->background_entities.size(); i++)
	{
		ra.cur_render_flag = X_ACTOR_BACKGROUND;
		scene->background_entities->Render(&d3d9rii, &ra);
	}

	//shadow

	//oqupae
	for (int i = 0; i < scene->opaque_entities.size(); i++)
	{
		ra.cur_render_flag = X_RENDER_OPAQUE;
		scene->opaque_entities->Render(&d3d9rii, &ra);
	}
	EndRender(scene_desc);
}

void XD3D9RenderMonitor::BeginRender(const XSceneDesc& scene_desc)
{
	x_ptr_d3ddevice->Clear(0, NULL, RenderUtil::GetClearBufferFlag(scene_desc.clear_buffer_flag), scene_desc.clear_color, 1.0f, 0);
	x_ptr_d3ddevice->BeginScene();
}

void XD3D9RenderMonitor::EndRender(const XSceneDesc& scene_desc)
{
	x_ptr_d3ddevice->EndScene();
	if(scene_desc.is_device_target)
		x_ptr_d3ddevice->Present(NULL, NULL, 0, NULL);
}