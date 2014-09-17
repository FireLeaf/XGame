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
#define Msg(a) MessageBox(NULL,a,"��ʾ",MB_OK)
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

	g_Log.WriteSeg("��ʼ��");
	if(!InitWindows()){
		return false;
	}
	g_Log.WriteRight("���ڳ�ʼ���ɹ�");

	if(!Init3DDevice(windowed)){
		return false;
	}
	g_Log.WriteRight("D3D�豸��ʼ���ɹ�");

	
	//������������������
	
	g_Log.WriteRight("��ʼ���ɹ�");
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
		Msg("ע�ᴰ����ʧ��");
		return false;
	}

	//��������
	RECT rc;
	SetRect(&rc,0,0,WIDTH,HEIGHT);
	AdjustWindowRect(&rc,WS_OVERLAPPEDWINDOW,FALSE);
	m_hWnd=CreateWindow(WNDCLASSNAME,WNDCAPTION,WS_THICKFRAME|WS_CAPTION|WS_MINIMIZEBOX|WS_OVERLAPPED|WS_SYSMENU/*WS_OVERLAPPEDWINDOW*/,0,0,(rc.right-rc.left),(rc.bottom-rc.top),0,NULL,m_hInstance,0);
	if(!m_hWnd)
	{
		Msg("��������ʧ��");
		return false;
	}

	ShowWindow(m_hWnd,SW_SHOW);
	UpdateWindow(m_hWnd);


	return true;

}

bool CGame::Init3DDevice(bool windowed){
	IDirect3D9 *pD3D9=Direct3DCreate9(D3D_SDK_VERSION);
	if(!pD3D9){
		Msg("��ȡIDirect3D9�ӿ�ʧ��");
		return false;
	}

	int vp=0;
	D3DCAPS9 caps;

	//�鿴�����Ƿ�֧��Ӳ�����㴦��
	pD3D9->GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&caps);
	if(caps.DevCaps&D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp=D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp=D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	//����D3D����
	m_D3Dpp.AutoDepthStencilFormat=D3DFMT_D24S8;//��̨�洢��ʽ
	m_D3Dpp.BackBufferCount=1;//��̨����
	m_D3Dpp.BackBufferFormat=D3DFMT_A8R8G8B8;//��̨���ظ�ʽ
	m_D3Dpp.BackBufferHeight=HEIGHT;//��̨�߶�
	m_D3Dpp.BackBufferWidth=WIDTH;//��̨���
	m_D3Dpp.EnableAutoDepthStencil=true;//������̨����
	m_D3Dpp.Flags=0;//�����־
	m_D3Dpp.FullScreen_RefreshRateInHz=D3DPRESENT_RATE_DEFAULT;//ˢ����
	m_D3Dpp.hDeviceWindow=m_hWnd;//�������ھ��
	m_D3Dpp.MultiSampleQuality=0;//��������
	m_D3Dpp.MultiSampleType=D3DMULTISAMPLE_NONE;//�������
	m_D3Dpp.PresentationInterval=D3DPRESENT_INTERVAL_IMMEDIATE;//�ύģʽ
	m_D3Dpp.SwapEffect=D3DSWAPEFFECT_DISCARD;//����Ч��
	m_D3Dpp.Windowed=windowed;//�Ƿ񴰿ڻ�

	HRESULT hr=pD3D9->CreateDevice//�����豸���� 
		(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,m_hWnd,vp,
		&m_D3Dpp,&m_pDevice
		);
	if(FAILED(hr))
	{
		Msg("�����豸����ʧ��");
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
	
	static float lastTime=(float)timeGetTime();//��һ�ε�ʱ��ֵ
	float currentTime;//��ǰʱ��ֵ��ֻ������Ⱦ�ڼ䣬������Ϣ����
	float timeDelta;
	while (WM_QUIT!=msg.message)
	{
		if(PeekMessage(&msg,0,0,0,PM_REMOVE))
		{//����ϵͳ��Ϣ
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}else
		{
			currentTime=(float)timeGetTime();
			timeDelta=(currentTime-lastTime)*0.001f;
			
			//������¼���ʾ��Ϸ
			Update(timeDelta);
			Show(timeDelta);

			lastTime=currentTime;//��ֵ��ǰʱ�䵽�ϴ�ʱ��ֵ
		}
	}
	
	return msg.wParam;
}

bool CGame::SetUp()
{
	g_Log.WriteSeg("���ó���");

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

	g_Log.WriteInfo("���óɹ�");
	return true;
}

bool CGame::LostDevice()
{
	try
	{
		m_isDeviceLost=true;
		//////////////////////////////////////////////////////////////////////////
		//�����豸��Ӧ�������������ز���

		g_pDrawText->LostDevice();

		//////////////////////////////////////////////////////////////////////////
	}
	catch (...){
		g_Log.WriteError("�豸��ʧʱ������ʧ��");
	}
	return true;
}

bool CGame::GenDevice(){
	try
	{
		m_pDevice->Reset(&m_D3Dpp);

		m_isDeviceLost=false;

		//////////////////////////////////////////////////////////////////////////
		//�����豸��Ӧ�����������
		g_pDrawText->GenDevice();

		//////////////////////////////////////////////////////////////////////////
	}
	catch (...){
		g_Log.WriteError("�豸��ȡʱ������ʧ��");
	}
	return true;
}

bool CGame::Update(float timeDelta)
{
	//����豸�����Ƿ�ʧ
	try{
		HRESULT hr=m_pDevice->TestCooperativeLevel();
		if(D3D_OK!=hr)
		{//�豸�쳣
			if(D3DERR_DEVICELOST==hr)//�豸��ʧ
			{
				if(m_isDeviceLost)
				{
					LostDevice();
				}
			}
			else if(D3DERR_DEVICENOTRESET)//�豸δ��λ
			{
				if(m_isDeviceLost==true)
					GenDevice();
			}

			Sleep(100);//��ͣ100���룬ʹϵͳ���㹻ʱ��ָ�

			return true;
		}
	}catch(...)
	{
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	//����д��Ҫ���µĴ��룬�Լ�Ҫ���Ķ���
	if(KeyDown('C'))
	{
		m_D3Dpp.Windowed=!m_D3Dpp.Windowed;
		LostDevice();
		GenDevice();
		g_Log.WriteInfo("�л���Ļ");

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

bool CGame::Show(float timeDelta)//��Ⱦ����
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
	g_Log.WriteInfo("������β");
	return true;
}


bool StardRender(IDirect3DDevice9 *pDevice,float timeDelta)
{

	return true;
}

CGame::~CGame()
{

}