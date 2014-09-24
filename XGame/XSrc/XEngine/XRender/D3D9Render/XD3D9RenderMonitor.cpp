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

void XD3D9RenderMonitor::AddToRenderList(XRenderEntity* pRenderEntity)
{
	all_entities.push_back(pRenderEntity);
}

void XD3D9RenderMonitor::MonitorRender()
{
	XD3D9RII d3d9rii;

	D3DXVECTOR3 eye(5.0f, 5.0f, -20.0f), up(0.0f, 1.0f, 0.0f), at(0.0f, 0.0f, 0.0f);
	D3DXMATRIX mat_view;
	D3DXMatrixLookAtLH(&mat_view, &eye, &at, &up);

	D3DXMATRIX mat_proj;
	D3DXMatrixPerspectiveFovLH(&mat_proj, D3DX_PI * 0.25f, 1024/768.0f, 0.1f, 1000.0f);
	
	D3DXMATRIX mat_view_proj = mat_view * mat_proj;
	XRenderArgs ra;
	memcpy(&ra.mat_view_proj, &mat_view_proj, sizeof(ra.mat_view_proj));
	
	for (int i = 0; i < all_entities.size(); i++)
	{
		all_entities[i]->Render(&d3d9rii, &ra);
	}
}

void XD3D9RenderMonitor::BeginRender()
{
	all_entities.clear();
	x_ptr_d3ddevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, 0xff0000ff, 1.0f, 0);
	x_ptr_d3ddevice->BeginScene();
}

void XD3D9RenderMonitor::EndRender()
{
	x_ptr_d3ddevice->EndScene();
	x_ptr_d3ddevice->Present(NULL, NULL, 0, NULL);
}