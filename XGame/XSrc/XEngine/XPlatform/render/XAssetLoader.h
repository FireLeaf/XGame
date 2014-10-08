/**************************************************************************
/*		FileName: XAssetLoader.h
/*		FileDesc: asset loader ,
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/01 21/03/49
/*		CopyRight: yc 
/*************************************************************************/

#include "XAsset.h"

class XAssetLoader
{
public:
	enum DATATYPE
	{
		RGBDATA		= 3,
		RGBADATA	= 4,
	};
public:
	static bool LoadAsset(XAsset* pAsset);
protected:
	//
	static bool Load2DTexture(XAsset* pAsset);
	static bool LoadCubeTexture(XAsset* pAsset);
	static bool Load3DTexture(XAsset* pAsset);
	//
};