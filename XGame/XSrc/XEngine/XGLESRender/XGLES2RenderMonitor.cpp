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
#include "XD3D9Asset.h"
#include "XRenderUtil.h"

XD3D9RenderMonitor d3d9_render_monitor;
XRenderMonitor* x_ptr_render_monitor = &d3d9_render_monitor;


void XD3D9RenderMonitor::MonitorRender(XRenderScene* scene)
{
	if (!scene)
	{
		return;
	}

	XD3D9RII d3d9rii;
	XSceneDesc* scene_desc = scene->GetSceneDesc();
	if (!scene_desc)
	{
		scene_desc = *(x_ptr_render_monitor->GetDefaultSceneDesc());
	}
	Assert(scene_desc);
	XRenderArgs ra;
	scene->ClassifyRenderList();
	BeginRender(scene_desc);
	
	ra.mat_view = scene_desc->camera.GetViewMatrix();
	ra.mat_proj = scene_desc->view_port.GetProjMatrix();

	ra.mat_view_proj = ra.mat_view * ra.mat_proj;
	D3DXMATRIX matView;
	D3DXVECTOR3 eye(scene_desc->camera.eye.x, scene_desc->camera.eye.y, scene_desc->camera.eye.z);
	D3DXVECTOR3 up(scene_desc->camera.up.x, scene_desc->camera.up.y, scene_desc->camera.up.z);
	D3DXVECTOR3 at(scene_desc->camera.at.x, scene_desc->camera.at.y, scene_desc->camera.at.z);
	D3DXMatrixLookAtLH(&matView, &eye, &at, &up);
	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, scene_desc->view_port.fov, scene_desc->view_port.width / (float)scene_desc->view_port.height, scene_desc->view_port.z_near, scene_desc->view_port.z_far);
	D3DXMATRIX matViewProj;
	D3DXMatrixMultiply(&matViewProj, &matView, &matProj);
	
// 	for (int i = 0; i < scene->all_entities.size(); i++)
// 	{
// 		scene->all_entities[i]->Render(&d3d9rii, &ra);
// 	}

	//back_ground
	for (int i = 0; i < (int)scene->background_entities.size(); i++)
	{
		ra.cur_render_flag = X_ACTOR_BACKGROUND;
		scene->background_entities[i]->Render(&d3d9rii, &ra);
	}

	//shadow

	//oqupae
	for (int i = 0; i < (int)scene->opaque_entities.size(); i++)
	{
		ra.cur_render_flag = X_RENDER_OPAQUE;
		scene->opaque_entities[i]->Render(&d3d9rii, &ra);
	}
	EndRender(scene_desc);
}

void XD3D9RenderMonitor::BeginRender(const XSceneDesc* scene_desc)
{
	if (scene_desc->render_target_count)
	{
		for (int i = 0; i < XSceneDesc::MAX_RENDER_TARGET; i++)
		{
			if(i < scene_desc->render_target_count)
				x_ptr_d3ddevice->SetRenderTarget(i, ((XD3D9RenderTarget*)scene_desc->ptr_asset_render_target[i])->GetD3D9RenderTarget());
			else
				x_ptr_d3ddevice->SetRenderTarget(i, NULL);

			x_ptr_d3ddevice->SetDepthStencilSurface(((XD3D9RenderTarget*)scene_desc->ptr_asset_depth_stencil)->GetD3D9DepthSurface());
		}
	}
	else
	{
		for (int i = 0; i < XSceneDesc::MAX_RENDER_TARGET; i++)
		{
			if (i == 0)
				x_ptr_d3ddevice->SetRenderTarget(i, ((XD3D9RenderTarget*)scene_desc->ptr_asset_depth_stencil)->GetD3D9RenderTarget());
			else
			
				x_ptr_d3ddevice->SetRenderTarget(i, NULL);

			x_ptr_d3ddevice->SetDepthStencilSurface(((XD3D9RenderTarget*)scene_desc->ptr_asset_depth_stencil)->GetD3D9DepthSurface());
		}
	}
	x_ptr_d3ddevice->Clear(0, NULL, RenderUtil::GetClearBufferFlag(scene_desc->clear_buffer_flag), scene_desc->clear_color, 1.0f, 0);
	x_ptr_d3ddevice->BeginScene();
}

void XD3D9RenderMonitor::EndRender(const XSceneDesc* scene_desc)
{
	x_ptr_d3ddevice->EndScene();
	if(0 == scene_desc->render_target_count)
		x_ptr_d3ddevice->Present(NULL, NULL, 0, NULL);
}