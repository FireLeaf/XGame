/**************************************************************************
/*		FileName: XAsset.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/07/29 19/57/00
/*		CopyRight: yc 
/*************************************************************************/
#ifndef __XASSET__H
#define __XASSET__H

class XAssetData//��Դ���ص�����
{
public:
	void SetAssetDataID(xuint32 id){uAssetDataID = id;}
	xuint32 GetAssetDataID(){return uAssetDataID;}
protected:
	void* pData;//���ݣ� �������ݣ�index����,�����
	xuint32 uDataLength;//���ݵĳ���
	xuint32 uAssetDataID;//���ݵ�ΨһID
};

class XAsset
{
public:
	enum ENUM_ASSET_TYPE
	{
		ASSET_VERTEX_POOL,//�����
		ASSET_INDEX_POOL,//����������
		ASSET_TEXTURE_2D,//2d����
		ASSET_TEXTURE_CUBE,//����������
		ASSET_TEXTURE_3D,//�������
		ASSET_TEXTURE_RENDER,//Render Target
		ASSET_TEXTURE_DEPTH,//Depth Stencil
	};
public:
	XAsset(ENUM_ASSET_TYPE assetType) : m_emAssetType(assetType){}
	virtual ~XAsset(){}
public:
	ENUM_ASSET_TYPE GetAssetType(){return m_emAssetType;}
	virtual void UpdateAsset(XAssetMonitor* pMonitor) {pMonitor->UpdateAsset(this);}
public:
	ENUM_ASSET_TYPE m_emAssetType;
};

class XVertexPool : public XAsset
{
public:
	XVertexPool() : XAsset(ASSET_VERTEX_POOL){}
};

class XIndexPool : public XAsset
{
public:
	XIndexPool() : XAsset(ASSET_INDEX_POOL){}
};

class XTexture : public XAsset
{
public:
	XTexture(ENUM_ASSET_TYPE assetType) : XAsset(assetType){}
};

class XTexture2D : public XTexture
{
public:
	XTexture2D() : XTexture(ASSET_TEXTURE_2D){}
};

class XTextureCube : public XTexture
{
public:
	XTextureCube() : XTexture(ASSET_TEXTURE_CUBE){}
};

class XTexture3D : public XTexture
{
public:
	XTexture3D() : XTexture(ASSET_TEXTURE_3D){}
};

class XRenderTarget : public XTexture
{
public:
	XRenderTarget() : XTexture(ASSET_TEXTURE_RENDER){}
};

class XDepthStencil : public XTexture
{
public:
	XIndexPool() : XTexture(ASSET_TEXTURE_DEPTH){}
};

#endif // XAsset