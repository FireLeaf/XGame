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
XThreadPool* load_pool = NULL;
XThread* load_thread = NULL;

xint32 Test_Proc(XJobDesc* desc)
{
	int a = 0;
	while (a < 3)
	{
		a++;
		int b = a + 2 * a;
	}
	char buf[64] = "\0";
	sprintf(buf, "%d: job complete!\n", (int)(desc));
	OutputDebugStringA(buf);
	return a;
}

xint32 Test_Proc2(XJobDesc* desc)
{
	while (1)
	{
		int a = 0;
		a++;
		int b = a + 2 * a;
		break;
	}
	char buf[64] = "\0";
	sprintf(buf, "%d: job complete!\n", (int)(desc));
	OutputDebugStringA(buf);
	return 0;
}

CXCClient::CXCClient()
{
	pMesh = NULL;
	//load_pool = XSys::XCreateThreadPool(2);
}

CXCClient::~CXCClient()
{
	if (pMesh)
	{
		delete pMesh;
		pMesh = NULL;
	}
	if (load_pool)
	{
		load_pool->ReleaseThreadPool();
	}
}

xbool CXCClient::Init()
{
//	pMesh = new XMesh;
// 	if(!pMesh->LoadMesh("AssetBundle\\model\\test2.AX"))
// 	{
// 		return false;
// 	}

	load_pool = XSys::XCreateThreadPool(2);
	XJob job;
	job.job_proc = Test_Proc;
	job.desc = (XJobDesc*)(1234567);
	load_pool->DoJob(job);

	job.job_proc = Test_Proc2;
	job.desc = (XJobDesc*)(2345678);
	load_pool->DoJob(job);

//	load_thread = XSys::XCreateThread(Test_Proc, (XJobDesc*)(98765));
	return true;
}

void CXCClient::Tick()
{
	static int time_acc = 0;
	time_acc++;
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