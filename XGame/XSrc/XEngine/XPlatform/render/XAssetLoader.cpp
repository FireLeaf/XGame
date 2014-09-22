/**************************************************************************
/*		FileName: XAssetLoader.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/21 11:18:29
/*		CopyRight: yc 
/*************************************************************************/

#include "XAssetLoader.h"
#include "XFile.h"

bool XAssetLoader::LoadAsset(XAsset* pAsset)
{
	if (!pAsset)
	{
		return false;
	}

	switch(pAsset->GetAssetType())
	{
	case ASSET_TEXTURE_2D:
		return Load2DTexture(pAsset);
		break;
	case ASSET_TEXTURE_CUBE:
		return LoadCubeTexture(pAsset);
		break;
	default:
		break;
	}
	return false;
}

bool LoadTGA(XFileMap& fm, XTextureData& data)
{

	return true;
}

bool XAssetLoader::Load2DTexture(XAsset* pAsset)
{

	if (!pAsset || 
		ASSET_TEXTURE_2D != pAsset->GetAssetType()||
		TEXTURE_LOAD_FROM_FILE != ((XTexture2D*)pAsset)->GetFormatDesc().from
		)
	{
		return false;
	}
	XTexture2D* ptr_texture_2d = (XTexture2D*)pAsset;
	XFileMap fm;
	if(!fm.OpenFile(ptr_texture_2d->GetPixelData().m_textureFile.c_str(), "rb"))
	{
		return false;
	}
#ifdef _WIN_PLATFORM
	if(LoadTGA(fm, ptr_texture_2d->GetTextureData()))
	{
		return false;
	}
#elif defined(_XOS_PLATFORM)
#endif

	return true;
}