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
	vertex_pool->UnLock();

	xint32 indices_num;
	file.QuickReadValue(indices_num);
	geometry_data.indices_pool = new MeshIndexBuffer16;
	geometry_data.indices_pool->Allocate(indices_num);
	void* pData = NULL;
	geometry_data.indices_pool->Lock(&pData);
	file.Read(pData, sizeof(xushort), indices_num);
	geometry_data.indices_pool->UnLock();

}