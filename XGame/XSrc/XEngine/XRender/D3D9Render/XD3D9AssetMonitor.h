/*******************************************************************************
	FILE:		XD3D9AssetMonitor.h
	
	DESCRIPTTION:	
	
	CREATED BY: YangCao, 2014/08/04

	Copyright (C) - All Rights Reserved with Coconat
*******************************************************************************/


#ifndef __COCONAT_XD3D9ASSETMONITOR_H_
#define __COCONAT_XD3D9ASSETMONITOR_H_

#include "XAssetMonitor.h"

class XD3D9AssetMonitor : public XAssetMonitor
{
public:
	virtual void UpdateAsset(XAsset* pAsset);
	virtual XAsset* CreateAsset(ENUM_ASSET_TYPE asset_type, xbool bDynamic);//创建一个资源
	virtual void ReleaseAsset(XAsset* pAsset);
	virtual void ResetAsset(){}
public:
};

#endif