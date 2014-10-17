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

bool XTerrain::Init(const char* terrain_file)
{
	MaterialEntityDesc med;
	med.vertex_shader_desc.entry = "VS_Main";
	med.vertex_shader_desc.profile = "vs_2_0";
	med.vertex_shader_desc.shader_path = "AssetBundle\\shader\\terrain_vs.shader";
	med.vertex_shader_desc.from = SHADER_FROM_FILE;

	med.pixel_shader_desc.entry = "PS_Main";
	med.pixel_shader_desc.profile = "ps_2_0";
	med.pixel_shader_desc.shader_path = "AssetBundle\\shader\\terrain_ps.shader";
	med.pixel_shader_desc.from = SHADER_FROM_FILE;

	material_entity = AssetUtil::GetMaterialEntity(med);
	if (!material_entity)
	{
		return false;
	}
}

void XTerrain::Tick(const XVector3& pos, xuint32 time_delta)
{
	//
}

void XTerrain::Render(XRII* rii, XRenderArgs* args)
{
	XMatrix mat_world;
	XMathMatrixIdentity(mat_world);
	for (PosToAreaMap::iterator iter = map_pos_area.begin(); iter != map_pos_area.end(); iter++)
	{
		XArea* area = (XArea*)(iter->second);
		if (area && args->ptr_frustum->DetectAABB(area->aabb))
		{
			for (XArea::PosToChunkAreaMap::iterator iter_area = area->map_pos_chunkarea.begin();
				iter_area != area->map_pos_chunkarea.end(); iter_area++
				)
			{
				XChunkArea* chunk_area = (XChunkArea*)(iter_area->second);
				if (chunk_area)
				{
					XVertexShader* vs = chunk_area->material_entity->vertex_shader;
					if(vs)
					{
						vs->GetShaderParamTable()->SetValue("view_proj_matrix", &args->mat_view_proj, sizeof(args->mat_view_proj));
						vs->GetShaderParamTable()->SetValue("world", &mat_world, sizeof(mat_world));
					}
					rii->DrawIndexEntity(chunk_area->geometry_data.vertex_attribute,
						chunk_area->geometry_data.asset_vertex_pools,
						chunk_area->geometry_data.
						asset_index_pool,
						chunk_area->material_entity,
						X_PT_TRIANGLELIST,
						0,
						chunk_area->geometry_data.tri_count
						);
				}
			}
		}
	}
}

void XTerrain::ThreadLoadChunkArea(XChunkArea* chunk_area)
{
	chunk_area->material_entity = material_entity;
}