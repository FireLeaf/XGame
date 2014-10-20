/*******************************************************************************
	FILE:		XTerrainLoader.h
	
	DESCRIPTTION:	
	
	CREATED BY: YangCao, 2014/10/14

	Copyright (C) - All Rights Reserved with Coconat
*******************************************************************************/

#ifndef __COCONAT_XTERRAINLOADER_H_
#define __COCONAT_XTERRAINLOADER_H_

#include "XTerrain.h"

struct TerrainHeader 
{
	int magic;
	int version;
	char map_name[32];
	float chunk_side;//
	int chunk_area_edges;
	float chunk_area_side;
	int area_edges;
	float area_side;
	int area_count;//��������
	int mtrl_count;//�õ��Ĳ�������

};

struct AreaInfo 
{
	XAnchorPos pos;
	int* mtrl;
};

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