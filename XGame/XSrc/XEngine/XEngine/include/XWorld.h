/**************************************************************************
/*		FileName: XWorld.h
/*		FileDesc:  ¿ΩÁ
/*		FileAuthor: YangCao
/*		CreateTime: 2014/10/06 22:37:01
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XWORLD__H
#define __XWORLD__H

class XRenderEntity;

class XWorld : public XRenderEntity
{
public:
	XWorld();
	virtual ~XWorld();
public:
	virtual bool Init(const char* world_path);
	virtual void Release();
	virtual void Tick(xint32 time_delta);
	virtual void Render();
protected:
	
};

#endif // XWorld