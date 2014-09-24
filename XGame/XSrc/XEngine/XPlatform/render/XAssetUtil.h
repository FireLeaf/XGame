/**************************************************************************
/*		FileName: XAssetUtil.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/21 0:30:18
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XASSETUTIL__H
#define __XASSETUTIL__H

#include "XAsset.h"

namespace AssetUtil
{
	XVertexAttribute* GetVertexAttribute(const Vertex_Decl_Element* vde);
	XVertexPool* GetVertexPool(const XVertexPoolDesc& desc, xbool dynamic);
	XIndexPool* GetIndexPool(const XIndexPoolDesc& desc, xbool dynamic);
	XTexture2D* GetTexture2D(const XTexFormatDesc& desc, const XTextureData& data, xbool dynamic);
	XVertexShader* GetVertexShader(const XVertexShaderDesc& desc);
	XPixelShader* GetPixelShader(const XPixelShaderDesc& desc);
	XMateriaEntity* GetMaterialEntity(const MaterialEntityDesc& desc);
}

#endif // XAssetUtil

