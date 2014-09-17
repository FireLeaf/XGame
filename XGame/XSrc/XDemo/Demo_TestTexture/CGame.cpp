#include"CGame.h"
#include <windows.h>
#include <d3dx9.h>
#include <d3d9.h>
#include"clog.h"
#include "CDrawText.h"

CGame *g_pGame=NULL;
#define WIDTH 800
#define HEIGHT 600
#define WNDCLASSNAME "GameRefer"
#define WNDCAPTION "www.GameRefer.com"
#define Msg(a) MessageBox(NULL,a,"提示",MB_OK)
#define ReleaseCom(a) if(a){  a->Release(); a=NULL;}
#define KeyDown(a) GetAsyncKeyState(a)&0x8000

CLog g_Log;
CDrawText *g_pDrawText=NULL;

CGame::CGame()
{
	m_hInstance=NULL;
	m_hWnd=NULL;
	m_pDevice=NULL;
	m_isDeviceLost=false;
	memset(&m_D3Dpp,0,sizeof(D3DPRESENT_PARAMETERS));
}

LRESULT CALLBACK WinProc(
							HWND hwnd,      // handle to window
							UINT uMsg,      // message identifier
							WPARAM wParam,  // first message parameter
							LPARAM lParam   // second message parameter
							)
{
	return g_pGame->WindowProc(hwnd,uMsg,wParam,lParam);
}

LRESULT CALLBACK CGame::WindowProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
	switch(uMsg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_CREATE:
			break;
		case WM_KEYDOWN:
			if(VK_ESCAPE==wParam){
				DestroyWindow(hwnd);
				break;
			}
			break;
	
	}

	return DefWindowProc(hwnd,uMsg,wParam,lParam);
}

bool CGame::Init(HINSTANCE hInstance,bool windowed)
{
	g_pGame=this;
	m_hInstance=hInstance;

	g_Log.WriteSeg("初始化");
	if(!InitWindows()){
		return false;
	}
	g_Log.WriteRight("窗口初始化成功");

	if(!Init3DDevice(windowed)){
		return false;
	}
	g_Log.WriteRight("D3D设备初始化成功");

	
	//这里可以添加其他代码
	
	g_Log.WriteRight("初始化成功");
	return true;
}

bool CGame::InitWindows()
{
	WNDCLASS wd;
	ZeroMemory(&wd,sizeof(WNDCLASS));
	wd.cbClsExtra=0;
	wd.cbWndExtra=0;
	wd.hInstance=m_hInstance;
	wd.hbrBackground=(HBRUSH)GetStockObject(BLACK_BRUSH);
	wd.hIcon=LoadIcon(NULL,IDI_APPLICATION);
	wd.hCursor=LoadCursor(NULL,IDC_ARROW);
	wd.lpfnWndProc=(WNDPROC)WinProc;
	wd.lpszClassName=WNDCLASSNAME;
	wd.lpszMenuName=NULL;
	wd.style=CS_HREDRAW|CS_VREDRAW;

	if(RegisterClass(&wd)==0)
	{
		Msg("注册窗口类失败");
		return false;
	}

	//调整窗口
	RECT rc;
	SetRect(&rc,0,0,WIDTH,HEIGHT);
	AdjustWindowRect(&rc,WS_OVERLAPPEDWINDOW,FALSE);
	m_hWnd=CreateWindow(WNDCLASSNAME,WNDCAPTION,WS_THICKFRAME|WS_CAPTION|WS_MINIMIZEBOX|WS_OVERLAPPED|WS_SYSMENU/*WS_OVERLAPPEDWINDOW*/,0,0,(rc.right-rc.left),(rc.bottom-rc.top),0,NULL,m_hInstance,0);
	if(!m_hWnd)
	{
		Msg("创建窗口失败");
		return false;
	}

	ShowWindow(m_hWnd,SW_SHOW);
	UpdateWindow(m_hWnd);


	return true;

}

bool CGame::Init3DDevice(bool windowed){
	IDirect3D9 *pD3D9=Direct3DCreate9(D3D_SDK_VERSION);
	if(!pD3D9){
		Msg("获取IDirect3D9接口失败");
		return false;
	}

	int vp=0;
	D3DCAPS9 caps;

	//查看窗口是否支持硬件顶点处理
	pD3D9->GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&caps);
	if(caps.DevCaps&D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp=D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp=D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	//设置D3D属性
	m_D3Dpp.AutoDepthStencilFormat=D3DFMT_D24S8;//后台存储格式
	m_D3Dpp.BackBufferCount=1;//后台个数
	m_D3Dpp.BackBufferFormat=D3DFMT_A8R8G8B8;//后台像素格式
	m_D3Dpp.BackBufferHeight=HEIGHT;//后台高度
	m_D3Dpp.BackBufferWidth=WIDTH;//后台宽度
	m_D3Dpp.EnableAutoDepthStencil=true;//启动后台缓存
	m_D3Dpp.Flags=0;//特殊标志
	m_D3Dpp.FullScreen_RefreshRateInHz=D3DPRESENT_RATE_DEFAULT;//刷新率
	m_D3Dpp.hDeviceWindow=m_hWnd;//关联窗口句柄
	m_D3Dpp.MultiSampleQuality=0;//采样质量
	m_D3Dpp.MultiSampleType=D3DMULTISAMPLE_NONE;//采样类别
	m_D3Dpp.PresentationInterval=D3DPRESENT_INTERVAL_IMMEDIATE;//提交模式
	m_D3Dpp.SwapEffect=D3DSWAPEFFECT_DISCARD;//交换效率
	m_D3Dpp.Windowed=windowed;//是否窗口化

	HRESULT hr=pD3D9->CreateDevice//创建设备对象 
		(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,m_hWnd,vp,
		&m_D3Dpp,&m_pDevice
		);
	if(FAILED(hr))
	{
		Msg("创建设备对象失败");
		ReleaseCom(pD3D9);
		return false;

	}
	
	ReleaseCom(pD3D9);
	return true;

}



int CGame::MesLoop()
{
	MSG msg;
	ZeroMemory(&msg,sizeof(msg));
	
	static float lastTime=(float)timeGetTime();//上一次的时间值
	float currentTime;//当前时间值，只用于渲染期间，不计消息处理
	float timeDelta;
	while (WM_QUIT!=msg.message)
	{
		if(PeekMessage(&msg,0,0,0,PM_REMOVE))
		{//处理系统消息
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}else
		{
			currentTime=(float)timeGetTime();
			timeDelta=(currentTime-lastTime)*0.001f;
			
			//这里更新及显示游戏
			Update(timeDelta);
			Show(timeDelta);

			lastTime=currentTime;//赋值当前时间到上次时间值
		}
	}
	
	return msg.wParam;
}

bool CGame::SetUp()
{
	g_Log.WriteSeg("配置程序");

	g_pDrawText=new CDrawText(m_pDevice);
	IDirect3DTexture9* pTexture = NULL;
	m_pDevice->CreateTexture(512, 512, 3, 0, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &pTexture, NULL);
	D3DSURFACE_DESC desc;
	pTexture->GetLevelDesc(0, &desc);512, 512
	pTexture->GetLevelDesc(1, &desc);256, 256
	pTexture->GetLevelDesc(2, &desc);

	m_pDevice->CreateTexture(216, 216, 3, 0, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &pTexture, NULL);
	pTexture->GetLevelDesc(0, &desc);
	pTexture->GetLevelDesc(1, &desc);
	pTexture->GetLevelDesc(2, &desc);

	m_pDevice->CreateTexture(1024, 1024, 3, 0, D3DFMT_X8R8G8B8, D3DPOOL_MANAGED, &pTexture, NULL);
	pTexture->GetLevelDesc(0, &desc);
	pTexture->GetLevelDesc(1, &desc);
	pTexture->GetLevelDesc(2, &desc);

	g_Log.WriteInfo("配置成功");
	return true;
}

bool CGame::LostDevice()
{
	try
	{
		m_isDeviceLost=true;
		//////////////////////////////////////////////////////////////////////////
		//所有设备都应该在这里进行相关操作

		g_pDrawText->LostDevice();

		//////////////////////////////////////////////////////////////////////////
	}
	catch (...){
		g_Log.WriteError("设备丢失时产生了失败");
	}
	return true;
}

bool CGame::GenDevice(){
	try
	{
		m_pDevice->Reset(&m_D3Dpp);

		m_isDeviceLost=false;

		//////////////////////////////////////////////////////////////////////////
		//所有设备都应该在这里操作
		g_pDrawText->GenDevice();

		//////////////////////////////////////////////////////////////////////////
	}
	catch (...){
		g_Log.WriteError("设备获取时产生了失败");
	}
	return true;
}

bool CGame::Update(float timeDelta)
{
	//检测设备对象是否丢失
	try{
		HRESULT hr=m_pDevice->TestCooperativeLevel();
		if(D3D_OK!=hr)
		{//设备异常
			if(D3DERR_DEVICELOST==hr)//设备丢失
			{
				if(m_isDeviceLost)
				{
					LostDevice();
				}
			}
			else if(D3DERR_DEVICENOTRESET)//设备未复位
			{
				if(m_isDeviceLost==true)
					GenDevice();
			}

			Sleep(100);//暂停100毫秒，使系统有足够时间恢复

			return true;
		}
	}catch(...)
	{
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	//下面写入要更新的代码，以及要检测的东西
	if(KeyDown('C'))
	{
		m_D3Dpp.Windowed=!m_D3Dpp.Windowed;
		LostDevice();
		GenDevice();
		g_Log.WriteInfo("切换屏幕");

		if(m_D3Dpp.Windowed)
		{
			RECT rc = {0, 0, WIDTH,HEIGHT};
			AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);
			SetWindowPos(m_hWnd, HWND_NOTOPMOST, 0, 0, rc.right - rc.left, rc.bottom - rc.top, SWP_SHOWWINDOW);
			UpdateWindow(m_hWnd);
		}
	}
	
	return true;
}

bool CGame::Show(float timeDelta)//渲染世界
{

	if(!m_isDeviceLost)
	{
		
		m_pDevice->Clear(0,NULL,D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,0xff0000ff,1.0f,0);
		m_pDevice->BeginScene();

		g_pDrawText->FPS(timeDelta);

		m_pDevice->EndScene();
		m_pDevice->Present(NULL,NULL,0,NULL);
		return true;

	}
	
	return false;
}



bool CGame::Clean(){

	ReleaseCom(m_pDevice);
	m_hWnd=NULL;
	m_hInstance=NULL;
	g_Log.WriteInfo("程序收尾");
	return true;
}


bool StardRender(IDirect3DDevice9 *pDevice,float timeDelta)
{

	return true;
}

CGame::~CGame()
{

}