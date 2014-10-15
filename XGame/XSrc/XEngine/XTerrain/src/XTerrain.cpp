/**************************************************************************
/*		FileName: XTerrain.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/10/14 20:06:40
/*		CopyRight: yc 
/*************************************************************************/

#include "XTerrain.h"

XTerrain::XTerrain()
{
	chunk_side = 0.0f;
	chunk_area_edges = 0;
	chunk_area_side = 0.0f;
	area_edges = 0;
	area_side = 0.0f;

	factor_lod1 = 0.0f;
	factor_lod2 = 0.0f;
}

void XTerrain::Tick(const XVector3& pos, xuint32 time_delta)
{
	//
}

void XTerrain::Render(XRII* rii, XRenderArgs* args)
{
	for (PosToAreaMap::iterator iter = map_pos_area.begin(); iter != map_pos_area.end(); iter++)
	{
		XArea* area = (XArea*)(iter->second);
		if (area && args->ptr_frustum->DetectAABB(area->aabb))
		{
			rii->DrawIndexEntity();
		}
	}
}

void XTerrain::ThreadLoadChunkArea(XChunkArea* chunk_area)
{
	
}