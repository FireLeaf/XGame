/**************************************************************************
/*		FileName: XD3D9Asset.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/07/29 20/15/00
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XD3D9ASSET__H
#define __XD3D9ASSET__H

class XD3D9Texture2D : public XTexture2D
{
public:
	XD3D9Texture2D();
	IDirect3DTexture9* GetD3D9Texture(){return m_pD3D9Texture;}
protected:
	IDirect3DTexture9* m_pD3D9Texture;
};

class XD3D9VertexPool : public XVertexPool
{
	
};


#endif // XD3D9Asset