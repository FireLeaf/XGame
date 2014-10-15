/*******************************************************************************
	FILE:		XTerrainLoader.h
	
	DESCRIPTTION:	
	
	CREATED BY: YangCao, 2014/10/14

	Copyright (C) - All Rights Reserved with Coconat
*******************************************************************************/

#ifndef __COCONAT_XTERRAINLOADER_H_
#define __COCONAT_XTERRAINLOADER_H_

class XTerrainLoader
{
public:
	XTerrainLoader(XTerrain* pTerrain);
	void InitTerrain();
	void LoadChunkArea(const XAnchorPos& pos);
protected:
	XTerrain* ptr_terrain;
};

#endif