/**************************************************************************
/*		FileName: main.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/15 22:59:03
/*		CopyRight: yc 
/*************************************************************************/

#include "XCPch.h"
#include "XC_Client.h"

HWND render_hwnd = NULL;

//bool 

int WINAPI WinMain(  HINSTANCE hInstance,  HINSTANCE hPrevInstance,  LPSTR lpCmdLine,  int nShowCmd )
{
	
	XInitContext init_conext;
	init_conext.hwnd = render_hwnd;
	XShell::GetInstance().Init(&init_conext);

	MSG msg = {0};
	while(WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, NULL, 0, 0, 0))
		{

		}
		else
		{
			XShell::GetInstance().Frame();
		}
	}
	return nShowCmd;
}