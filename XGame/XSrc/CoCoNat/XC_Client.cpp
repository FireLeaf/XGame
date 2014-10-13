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

void CXCClient::Render(XRenderScene* ptr_render_scene)
{
	XVector3 eye(5.0f, 5.0f, -20.0f), up(0.0f, 1.0f, 0.0f), at(0.0f, 0.0f, 0.0f);
	XCamera& camera = ptr_render_scene->GetSceneDesc()->camera;
	XViewPort& viwport = ptr_render_scene->GetSceneDesc()->view_port;

	camera.SetEye(eye);
	camera.SetUp(up);
	camera.SetAt(at);

	viwport.fov = X_PI * 0.25f;
	viwport.width = 1024;
	viwport.height = 768;
	viwport.z_near = 0.1f;
	viwport.z_far = 1000.0f;
	if (pMesh)
	{
		ptr_render_scene->AddToRenderList(pMesh);
	}
}