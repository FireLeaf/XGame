/**************************************************************************
/*		FileName: XD3D9Asset.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/07/29 20/15/00
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XD3D9ASSET__H
#define __XD3D9ASSET__H

extern IDirect3DDevice9* x_ptr_d3ddevice;

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
	IDirect3DCubeTexture9* GetD3D9CubeTexture(){return m_pD3D9CubeTexture;}
public:
	virtual void ReleaseAsset(XAssetMonitor* pMonitor);
	virtual void UpdateAsset(XAssetMonitor* pMonitor);
protected:
	IDirect3DCubeTexture9* m_pD3D9CubeTexture;
};

class XD3D9Texture3D : public XTexture3D
{

};

class XD3D9RenderTarget : public XRenderTarget
{
public:
	XD3D9RenderTarget() : XRenderTarget(), m_pD3D9DepthSurface(NULL), m_pD3D9RenderTarget(NULL){}
	IDirect3DTexture9* GetD3D9RenderTarget() {return m_pD3D9RenderTarget;}
	IDirect3DSurface9* GetD3D9DepthSurface() {return m_pD3D9DepthSurface;}
public:
	virtual void ReleaseAsset(XAssetMonitor* pMonitor);
	virtual void UpdateAsset(XAssetMonitor* pMonitor);
protected:
	IDirect3DTexture9* m_pD3D9RenderTarget;
	IDirect3DSurface9* m_pD3D9DepthSurface;
};

class XD3D9VertexPool : public XVertexPool
{
public:
	XD3D9VertexPool() : XVertexPool(), m_pD3D9VertexBuffer(NULL) {}
	IDirect3DVertexBuffer9* GetD3D9VertexBuffer(){return m_pD3D9VertexBuffer;}
public:
	virtual void ReleaseAsset(XAssetMonitor* pMonitor);
	virtual void UpdateAsset(XAssetMonitor* pMonitor);
protected:
	IDirect3DVertexBuffer9* m_pD3D9VertexBuffer;
};

class XD3D9IndexPool : public XIndexPool
{
public:
	XD3D9IndexPool() : XIndexPool(), m_pD3D9IndexBuffer(NULL){}
	IDirect3DIndexBuffer9* GetD3D9IndexBuffer(){return m_pD3D9IndexBuffer;}
public:
	virtual void ReleaseAsset(XAssetMonitor* pMonitor);
	virtual void UpdateAsset(XAssetMonitor* pMonitor);
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
	XD3D9VertexAttribute() : XVertexAttribute(), m_pD3D9VertexDeclaration(NULL){}
	IDirect3DVertexDeclaration9* GetD3D9VertexDecl(){return m_pD3D9VertexDeclaration;}
public:
	virtual void ReleaseAsset(XAssetMonitor* pMonitor);
	virtual void UpdateAsset(XAssetMonitor* pMonitor);
protected:
	IDirect3DVertexDeclaration9* m_pD3D9VertexDeclaration;
};

class XD3D9ShaderParamTable : public XShaderParamTable
{
public:
	XD3D9ShaderParamTable(){m_pD3DXConstantTable = NULL;}
	~XD3D9ShaderParamTable(){if(m_pD3DXConstantTable) m_pD3DXConstantTable->Release(); m_pD3DXConstantTable = NULL;}
public:
// 	void SetTexture(const char* name, XTexture* texture);
// 	void SetBool(const char* name, xbool val);
// 	void SetBoolArray(const char* name, xbool* vals, int count);
// 	void SetFloat(const char* name, float val);
// 	void SetFloatArray(const char* name, float* vals, int count);
// 	void SetInt(const char* name, int val);
// 	void SetIntArray(const char* name, int* vals, int count);
// 	void SetValue(const char* name, void* val, int size);

	virtual void SetValue(const char* name, XTexture* texture);
	virtual void SetValue(const char* name, const xbool val);
	virtual void SetValue(const char* name, const xbool* vals, const int count);
	virtual void SetValue(const char* name, const float val);
	virtual void SetValue(const char* name, const float* vals, const int count);
	virtual void SetValue(const char* name, const int val);
	virtual void SetValue(const char* name, const int* vals, const int count);
	virtual void SetValue(const char* name, const void* val, const int size);
public:
	void DumpConstTable()
	{
		D3DXCONSTANTTABLE_DESC table_desc;
		m_pD3DXConstantTable->GetDesc(&table_desc);
		for (int i = 0; i < table_desc.Constants; i++)
		{
			D3DXHANDLE handle = m_pD3DXConstantTable->GetConstant(NULL, i);
			if (handle)
			{
				xint32 count = 1;
				D3DXCONSTANT_DESC desc;
				if(FAILED(m_pD3DXConstantTable->GetConstantDesc(handle, &desc, (UINT*)&count)))
				{
					continue;
				}
				ShaderConstDesc scd;
				scd.handle = (void*)handle;
				scd.reg_index = desc.RegisterIndex;
			}
		}
	}

	ID3DXConstantTable* GetD3D9ConstantTable(){return m_pD3DXConstantTable;}
public:
	ID3DXConstantTable* m_pD3DXConstantTable;
	
};

class XD3D9VertexShader : public XVertexShader
{
public:
	XD3D9VertexShader() : XVertexShader(), m_pD3D9VertexShader(NULL){}
	IDirect3DVertexShader9* GetD3D9VertexShader(){return m_pD3D9VertexShader;}
public:
	virtual void ReleaseAsset(XAssetMonitor* pMonitor);
	virtual void UpdateAsset(XAssetMonitor* pMonitor);
public:
	IDirect3DVertexShader9* m_pD3D9VertexShader;
	//ID3DXConstantTable* m_pD3DXConstantTable;
};

class XD3D9PixelShader : public XPixelShader
{
public:
	XD3D9PixelShader() : XPixelShader(), m_pD3D9PixelShader(NULL){}
	IDirect3DPixelShader9* GetD3D9PixelShader(){return m_pD3D9PixelShader;}
public:
	virtual void ReleaseAsset(XAssetMonitor* pMonitor);
	virtual void UpdateAsset(XAssetMonitor* pMonitor);
public:
	IDirect3DPixelShader9* m_pD3D9PixelShader;
	//ID3DXConstantTable* m_pD3DXConstantTable;
};

#endif // XD3D9Asset