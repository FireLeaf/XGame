/*******************************************************************************
	FILE:		XD3D9AssetMonitor.cpp
	
	DESCRIPTTION:	
	
	CREATED BY: YangCao, 2014/08/04

	Copyright (C) - All Rights Reserved with Coconat
*******************************************************************************/

#include "XD3D9Header.h"
#include "XD3D9AssetMonitor.h"
#include "XD3D9Asset.h"

#define ASSET_CASE_NEW(a, b) case a:\
					  {\
						b* ptr = new b;\
						if(ptr) \
						{\
							ptr->uAssetDataID = GetAssetDataStamp();\
							ptr->m_bDynamic = bDynamic;\
							m_mapAsset[ptr->uAssetDataID] = ptr;\
						}\
						return ptr;\
					  }\
					  break;

XD3D9AssetMonitor d3d9_asset_monitor;
XAssetMonitor* x_ptr_asset_monitor = &d3d9_asset_monitor;

void XD3D9AssetMonitor::UpdateAsset(XAsset* pAsset)
{
	if (pAsset)
	{
		pAsset->UpdateAsset(this);
	}
}

XAsset* XD3D9AssetMonitor::CreateAsset(ENUM_ASSET_TYPE asset_type, bool bDynamic)
{
	switch (asset_type)
	{
		ASSET_CASE_NEW(ASSET_VERTEX_POOL, XD3D9VertexPool)
		ASSET_CASE_NEW(ASSET_INDEX_POOL, XD3D9IndexPool)
		ASSET_CASE_NEW(ASSET_TEXTURE_2D, XD3D9Texture2D)
		ASSET_CASE_NEW(ASSET_TEXTURE_CUBE, XD3D9TextureCube)
		ASSET_CASE_NEW(ASSET_TEXTURE_RENDER, XD3D9RenderTarget)
		ASSET_CASE_NEW(ASSET_VERTEX_ATTRIBUTE, XD3D9VertexAttribute)
		ASSET_CASE_NEW(ASSET_VERTEX_SHADER, XD3D9VertexShader)
		ASSET_CASE_NEW(ASSET_PIXEL_SHADER, XD3D9PixelShader)
	}
	return NULL;
}

void XD3D9AssetMonitor::ReleaseAsset(XAsset* pAsset)
{
	if (!pAsset)
	{
		Assert(0);
		return;
	}
	int ref = pAsset->GetRef();
	pAsset->ReleaseAsset(this);
	MapAsset::iterator iter = m_mapAsset.find(pAsset->uAssetDataID);
	if (1 == ref && iter != m_mapAsset.end())
	{
		m_mapAsset.erase(iter);
	}
}