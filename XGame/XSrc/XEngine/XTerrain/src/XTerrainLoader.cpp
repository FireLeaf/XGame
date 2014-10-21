/*******************************************************************************
	FILE:		XTerrainLoader.cpp
	
	DESCRIPTTION:	
	
	CREATED BY: YangCao, 2014/10/14

	Copyright (C) - All Rights Reserved with Coconat
*******************************************************************************/

#include "XTerrainLoader.h"
#include "XAsset.h"
#include "XAssetUtil.h"
#include "XMath.h"

const static Vertex_Decl_Element T_PTX0N_elements[] = 
{
	{X_DECLTYPE_FLOAT3, 0, 0, X_DECLUSAGE_POSITION, 0},
	{X_DECLTYPE_FLOAT2, 12, 0, X_DECLUSAGE_TEXCOORD, 0},
	{X_DECLTYPE_FLOAT3, 20, 0, X_DECLUSAGE_NORMAL, 0},
	VERTEX_DECL_END()
};

XTerrainLoader::XTerrainLoader(XTerrain* pTerrain)
{
	ptr_terrain = pTerrain;
}

void XTerrainLoader::LoadChunkArea(const XAnchorPos& pos)
{
	XChunkArea* chunk_area = new XChunkArea;
	chunk_area->pos = pos;
	TerrainVertexBufferPTX0N* vertex_pool = new TerrainVertexBufferPTX0N;
	chunk_area->geometry_data.vertex_attribute = AssetUtil::GetVertexAttribute(T_PTX0N_elements);
	chunk_area->geometry_data.vertex_pools.push_back(vertex_pool);

	int vertex_num = (ptr_terrain->chunk_edges * ptr_terrain->chunk_area_edges) + 1;
	int row_col_num = vertex_num;
	vertex_num *= vertex_num;
	vertex_pool->Allocate(vertex_num); 
	TerrainVertexPTXnN<1>* pVData = NULL;
	vertex_pool->Lock((void**)&pVData);
	for (int i = 0; i < row_col_num; i++)
	{ 
		for (int j = 0; j < row_col_num; j++)
		{
			TerrainVertexPTXnN<1>& v = pVData[i * row_col_num + j];
			v.x = pos.x * ptr_terrain->chunk_area_side + i * ptr_terrain->edges_side;
			v.y = XRandomFloat(0.0f, 50.0f);
			v.z = pos.z * ptr_terrain->chunk_area_side + j * ptr_terrain->edges_side;
		}
	}
	XVertexPoolDesc vertex_pool_desc;
	vertex_pool_desc.buffer = (xbyte*)pVData;
	vertex_pool_desc.count = vertex_pool->Count();
	XVertexPool* asset_vertex_pool = AssetUtil::GetVertexPool(vertex_pool_desc, false);
	if (!asset_vertex_pool)
		return;
	chunk_area->geometry_data.asset_vertex_pools.push_back(asset_vertex_pool);
	vertex_pool->UnLock();

	int index_num = ptr_terrain->chunk_edges * ptr_terrain->chunk_area_edges;
	row_col_num = index_num + 1;
	index_num = index_num * index_num * 2 * 3;
	chunk_area->geometry_data.indices_pool = new TerrainIndexBuffer16;
	chunk_area->geometry_data.indices_pool->Allocate(index_num);
	xushort* indices = NULL;
	chunk_area->geometry_data.indices_pool->Lock((void**)&indices);

	for (int i = 0; i < row_col_num - 1; i++) 
	{
		for (int j = 0; j < row_col_num - 1; j++)
		{
			indices[ (i* row_col_num + j) * 6 + 0] = i * row_col_num + j;
			indices[ (i* row_col_num + j) * 6 + 1] = i * row_col_num + j + 1;
			indices[ (i* row_col_num + j) * 6 + 2] = (i + 1) * row_col_num + j + 1;

			indices[ (i* row_col_num + j) * 6 + 3] = i * row_col_num + j;
			indices[ (i* row_col_num + j) * 6 + 4] = (i + 1) * row_col_num + j + 1;
			indices[ (i* row_col_num + j) * 6 + 5] = (i + 1) * row_col_num + j;
		}
	}
	XIndexPoolDesc index_pool_desc;
	index_pool_desc.buffer = (xbyte*)indices;
	index_pool_desc.count = chunk_area->geometry_data.indices_pool->Count();
	index_pool_desc.type = INDEX_16BITS;
	chunk_area->geometry_data.asset_index_pool = AssetUtil::GetIndexPool(index_pool_desc, true);
	chunk_area->geometry_data.indices_pool->UnLock();
	
	chunk_area->geometry_data.tri_count = index_num / 3;

	ptr_terrain->ThreadLoadChunkArea(chunk_area);
}