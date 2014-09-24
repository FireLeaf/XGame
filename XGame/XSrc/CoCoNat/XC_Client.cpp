/**************************************************************************
/*		FileName: XC_Client.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/15 23:03:45
/*		CopyRight: yc 
/*************************************************************************/

#include "NatPch.h"
#include "XC_Client.h"

CXCClient client;
XClient* x_ptr_client = &client;

CXCClient::CXCClient()
{
	pMesh = NULL;
}

CXCClient::~CXCClient()
{
	if (pMesh)
	{
		delete pMesh;
		pMesh = NULL;
	}
}

xbool CXCClient::Init()
{
	pMesh = new XMesh;
	if(!pMesh->LoadMesh("AssetBundle\\model\\test2.AX"))
	{
		return false;
	}
	return true;
}

void CXCClient::Tick()
{
	
}

void CXCClient::Render(XRenderMonitor* ptr_render_monitor)
{
	if (pMesh)
	{
		ptr_render_monitor->AddToRenderList(pMesh);
	}
}