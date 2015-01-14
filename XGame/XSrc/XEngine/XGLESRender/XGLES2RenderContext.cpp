/**************************************************************************
/*		FileName: XD3D9RenderContext.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/15 21/52/59
/*		CopyRight: yc 
/*************************************************************************/

#include "XD3D9RenderContext.h"
#include "XD3D9AssetMonitor.h"
#include "XRenderScene.h"
#include "XD3D9RenderMonitor.h"
#include "XD3D9Asset.h"

XD3D9RenderContext d3d9_render_context;
XRenderContext* x_ptr_render_context = &d3d9_render_context;
IDirect3DDevice9* x_ptr_d3ddevice = NULL;

XD3D9RenderContext::XD3D9RenderContext()
{
	//x_ptr_d3ddevice = NULL;
}

bool XD3D9RenderContext::Init(xint32 width, xint32 height, xbool windowed)
{
	IDirect3D9 *pD3D9=Direct3DCreate9(D3D_SDK_VERSION);
	if(!pD3D9){
	//	Msg("��ȡIDirect3D9�ӿ�ʧ��");
		return false;
	}

	int vp=0;
	HWND hWnd = GetActiveWindow();

	D3DCAPS9 caps;

	//�鿴�����Ƿ�֧��Ӳ�����㴦��
	pD3D9->GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&caps);
	if(caps.DevCaps&D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp=D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp=D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	//����D3D����
	d3dpp.AutoDepthStencilFormat=D3DFMT_D24S8;//��̨�洢��ʽ
	d3dpp.BackBufferCount=1;//��̨����
	d3dpp.BackBufferFormat=D3DFMT_A8R8G8B8;//��̨���ظ�ʽ
	d3dpp.BackBufferHeight=width;//��̨�߶�
	d3dpp.BackBufferWidth=height;//��̨���
	d3dpp.EnableAutoDepthStencil=true;//������̨����
	d3dpp.Flags=0;//�����־
	d3dpp.FullScreen_RefreshRateInHz=D3DPRESENT_RATE_DEFAULT;//ˢ����
	d3dpp.hDeviceWindow=hWnd;//�������ھ��
	d3dpp.MultiSampleQuality=0;//��������
	d3dpp.MultiSampleType=D3DMULTISAMPLE_NONE;//�������
	d3dpp.PresentationInterval=D3DPRESENT_INTERVAL_IMMEDIATE;//�ύģʽ
	d3dpp.SwapEffect=D3DSWAPEFFECT_DISCARD;//����Ч��
	d3dpp.Windowed=windowed;//�Ƿ񴰿ڻ�
	//HWND GetForegroundWindow();
	HRESULT hr=pD3D9->CreateDevice//�����豸���� 
		(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,hWnd,vp,
		&d3dpp, &x_ptr_d3ddevice
		);
	if(FAILED(hr))
	{
		if (pD3D9)
		{
			pD3D9->Release();
		}
		return false;

	}

	if(pD3D9) pD3D9->Release();
	
	BuildDefaultSceneDesc();

	return true;
}

void XD3D9RenderContext::Resize(xint32 width, xint32 height, xbool windowed)
{
	if (x_ptr_d3ddevice)
	{
		d3dpp.BackBufferWidth = width;
		d3dpp.BackBufferHeight = height;
		x_ptr_d3ddevice->Reset(&d3dpp);
		x_ptr_asset_monitor->ResetAsset();
		BuildDefaultSceneDesc();
	}
}

void XD3D9RenderContext::BuildDefaultSceneDesc()
{
	XSceneDesc** pptr_default_desc = x_ptr_render_monitor->GetDefaultSceneDesc();
	if (*pptr_default_desc)
	{
		delete *pptr_default_desc;
	}
	*pptr_default_desc = new XSceneDesc;
	XSceneDesc* ptr_default_desc = *pptr_default_desc;
	IDirect3DSurface9* pRenderTarget = NULL;
	IDirect3DSurface9* pDepthStencil = NULL;
	x_ptr_d3ddevice->GetDepthStencilSurface(&pDepthStencil);
	x_ptr_d3ddevice->GetRenderTarget(0, &pRenderTarget);
	XD3D9RenderTarget* pD3D9RenderTarget = new XD3D9RenderTarget(pRenderTarget, pDepthStencil);
	ptr_default_desc->ptr_asset_depth_stencil = pD3D9RenderTarget;
	ptr_default_desc->render_target_count = 0;
}