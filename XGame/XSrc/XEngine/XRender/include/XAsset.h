/**************************************************************************
/*		FileName: XAsset.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/07/29 19/57/00
/*		CopyRight: yc 
/*************************************************************************/
#ifndef __XASSET__H
#define __XASSET__H

#include "XType.h"
#include "XRefObject.h"

enum X_DECLTYPE
{
	X_DECLTYPE_FLOAT1    =  0,  // 1D float expanded to (value, 0., 0., 1.)
	X_DECLTYPE_FLOAT2    =  1,  // 2D float expanded to (value, value, 0., 1.)
	X_DECLTYPE_FLOAT3    =  2,  // 3D float expanded to (value, value, value, 1.)
	X_DECLTYPE_FLOAT4    =  3,  // 4D float
	X_DECLTYPE_COLOR  =  4,  // 4D packed unsigned bytes mapped to 0. to 1. range
	// Input is in X_COLOR format (ARGB) expanded to (R, G, B, A)
	X_DECLTYPE_UBYTE4    =  5,  // 4D unsigned byte
	X_DECLTYPE_SHORT2    =  6,  // 2D signed short expanded to (value, value, 0., 1.)
	X_DECLTYPE_SHORT4    =  7,  // 4D signed short

	// The following types are valid only with vertex shaders >= 2.0


	X_DECLTYPE_UBYTE4N   =  8,  // Each of 4 bytes is normalized by dividing to 255.0
	X_DECLTYPE_SHORT2N   =  9,  // 2D signed short normalized (v[0]/32767.0,v[1]/32767.0,0,1)
	X_DECLTYPE_SHORT4N   = 10,  // 4D signed short normalized (v[0]/32767.0,v[1]/32767.0,v[2]/32767.0,v[3]/32767.0)
	X_DECLTYPE_USHORT2N  = 11,  // 2D unsigned short normalized (v[0]/65535.0,v[1]/65535.0,0,1)
	X_DECLTYPE_USHORT4N  = 12,  // 4D unsigned short normalized (v[0]/65535.0,v[1]/65535.0,v[2]/65535.0,v[3]/65535.0)
	X_DECLTYPE_UDEC3     = 13,  // 3D unsigned 10 10 10 format expanded to (value, value, value, 1)
	X_DECLTYPE_DEC3N     = 14,  // 3D signed 10 10 10 format normalized and expanded to (v[0]/511.0, v[1]/511.0, v[2]/511.0, 1)
	X_DECLTYPE_FLOAT16_2 = 15,  // Two 16-bit floating point values, expanded to (value, value, 0, 1)
	X_DECLTYPE_FLOAT16_4 = 16,  // Four 16-bit floating point values
	X_DECLTYPE_UNUSED    = 17,  // When the type field in a decl is unused.

	X_DECLTYPE_FLOAT_VECTOR		= 50,  //һ��float����
	X_DECLTYPE_MATRIX3X3_VECTOR	= 51,	//һ��3*3�ľ�������
	X_DECLTYPE_MATRIX4X3_VECTOR	= 52,
	X_DECLTYPE_MATRIX3X4_VECTOR	= 53,
	X_DECLTYPE_MATRIX4X4_VECTOR	= 54,
} ;

enum X_DECLUSAGE
{
	X_DECLUSAGE_POSITION = 0,
	X_DECLUSAGE_BLENDWEIGHT,   // 1
	X_DECLUSAGE_BLENDINDICES,  // 2
	X_DECLUSAGE_NORMAL,        // 3
	X_DECLUSAGE_PSIZE,         // 4
	X_DECLUSAGE_TEXCOORD,      // 5
	X_DECLUSAGE_TANGENT,       // 6
	X_DECLUSAGE_BINORMAL,      // 7
	X_DECLUSAGE_TESSFACTOR,    // 8
	X_DECLUSAGE_POSITIONT,     // 9
	X_DECLUSAGE_COLOR,         // 10
	X_DECLUSAGE_FOG,           // 11
	X_DECLUSAGE_DEPTH,         // 12
	X_DECLUSAGE_SAMPLE,        // 13
}; 

enum ENUM_ASSET_TYPE
{
	ASSET_VERTEX_POOL,//�����
	ASSET_INDEX_POOL,//����������
	ASSET_VERTEX_ATTRIBUTE,//�������ԣ�D3D���Ƕ�������ԣ�opgles����vertex bind info��
	ASSET_TEXTURE_2D,//2d����
	ASSET_TEXTURE_CUBE,//����������
	ASSET_TEXTURE_3D,//�������
	ASSET_TEXTURE_RENDER,//Render Target
	ASSET_TEXTURE_DEPTH,//Depth Stencil
	ASSET_MTRL,//����
	ASSET_VERTEX_SHADER,//������ɫ��
	ASSET_PIXEL_SHADER,//������ɫ��
};

enum X_D3DCUBEMAP_FACES
{
	X_CUBEMAP_FACE_POSITIVE_X     = 0,
	X_CUBEMAP_FACE_NEGATIVE_X     = 1,
	X_CUBEMAP_FACE_POSITIVE_Y     = 2,
	X_CUBEMAP_FACE_NEGATIVE_Y     = 3,
	X_CUBEMAP_FACE_POSITIVE_Z     = 4,
	X_CUBEMAP_FACE_NEGATIVE_Z     = 5,
	X_CUBEMAP_FACE_NUM,
} ;

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

class XAssetMonitor;

class XAsset : public XRefObject//, public XAssetData
{
public:
	
public:
	XAsset(ENUM_ASSET_TYPE assetType) : m_emAssetType(assetType), uAssetDataID(0), m_bDirty(true), m_bDynamic(false){}
	virtual ~XAsset(){}
public:
	ENUM_ASSET_TYPE GetAssetType(){return m_emAssetType;}
	virtual void UpdateAsset(XAssetMonitor* pMonitor) {}//{pMonitor->UpdateAsset(this);}
	virtual void ReleaseAsset(XAssetMonitor* pMonitor){}//{pMonitor->ReleaseAsset(this);}
	void Dirty(){m_bDirty = true;}
public:
	ENUM_ASSET_TYPE m_emAssetType;
	xuint32 uAssetDataID;//���ݵ�ΨһID
	xbool m_bDirty;//������Ⱦ
	xbool m_bDynamic;//�Ƿ��Ƕ�̬��
};

struct XVertexPoolDesc 
{
	xbyte* buffer;
	xint32 count;
};

class XVertexPool : public XAsset
{
public:
	XVertexPool() : XAsset(ASSET_VERTEX_POOL){}
public:
	const XVertexPoolDesc& GetVertexPoolDesc(){return m_VertexPoolDesc;}
	void SetVertexPoolDesc(const XVertexPoolDesc& desc){m_VertexPoolDesc = desc;}
protected:
	XVertexPoolDesc m_VertexPoolDesc;
};

enum
{
	INDEX_16BITS,
	INDEX_32BITS,
};
struct XIndexPoolDesc 
{
	xbyte* buffer;
	xint32 count;
	xint32 type;
};
class XIndexPool : public XAsset
{
public:
	XIndexPool() : XAsset(ASSET_INDEX_POOL){}
public:
	const XIndexPoolDesc& GetIndexPoolDesc(){return m_IndexPoolDesc;}
	void SetIndexPoolDesc(const XIndexPoolDesc& desc){m_IndexPoolDesc = desc;}
protected:
	XIndexPoolDesc m_IndexPoolDesc;
};

enum
{
	XTEX_FORMAT_A8R8G8B8,
	XTEX_FORMAT_A16R16G16B16,
};

struct XTexFormatDesc
{
	xint32 width;
	xint32 height;
	xint32 length;
	//xint32 
	xint32 lod;
	xint32 tex_format;
};

class XTexture : public XAsset
{
public:
	enum
	{
		TEXTURE_LOAD_FROM_FILE,
		TEXTURE_PEOGRAM_GENERATER,
	};
public:
	XTexture(ENUM_ASSET_TYPE assetType) : XAsset(assetType){}
public:
	const XTexFormatDesc& GetFormatDesc(){return m_formatDesc;}
	const XStl::string& GetTextureFile(){return m_textureFile;}
	void SetTextureFile(const XStl::string& textureFile){m_textureFile = textureFile;}
	void SetTextureDesc(const XTexFormatDesc& fd){m_formatDesc = fd;}
protected:
	XTexFormatDesc m_formatDesc;
	XStl::string m_textureFile;
};

#define MAX_LEVEL_TEXTURE 3
//����ʹ������������һ��ʹ��ԭʼ����ߴ磬�ڶ���ʹ��1/2����������1/4
struct XTextureData
{
	struct  
	{
		int width;
		int height;
		xbyte** ptr_pixel;
	}level_data[MAX_LEVEL_TEXTURE];
	int generate_level;
};

class XTexture2D : public XTexture
{
public:
	XTexture2D() : XTexture(ASSET_TEXTURE_2D){}
protected:
	XTextureData pixelData;
};

class XTextureCube : public XTexture
{
public:
	XTextureCube() : XTexture(ASSET_TEXTURE_CUBE){}
protected:
	XTextureData pixelData[X_CUBEMAP_FACE_NUM];
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
	XDepthStencil() : XTexture(ASSET_TEXTURE_DEPTH){}
};

class XVertexAttribute : public XAsset
{
public:
	struct VertexElement 
	{
		struct Element 
		{
			X_DECLTYPE data_type;//��������
			int offset;//�����ڶ�����ƫ��
			int method_normalized;//...����
			X_DECLUSAGE usage;//��;������d3d����
			xint32 stream;//������ 
		};
		std::vector<Element> vecElement;
		xint32 stream_index;//��ǰ������������D3D
		xint32 asset_vertex_pool;//��ǰ�����
	};
public:
	XVertexAttribute() : XAsset(ASSET_VERTEX_ATTRIBUTE){}
public:
	void AddVertexElement(const VertexElement& ve){vecVertexElement.push_back(ve);}
protected:
	std::vector<VertexElement> vecVertexElement;
};

typedef XVertexAttribute::VertexElement::Element Vertex_Decl_Element;

struct ShaderMarco
{
	XStl::string name;
	XStl::string definition;
};

struct VertexShaderDesc
{
	XStl::string entry;
	std::string shader_src;
	XStl::vector<ShaderMarco> marcos;
};

class XVertexShader : public XAsset
{
public:
	XVertexShader() : XAsset(ASSET_VERTEX_SHADER){}
public:
	const VertexShaderDesc& GetVertexShaderDesc(){return m_VertexShaderDesc;}
	void SetVertexShaderDesc(const VertexShaderDesc& desc){m_VertexShaderDesc = desc;}
public:
	VertexShaderDesc m_VertexShaderDesc;
};

struct PixelShaderDesc
{
	XStl::string entry;
	XStl::string shader_src;
	XStl::vector<ShaderMarco> marcos;
};

class XPixelShader : public XAsset
{
public:
	XPixelShader() : XAsset(ASSET_PIXEL_SHADER){}
public:
	PixelShaderDesc m_VertexShaderDesc;
};

class XMtrl : public XAsset
{
public:
protected:
	xint32 vertexShader;
	xint32 pixelShader;
};

// template<typename V, typename I>
// void CreateGemotryDataAsset(XGeometryData<V, T>& gd)
// {
// 	
// }

template<typename V, typename I>
struct XGeometryData
{
public:
	void Release()
	{
		if (indices_pool)
		{
			indices_pool->Release();
			delete indices_pool;
			indices_pool = NULL;
		}

		for (int i = 0; i < vertex_pools.size(); i++)
		{
			if (vertex_pools[i])
			{
				vertex_pools[i]->Release();
				delete vertex_pools[i];
			}
		}
		vertex_pools.clear();
	}
public:
	XVertexAttribute* vertex_attribute;//�������㻺������ݣ��Լ���������
	XStl::vector<typename V*> vertex_pools;//�������ݣ����ڱ����ڱ��ص�����
	xint32 asset_vertex_decl;//

	typename I* indices_pool;//����������
	xint32 asset_indices_id;//����
};

struct ShaderParam 
{
	XStl::string param_name;//��������
	bool in_vertex_shader;//���ڶ���
	X_DECLTYPE type;
	struct
	{
		int reg_index;
		int count;
		union
		{
			float f;
			int i;
			float v[3];
			float mat[16];
			float* arr_f;
		};
	}Value;
	
};

struct XConfigData//�������ݣ�������Ⱦ״̬�����ʣ��ƹ⣬shader���Լ�shader��uniform�Ĳ�������
{
	//shader
	int asset_vertex_shader;
	int asset_pixel_shader;//gles�б�ʾfragment shader
	int asset_bind;//gles�б�ʾprogram
	XStlext::hash_map<XStl::string, ShaderParam> map_shader_param;
};

//��Ⱦ���������ݣ�
struct XContextData 
{
public:
	
	//��ȾĿ���Լ���Ȼ��棬�����0��ʾĬ��
	int asset_render_target;
	int asset_depth_target;
};

#endif // XAsset