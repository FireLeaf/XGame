/**************************************************************************
/*		FileName: XMesh.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/09 21/37/40
/*		CopyRight: yc 
/*************************************************************************/

#include "XMesh.h"
#include "XFile.h"
#include "XAssetUtil.h"

bool XMesh::InitMesh()
{
// 	for (int i = 0; i < )
// 	{
// 	}
	return true;
}

bool XMesh::LoadMesh(const char* file_name)
{
	XFile file;
	if(!file.OpenFile(file_name, "rb"))
	{
		return false;
	}
	xint32 vertex_num;
	file.QuickReadValue(vertex_num);
	X3DVector3* poses = new X3DVector3[vertex_num];
	file.Read((void*)poses, sizeof(X3DVector3), vertex_num);

	MeshVertexBufferPTX0N* vertex_pool = new MeshVertexBufferPTX0N;
	geometry_data.vertex_attribute = AssetUtil::GetVertexAttribute(PTX0N_elements);
	geometry_data.vertex_pools.push_back(vertex_pool);


	vertex_pool->Allocate(vertex_num);
	AnimationVertexPTXnN<1>* pVData;
	vertex_pool->Lock((void**)&pVData);
	for (int i = 0; i < vertex_num; i++)
	{
		pVData[i].x = poses[i].x;
		pVData[i].y = poses[i].y;
		pVData[i].z = poses[i].z;
	}
	//¶¥µã³Ø
	XVertexPoolDesc vertex_pool_desc;
	vertex_pool_desc.buffer = (xbyte*)pVData;
	vertex_pool_desc.count = vertex_pool->Count();
	XVertexPool* ptr_vertex_pool = AssetUtil::GetVertexPool(vertex_pool_desc, false);
	if (!ptr_vertex_pool)
	{
		return false;
	}
	geometry_data.asset_vertex_pools.push_back(ptr_vertex_pool);
	vertex_pool->UnLock();

	xint32 face_num;
	xint32 indices_num;
	file.QuickReadValue(face_num);
	indices_num = face_num * 3;
	geometry_data.indices_pool = new MeshIndexBuffer16;
	geometry_data.indices_pool->Allocate(indices_num);
	void* pData = NULL;
	geometry_data.indices_pool->Lock(&pData);
	file.Read(pData, sizeof(xushort), indices_num);
	XIndexPoolDesc index_pool_desc;
	index_pool_desc.buffer = (xbyte*)pData;
	index_pool_desc.count = geometry_data.indices_pool->Count();
	index_pool_desc.type = INDEX_16BITS;
	geometry_data.asset_index_pool = AssetUtil::GetIndexPool(index_pool_desc, false);
	geometry_data.indices_pool->UnLock();
	
	MaterialEntityDesc med;
	med.vertex_shader_desc.entry = "VS_Main";
	med.vertex_shader_desc.profile = "vs_2_0";
	med.vertex_shader_desc.shader_path = "AssetBundle\\shader\\mesh_vs.shader";
	med.vertex_shader_desc.from = SHADER_FROM_FILE;

	med.pixel_shader_desc.entry = "PS_Main";
	med.pixel_shader_desc.profile = "ps_2_0";
	med.pixel_shader_desc.shader_path = "AssetBundle\\shader\\mesh_ps.shader";
	med.pixel_shader_desc.from = SHADER_FROM_FILE;

	material_entity = AssetUtil::GetMaterialEntity(med);
	if (!material_entity)
	{
		return false;
	}
	return true;
}

void XMesh::Render(XRII* rii, XRenderArgs* args)
{
	XVertexShader* vs = material_entity->vertex_shader;
	if(vs)
	{
		memset(&matWorld, 0, sizeof(matWorld));
		matWorld[0] = matWorld[5] = matWorld[10] = matWorld[15] = 1.0f;
		vs->GetShaderParamTable()->SetValue("view_proj_matrix", &args->mat_view_proj, sizeof(args->mat_view_proj));
		vs->GetShaderParamTable()->SetValue("world", &matWorld, sizeof(matWorld));
	}

	xint32 tri_count = geometry_data.indices_pool->Count() / (3 * sizeof(xushort));
	rii->DrawIndexEntity(geometry_data.vertex_attribute,
		geometry_data.asset_vertex_pools,
		geometry_data.asset_index_pool,
		material_entity,
		X_PT_TRIANGLELIST,
		0,
		tri_count
		);
}