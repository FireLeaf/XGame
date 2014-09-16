/**************************************************************************
/*		FileName: XAssetLoader.h
/*		FileDesc: asset loader ,
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/01 21/03/49
/*		CopyRight: yc 
/*************************************************************************/

class XAssetLoader
{
public:
	bool LoadAsset(XAsset* pAsset);
protected:
	//
	bool Load2DTexture(XAsset* pAsset);
	bool LoadCubeTexture(XAsset* pAsset);
	bool Load3DTexture(XAsset* pAsset);
	//
};