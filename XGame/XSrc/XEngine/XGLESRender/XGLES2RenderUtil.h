/**************************************************************************
/*		FileName: XRenderUtil.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/16 22:31:59
/*		CopyRight: yc 
/*************************************************************************/

#ifdef _ANDROID_PLATFORM
#include "XRenderUtil.h"
#include "XAsset.h"
#include "XRenderScene.h"
#include "XRII.h"

#ifndef __XD3D9RENDERUTIL__H
#define __XD3D9RENDERUTIL__H

#define CASE_RETURN(a, b) case a: return b;break;

namespace RenderUtil
{
	int GetTexFormat(int format)
	{
		switch(format)
		{
		case XTEX_FORMAT_A8R8G8B8:
			return D3DFMT_X8R8G8B8;
			break;
		case XTEX_FORMAT_A16R16G16B16:
			break;
		default:
			break;
		}

		return D3DFMT_UNKNOWN;
	}

	int GetTexStride(int format)
	{
		switch(format)
		{
		case XTEX_FORMAT_A8R8G8B8:
			return 4;
			break;
		case XTEX_FORMAT_A16R16G16B16:
			break;
		default:
			break;
		}

		return 0;
	}

	int GetAssetManageType(bool bDynamic)
	{
		return bDynamic ? D3DPOOL_DEFAULT : D3DPOOL_MANAGED;
	}

	int GetIndexBits(int bits)
	{
		switch(bits)
		{
		case INDEX_16BITS:
			return D3DFMT_INDEX16;
			break;
		case INDEX_32BITS:
			return D3DFMT_INDEX32;
			break;
		}
		return 0;
	}

	int GetValueTypeSize(int declType)
	{
		switch(declType)
		{
		case X_DECLTYPE_FLOAT1: return 4; break;
		case X_DECLTYPE_FLOAT2: return 8;break;
		case X_DECLTYPE_FLOAT3: return 12;break;
		case X_DECLTYPE_FLOAT4: return 16;break;
		case X_DECLTYPE_COLOR: return 4;break;
			// Input is in X_COLOR format (ARGB) expanded to (R, G, B, A)
		case X_DECLTYPE_UBYTE4: return 4;break;
		case X_DECLTYPE_SHORT2: return 4;break;
		case X_DECLTYPE_SHORT4: return 8;break;
			// The following types are valid only with vertex shaders >= 2.0
		case X_DECLTYPE_UBYTE4N: return 4;break;
		case X_DECLTYPE_SHORT2N: return 4;break;
		case X_DECLTYPE_SHORT4N: return 4;break;
		case X_DECLTYPE_USHORT2N: return 4;break;
		case X_DECLTYPE_USHORT4N: return 4;break;
		case X_DECLTYPE_UDEC3: return 4;break;
		case X_DECLTYPE_DEC3N: return 4;break;
		case X_DECLTYPE_FLOAT16_2: return 4;break;
		case X_DECLTYPE_FLOAT16_4: return 4;break;
		}
		return 0;
	}

	int GetDeclType(int declType)
	{
		switch(declType)
		{
		case X_DECLTYPE_FLOAT1: return D3DDECLTYPE_FLOAT1; break;
		case X_DECLTYPE_FLOAT2: return D3DDECLTYPE_FLOAT2;break;
		case X_DECLTYPE_FLOAT3: return D3DDECLTYPE_FLOAT3;break;
		case X_DECLTYPE_FLOAT4: return D3DDECLTYPE_FLOAT4;break;
		case X_DECLTYPE_COLOR: return D3DDECLTYPE_D3DCOLOR;break;
			// Input is in X_COLOR format (ARGB) expanded to (R, G, B, A)
		case X_DECLTYPE_UBYTE4: return D3DDECLTYPE_UBYTE4;break;
		case X_DECLTYPE_SHORT2: return D3DDECLTYPE_SHORT2;break;
		case X_DECLTYPE_SHORT4: return D3DDECLTYPE_SHORT4;break;
			// The following types are valid only with vertex shaders >= 2.0
		case X_DECLTYPE_UBYTE4N: return D3DDECLTYPE_UBYTE4N;break;
		case X_DECLTYPE_SHORT2N: return D3DDECLTYPE_SHORT2N;break;
		case X_DECLTYPE_SHORT4N: return D3DDECLTYPE_SHORT4N;break;
		case X_DECLTYPE_USHORT2N: return D3DDECLTYPE_USHORT2N;break;
		case X_DECLTYPE_USHORT4N: return D3DDECLTYPE_USHORT4N;break;
		case X_DECLTYPE_UDEC3: return D3DDECLTYPE_UDEC3;break;
		case X_DECLTYPE_DEC3N: return D3DDECLTYPE_DEC3N;break;
		case X_DECLTYPE_FLOAT16_2: return D3DDECLTYPE_FLOAT16_2;break;
		case X_DECLTYPE_FLOAT16_4: return D3DDECLTYPE_FLOAT16_4;break;
		}

		return D3DDECLTYPE_UNUSED;
	}

	int GetDeclUsage(int declUsage)
	{
		switch(declUsage)
		{
		case X_DECLUSAGE_POSITION: return D3DDECLUSAGE_POSITION;break;
		case X_DECLUSAGE_BLENDWEIGHT: return D3DDECLUSAGE_BLENDWEIGHT;break;   // 1
		case X_DECLUSAGE_BLENDINDICES: return D3DDECLUSAGE_BLENDINDICES;break;  // 2
		case X_DECLUSAGE_NORMAL: return D3DDECLUSAGE_NORMAL;break;        // 3
		case X_DECLUSAGE_PSIZE: return D3DDECLUSAGE_PSIZE;break;         // 4
		case X_DECLUSAGE_TEXCOORD: return D3DDECLUSAGE_TEXCOORD;break;      // 5
		case X_DECLUSAGE_TANGENT: return D3DDECLUSAGE_TANGENT;break;       // 6
		case X_DECLUSAGE_BINORMAL: return D3DDECLUSAGE_BINORMAL;break;      // 7
		case X_DECLUSAGE_TESSFACTOR: return D3DDECLUSAGE_TESSFACTOR;break;    // 8
		case X_DECLUSAGE_POSITIONT: return D3DDECLUSAGE_POSITIONT;break;     // 9
		case X_DECLUSAGE_COLOR: return D3DDECLUSAGE_COLOR;break;         // 10
		case X_DECLUSAGE_FOG: return D3DDECLUSAGE_FOG;break;           // 11
		case X_DECLUSAGE_DEPTH: return D3DDECLUSAGE_DEPTH;break;         // 12
		case X_DECLUSAGE_SAMPLE: return D3DDECLUSAGE_SAMPLE;break;        // 13
		}

		return -1;
	}

	int GetPrimitiveType(int primtiveType)
	{
		switch(primtiveType)
		{
			CASE_RETURN(X_PT_POINTLIST, D3DPT_POINTLIST)
				CASE_RETURN(X_PT_LINELIST, D3DPT_LINELIST)
				CASE_RETURN(X_PT_LINESTRIP, D3DPT_LINESTRIP)
				CASE_RETURN(X_PT_TRIANGLELIST, D3DPT_TRIANGLELIST)
				CASE_RETURN(X_PT_TRIANGLESTRIP, D3DPT_TRIANGLESTRIP)
				CASE_RETURN(X_PT_TRIANGLEFAN, D3DPT_TRIANGLEFAN)
		}
		return X_PT_INVALID;
	}

	xulong GetClearBufferFlag(xulong flag)
	{
		xulong ret = 0;
		if (flag & X_CLEAR_TARGET)
			ret |= D3DCLEAR_TARGET;
		if (flag & X_CLEAR_STENCIL)
			ret |= D3DCLEAR_STENCIL;
		if (flag & X_CLEAR_ZBUFFER)
			ret |= D3DCLEAR_ZBUFFER;
		return ret;
	}
}

#endif // XD3D9RenderUtil

#endif