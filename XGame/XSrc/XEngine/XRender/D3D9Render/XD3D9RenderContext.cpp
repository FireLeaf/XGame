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
IDirect3DDevice9* x_ptr_d3ddevice = NULL;

XD3D9RenderContext::XD3D9RenderContext()
{
	//x_ptr_d3ddevice = NULL;
}

bool XD3D9RenderContext::Init(xint32 width, xint32 height, xbool windowed)
{
	IDirect3D9 *pD3D9=Direct3DCreate9(D3D_SDK_VERSION);
	if(!pD3D9){
	//	Msg("获取IDirect3D9接口失败");
		return false;
	}

	int vp=0;
	HWND hWnd = GetActiveWindow();

	D3DCAPS9 caps;

	//查看窗口是否支持硬件顶点处理
	pD3D9->GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&caps);
	if(caps.DevCaps&D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp=D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp=D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	//设置D3D属性
	d3dpp.AutoDepthStencilFormat=D3DFMT_D24S8;//后台存储格式
	d3dpp.BackBufferCount=1;//后台个数
	d3dpp.BackBufferFormat=D3DFMT_A8R8G8B8;//后台像素格式
	d3dpp.BackBufferHeight=width;//后台高度
	d3dpp.BackBufferWidth=height;//后台宽度
	d3dpp.EnableAutoDepthStencil=true;//启动后台缓存
	d3dpp.Flags=0;//特殊标志
	d3dpp.FullScreen_RefreshRateInHz=D3DPRESENT_RATE_DEFAULT;//刷新率
	d3dpp.hDeviceWindow=hWnd;//关联窗口句柄
	d3dpp.MultiSampleQuality=0;//采样质量
	d3dpp.MultiSampleType=D3DMULTISAMPLE_NONE;//采样类别
	d3dpp.PresentationInterval=D3DPRESENT_INTERVAL_IMMEDIATE;//提交模式
	d3dpp.SwapEffect=D3DSWAPEFFECT_DISCARD;//交换效率
	d3dpp.Windowed=windowed;//是否窗口化
	//HWND GetForegroundWindow();
	HRESULT hr=pD3D9->CreateDevice//创建设备对象 
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
	return true;
	return true;
}
