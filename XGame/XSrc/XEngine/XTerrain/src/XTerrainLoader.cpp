/*******************************************************************************
	FILE:		XTerrainLoader.cpp
	
	DESCRIPTTION:	
	
	CREATED BY: YangCao, 2014/10/14

	Copyright (C) - All Rights Reserved with Coconat
*******************************************************************************/

#include "XTerrainLoader.h"

const static Vertex_Decl_Element T_PTX0N_elements[] = 
{
	{X_DECLTYPE_FLOAT3, 0, 0, X_DECLUSAGE_POSITION, 0},
	{X_DECLTYPE_FLOAT2, 12, 0, X_DECLUSAGE_TEXCOORD, 0},
	{X_DECLTYPE_FLOAT3, 20, 0, X_DECLUSAGE_NORMAL, 0},
	VERTEX_DECL_END()
};

XTerrainLoader::XTerrainLoader()
{
	ptr_terrain = NULL;
}

void XTerrainLoader::LoadChunkArea(const XAnchorPos& pos)
{
	XChunkArea* area = new XChunkArea;
	area->pos = pos;
	TerrainVertexBufferPTX0N* vertex_pool = new TerrainVertexBufferPTX0N;
	area->geometry_data.vertex_attribute = AssetUtil::GetVertexAttribute(T_PTX0N_elements);
	area->geometry_data.vertex_pools.push_back(vertex_pool);

	int vertex_num = (ptr_terrain->chunk_edges * ptr_terrain->chunk_area_edges) + 1;
	vertex_num *= vertex_num;
	vertex_pool->Allocate(vertex_num);
	TerrainVertexPTXnN<1>* pVData = NULL;
	vertex_pool->Lock((void**)&pVData);
	for (int i = 0; i < vertex_num; i++)
	{
		pVData[i].x = ;
		pVData[i].y = XRandomFloat(0.0f, 256.0f);
	}

	int index_num = ptr_terrain->chunk_edges * ptr_terrain->chunk_area_edges;
	index_num = index_num * index_num * 2;
}