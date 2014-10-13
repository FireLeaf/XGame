/**************************************************************************
/*		FileName: XTextureManager.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/21 10:35:25
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XTEXTUREMANAGER__H
#define __XTEXTUREMANAGER__H

#include "XType.h"
#include "XAsset.h"

class XTextureManager
{
public:
	typedef XStl::pair<XStl::string, xbool> TexPair;
	struct TexKey 
	{
		XStl::string path;
		xbool dynamic;
		TexKey(){}
		TexKey(const XStl::string str, xbool dy):path(str), dynamic(dy){}
		operator size_t () const
		{
			xint64 value = 0;
			for (int i = 0; i < path.length(); i++)
			{
				value += path[i];
			}
			value += dynamic;
			return (size_t)value;
		}
		bool operator < (const TexKey& tk) const
		{
			return false;
		}
	};
	typedef XStlext::hash_map<TexKey, xint32> PathToAssetIDMap;
public:
	XTexture2D* LoadTexture2D(const XTexFormatDesc& desc, const XTextureData& data, xbool dynamic);
protected:
	PathToAssetIDMap map_path_to_assetid;
	xint32 default_asset_id;//Ä¬ÈÏµÄÆåÅÌ¸ñ
};

#endif // XTextureManager