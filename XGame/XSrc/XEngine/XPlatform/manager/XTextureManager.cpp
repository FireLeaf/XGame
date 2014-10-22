/**************************************************************************
/*		FileName: XTextureManager.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/21 10:39:38
/*		CopyRight: yc 
/*************************************************************************/

#include "XTextureManager.h"
#include "XAssetMonitor.h"
#include "XAssetUtil.h"
#include "XAssetLoader.h"

XTextureManager& XTextureManager::GetInstance()
{
	static XTextureManager inst;
	return inst;
}

XTexture2D* XTextureManager::LoadTexture2D(const XTexFormatDesc& desc, const XTextureData& data, bool dynamic)
{
	if (TEXTURE_LOAD_FROM_FILE != desc.from)
	{
		return AssetUtil::GetTexture2D(desc, data, dynamic);
	}

	TexKey key(data.m_textureFile, dynamic);
	PathToAssetIDMap::iterator iter = map_path_to_assetid.find(key);
	if (iter != map_path_to_assetid.end())
	{
		XAsset* ptr_asset = x_ptr_asset_monitor->GetAsset(iter->second);
		if (ptr_asset && ASSET_TEXTURE_2D == ptr_asset->GetAssetType() && ptr_asset->Dynamic() == dynamic)
		{
			return (XTexture2D*)ptr_asset;
		}
	}
	
	XAsset* ptr_asset = x_ptr_asset_monitor->CreateAsset(ASSET_TEXTURE_2D, dynamic);
	if (!ptr_asset)
	{
		return NULL;
	}

	XTexture2D* ptr_texture_2d = (XTexture2D*)ptr_asset;
	ptr_texture_2d->SetTextureDesc(desc);
	ptr_texture_2d->SetPixelData(data);
	if(!XAssetLoader::LoadAsset(ptr_asset))
	{
		ptr_asset->ReleaseAsset(x_ptr_asset_monitor);
		return NULL;
	}
	ptr_asset->UpdateAsset(x_ptr_asset_monitor);
	map_path_to_assetid.insert(std::make_pair<TexKey, xint32>(key, ptr_asset->GetAssetID()));
	return ptr_texture_2d;
}