/**************************************************************************
/*		FileName: XShell.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/15 22/03/46
/*		CopyRight: yc 
/*************************************************************************/

#include "XClient.h"
#include "XShell.h"


bool XShell::Init(const XInitContext* ptr_ini_context)
{
	if (!x_ptr_client)
	{
		return false;
	}
	return x_ptr_client->Init();
}

void XShell::Frame()
{
	if (x_ptr_client)
	{
		x_ptr_client->Tick();
		x_ptr_client->Render();
	}
}