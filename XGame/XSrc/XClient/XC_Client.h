/**************************************************************************
/*		FileName: XC_Client.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/15 22:47:32
/*		CopyRight: yc 
/*************************************************************************/

#include "XClient.h"
class CXCClient : public XClient
{
public:
	virtual xbool Init();
	virtual void Tick();
	virtual void Render();
};