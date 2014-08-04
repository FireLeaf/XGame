/*******************************************************************************
	FILE:		XD3D9AssetMonitor.h
	
	DESCRIPTTION:	
	
	CREATED BY: YangCao, 2014/08/04

	Copyright (C) - All Rights Reserved with Coconat
*******************************************************************************/


#ifndef __COCONAT_XD3D9ASSETMONITOR_H_
#define __COCONAT_XD3D9ASSETMONITOR_H_

class XD3D9AssetMonitor : public XAssetMonitor
{
public:
	void UpdateAsset(XAsset* pAsset);
	bool CreateAsset(XAsset* pAsset);
	void ReleaseAsset(XAsset* pAsset);
};

#endif