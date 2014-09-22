/**************************************************************************
/*		FileName: XAssetUtil.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/21 0:33:08
/*		CopyRight: yc 
/*************************************************************************/

#include "XAssetUtil.h"
#include "XAssetMonitor.h"

namespace AssetUtil
{
	struct VertexAttributeCache 
	{
		typedef XStl::pair<XStl::vector<Vertex_Decl_Element>, XVertexAttribute*> VDE_Asset_Pair;
		XStl::vector<VDE_Asset_Pair> cacheDeclElements;

		XVertexAttribute* CreateVertexAttribute(const Vertex_Decl_Element* vde, const XStl::vector<Vertex_Decl_Element>& vecVde)
		{
			XAsset* ptr_asset = x_ptr_asset_monitor->CreateAsset(ASSET_VERTEX_ATTRIBUTE, false);
			if (!ptr_asset)
			{
				return NULL;
			}
			XVertexAttribute* ptr_vab = (XVertexAttribute*)ptr_asset;
			XVertexAttributeDesc vad;
			Vertex_Decl_Element next = VERTEX_DECL_NEXT_STREAM();
			Vertex_Decl_Element end = VERTEX_DECL_END(); 
			int iCurIndex = 0;
			int iCurStream = 0;
			XVertexAttributeDesc::VertexElement ve;
			while(1)
			{
				if (vde[iCurIndex] == end)
				{
					break;
				}

				if (vde[iCurIndex] == next)
				{
					ve.vecElement.clear();
					ve.stream_index = iCurStream;
					iCurStream++;
					vad.AddVertexElement(ve);
				}
				else
				{
					ve.vecElement.push_back(vde[iCurIndex]);
				}

				iCurStream++;
			}
			ptr_vab->SetVertexAttributeDesc(vad);
			ptr_vab->UpdateAsset(x_ptr_asset_monitor);
			VDE_Asset_Pair vap;
			vap.second = ptr_vab;
			vap.first = vecVde;
			cacheDeclElements.push_back(vap);
			return ptr_vab;
		}
	};

	XVertexAttribute* GetVertexAttribute(const Vertex_Decl_Element* vde)
	{
		if (!vde)
		{
			return NULL;
		}
		static VertexAttributeCache cache;
		Vertex_Decl_Element next = VERTEX_DECL_NEXT_STREAM();
		Vertex_Decl_Element end = VERTEX_DECL_END(); 
		
		int iCurIndex = 0;
		XStl::vector<Vertex_Decl_Element> vecVDE;
		while(1)
		{
			if (vde[iCurIndex] == end)
			{
				break;
			}
			if (vde[iCurIndex] == next)
			{
				
			}
			else
			{
				vecVDE.push_back(vde[iCurIndex]);
			}
			iCurIndex++;
		}
		int i = 0;
		for (; i < cache.cacheDeclElements.size(); i++)
		{
			if (cache.cacheDeclElements[i].first.size() != vecVDE.size())
			{
				continue;
			}
			int j = 0;
			for ( ; j < vecVDE.size(); j++)
			{
				if (cache.cacheDeclElements[i].first[i] != vecVDE[i])
				{
					break;
				}
			}

			if (j >= vecVDE.size())
			{
				break;
			}
		}

		if (i < cache.cacheDeclElements.size())
		{
			return cache.cacheDeclElements[i].second;
		}
		
		return cache.CreateVertexAttribute(vde, vecVDE);
	}

	XVertexPool* GetVertexPool(const XVertexPoolDesc& desc, xbool dynamic)
	{
		XVertexPool* ptr_asset = (XVertexPool*)x_ptr_asset_monitor->CreateAsset(ASSET_VERTEX_POOL, dynamic);
		if (!ptr_asset)
		{
			return ptr_asset;
		}
		ptr_asset->SetVertexPoolDesc(desc);
		ptr_asset->UpdateAsset(x_ptr_asset_monitor);
		return ptr_asset;
	}

	XIndexPool* GetIndexPool(const XIndexPoolDesc& desc, xbool dynamic)
	{
		XIndexPool* ptr_asset = (XIndexPool*)x_ptr_asset_monitor->CreateAsset(ASSET_INDEX_POOL, dynamic);
		if (!ptr_asset)
		{
			return ptr_asset;
		}
		ptr_asset->SetIndexPoolDesc(desc);
		ptr_asset->UpdateAsset(x_ptr_asset_monitor);
		return ptr_asset;
	}

	XTexture2D* GetTexture2D(const XTexFormatDesc& desc, const XTextureData& data, xbool dynamic)
	{
		XTexture2D* ptr_asset = (XTexture2D*)x_ptr_asset_monitor->CreateAsset(ASSET_TEXTURE_2D, dynamic);
		if (!ptr_asset)
		{
			return ptr_asset;
		}
		ptr_asset->SetTextureDesc(desc);
		ptr_asset->SetPixelData(data);
		ptr_asset->UpdateAsset(x_ptr_asset_monitor);
		return ptr_asset;
	}
}