/**************************************************************************
/*		FileName: XAssetUtil.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/21 0:33:08
/*		CopyRight: yc 
/*************************************************************************/

#include "XAssetUtil.h"
#include "XAssetMonitor.h"
#include "XRenderUtil.h"

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
			ve.stride = 0;
			while(1)
			{
				if (vde[iCurIndex] == end)
				{
					break;
				}

				if (vde[iCurIndex] == next)
				{
					ve.stream_index = iCurStream;
					iCurStream++;
					vad.AddVertexElement(ve);
					ve.vecElement.clear();
					ve.stride = 0;
				}
				else
				{
					ve.stride += RenderUtil::GetValueTypeSize(vde[iCurIndex].data_type);
					ve.vecElement.push_back(vde[iCurIndex]);
				}

				iCurIndex++;
			}
			
			//加上最后一个
			ve.stream_index = iCurStream;
			vad.AddVertexElement(ve);

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
		for (; i < (int)cache.cacheDeclElements.size(); i++)
		{
			if (cache.cacheDeclElements[i].first.size() != vecVDE.size())
			{
				continue;
			}
			int j = 0;
			for ( ; j < (int)vecVDE.size(); j++)
			{
				if (cache.cacheDeclElements[i].first[i] != vecVDE[i])
				{
					break;
				}
			}

			if (j >= (int)vecVDE.size())
			{
				break;
			}
		}

		if (i < (int)cache.cacheDeclElements.size())
		{
			return cache.cacheDeclElements[i].second;
		}
		
		return cache.CreateVertexAttribute(vde, vecVDE);
	}

	XVertexPool* GetVertexPool(const XVertexPoolDesc& desc, bool dynamic)
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

	XIndexPool* GetIndexPool(const XIndexPoolDesc& desc, bool dynamic)
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

	XTexture2D* GetTexture2D(const XTexFormatDesc& desc, const XTextureData& data, bool dynamic)
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

	XVertexShader* GetVertexShader(const XVertexShaderDesc& desc)
	{
		XVertexShader* ptr_asset = (XVertexShader*)x_ptr_asset_monitor->CreateAsset(ASSET_VERTEX_SHADER, false);
		if (!ptr_asset)
		{
			return ptr_asset;
		}
		ptr_asset->SetVertexShaderDesc(desc);
		ptr_asset->UpdateAsset(x_ptr_asset_monitor);
		return ptr_asset;
	}

	XPixelShader* GetPixelShader(const XPixelShaderDesc& desc)
	{
		XPixelShader* ptr_asset = (XPixelShader*)x_ptr_asset_monitor->CreateAsset(ASSET_PIXEL_SHADER, false);
		if (!ptr_asset)
		{
			return ptr_asset;
		}
		ptr_asset->SetPixelShaderDesc(desc);
		ptr_asset->UpdateAsset(x_ptr_asset_monitor);
		return ptr_asset;
	}

	XMateriaEntity* GetMaterialEntity(const MaterialEntityDesc& desc)
	{
		XMateriaEntity* ptr_mtrl_entity = new XMateriaEntity;
		memcpy(&ptr_mtrl_entity->render_flag , &desc.render_flag, sizeof(desc.render_flag));
		ptr_mtrl_entity->vertex_shader = GetVertexShader(desc.vertex_shader_desc);
		ptr_mtrl_entity->pixel_shader = GetPixelShader(desc.pixel_shader_desc);
		return ptr_mtrl_entity;
	}
}