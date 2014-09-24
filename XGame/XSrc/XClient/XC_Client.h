/**************************************************************************
/*		FileName: XC_Client.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/15 22:47:32
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XC_CLIENT__H
#define __XC_CLIENT__H
#include "XClient.h"
class CXCClient : public XClient
{
public:
	CXCClient();
	~CXCClient();
public:
	virtual xbool Init();
	virtual void Tick();
	virtual void Render();
};
#endif // XC_Client

