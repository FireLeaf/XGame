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
	XD3D9Texture2D() : XTexture2D(), m_pD3D92DTexture(NULL){}
	IDirect3DTexture9* GetD3D9Texture(){return m_pD3D92DTexture;}
public:
	virtual void UpdateAsset(XAssetMonitor* pMonitor);
	virtual void ReleaseAsset(XAssetMonitor* pMonitor);
protected:
	IDirect3DTexture9* m_pD3D92DTexture;
};

class XD3D9TextureCube : public XTextureCube
{
public:
	XD3D9TextureCube() : XTextureCube(), m_pD3D9CubeTexture(NULL){}
public:
	virtual void ReleaseAsset(XAssetMonitor* pMonitor);
protected:
	IDirect3DCubeTexture9* m_pD3D9CubeTexture;
};

class XD3D9Texture3D : public XTexture3D
{

};

class XD3DRenderTarget : public XRenderTarget
{
public:
	virtual void ReleaseAsset(XAssetMonitor* pMonitor);
protected:
	IDirect3DTexture9* m_pD3D9RenderTarget;
	IDirect3DSurface9* m_pD3D9DepthSurface;
};

class XD3D9VertexPool : public XVertexPool
{
public:
	XD3D9VertexPool() : XVertexPool(), m_pD3D9VertexBuffer(NULL) {}
public:
	virtual void ReleaseAsset(XAssetMonitor* pMonitor);
protected:
	IDirect3DVertexBuffer9* m_pD3D9VertexBuffer;
};

class XD3D9IndexPool : public XIndexPool
{
public:
	XD3D9IndexPool() : XIndexPool(), m_pD3D9IndexBuffer(NULL){}
protected:
	IDirect3DIndexBuffer9* m_pD3D9IndexBuffer;
};

// typedef struct D3DVERTEXELEMENT9 {
// 	WORD Stream;
// 	WORD Offset;
// 	BYTE Type;//D3DDECLTYPE
// 	BYTE Method;//D3DDECLMETHOD
// 	BYTE Usage;//D3DDECLUSAGE
// 	BYTE UsageIndex;//COLOR0 ---> 0, COLOR1 --->1
// } D3DVERTEXELEMENT9, *LPD3DVERTEXELEMENT9;
// 
// typedef enum D3DDECLTYPE {
// 	D3DDECLTYPE_FLOAT1      = 0,
// 	D3DDECLTYPE_FLOAT2      = 1,
// 	D3DDECLTYPE_FLOAT3      = 2,
// 	D3DDECLTYPE_FLOAT4      = 3,
// 	D3DDECLTYPE_D3DCOLOR    = 4,
// 	D3DDECLTYPE_UBYTE4      = 5,
// 	D3DDECLTYPE_SHORT2      = 6,
// 	D3DDECLTYPE_SHORT4      = 7,
// 	D3DDECLTYPE_UBYTE4N     = 8,
// 	D3DDECLTYPE_SHORT2N     = 9,
// 	D3DDECLTYPE_SHORT4N     = 10,
// 	D3DDECLTYPE_USHORT2N    = 11,
// 	D3DDECLTYPE_USHORT4N    = 12,
// 	D3DDECLTYPE_UDEC3       = 13,
// 	D3DDECLTYPE_DEC3N       = 14,
// 	D3DDECLTYPE_FLOAT16_2   = 15,
// 	D3DDECLTYPE_FLOAT16_4   = 16,
// 	D3DDECLTYPE_UNUSED      = 17 
// } D3DDECLTYPE, *LPD3DDECLTYPE;
// 
// typedef enum D3DDECLMETHOD {
// 	D3DDECLMETHOD_DEFAULT            = 0,
// 	D3DDECLMETHOD_PARTIALU           = 1,
// 	D3DDECLMETHOD_PARTIALV           = 2,
// 	D3DDECLMETHOD_CROSSUV            = 3,
// 	D3DDECLMETHOD_UV                 = 4,
// 	D3DDECLMETHOD_LOOKUP             = 5,
// 	D3DDECLMETHOD_LOOKUPPRESAMPLED   = 6 
// } D3DDECLMETHOD, *LPD3DDECLMETHOD;
// 
// typedef enum D3DDECLUSAGE {
// 	D3DDECLUSAGE_POSITION       = 0,
// 	D3DDECLUSAGE_BLENDWEIGHT    = 1,
// 	D3DDECLUSAGE_BLENDINDICES   = 2,
// 	D3DDECLUSAGE_NORMAL         = 3,
// 	D3DDECLUSAGE_PSIZE          = 4,
// 	D3DDECLUSAGE_TEXCOORD       = 5,
// 	D3DDECLUSAGE_TANGENT        = 6,
// 	D3DDECLUSAGE_BINORMAL       = 7,
// 	D3DDECLUSAGE_TESSFACTOR     = 8,
// 	D3DDECLUSAGE_POSITIONT      = 9,
// 	D3DDECLUSAGE_COLOR          = 10,
// 	D3DDECLUSAGE_FOG            = 11,
// 	D3DDECLUSAGE_DEPTH          = 12,
// 	D3DDECLUSAGE_SAMPLE         = 13 
// } D3DDECLUSAGE, *LPD3DDECLUSAGE;


class XD3D9VertexAttribute : public XVertexAttribute
{
public:
public:
	XD3D9VertexAttribute() : XVertexAttribute(), m_pD3D9VertexDeclaration(NULL){}
	IDirect3DVertexDeclaration9* GetD3D9VertexDecl(){return m_pD3D9VertexDeclaration;}
protected:
	IDirect3DVertexDeclaration9* m_pD3D9VertexDeclaration;
};

#endif // XD3D9Asset