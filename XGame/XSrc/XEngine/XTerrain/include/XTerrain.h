/**************************************************************************
/*		FileName: XTerrain.h
/*		FileDesc: 无限型的地图
/*		FileAuthor: YangCao
/*		CreateTime: 2014/10/08 22:42:46
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XTERRAIN__H
#define __XTERRAIN__H

#include "XRenderEntity.h"
#include "XMath.h"
#include "XVector.h"
#include "XType.h"
#include "XTemplate.h"
#include "XAsset.h"

//带有顶点，UV，法线
template<int N>
struct TerrainVertexPTXnN
{
	float x,y,z;
	XVector2 uv[N];
	float nx, ny, nz;
};

struct TerrainVertexBufferPTX0N : public XBufferData<typename TerrainVertexPTXnN<1> >
{

};

struct TerrainIndexBuffer16 : public XBufferData<xushort> 
{

};

struct XAnchorPos 
{
	int x,y,z;

	operator size_t () const
	{
		return x + y + z;
	}

	bool operator < (const XAnchorPos& pos) const 
	{
		return x < pos.x && y < pos.y && z < pos.z;
	}
};

struct XChunk//一个小区块
{
	XAnchorPos pos;
};

struct XChunkArea//共享一个顶点数据块
{
	XAnchorPos pos;
	XGeometryData<TerrainVertexBufferPTX0N, TerrainIndexBuffer16> geometry_data;
	XMateriaEntity* material_entity;
	//XVertexAttribute* vertex_attrib;
	XAABB aabb;
public:
	void Render(XRII* rii, XRenderArgs* args);
};

struct XArea//
{
public:
	typedef stdext::hash_map<XAnchorPos, XChunkArea*> PosToChunkAreaMap;
public:
	void Render(XRII* rii, XRenderArgs* args);
public:
	XAnchorPos pos;
	PosToChunkAreaMap map_pos_chunkarea;
	XAABB aabb;
};

class XTerrain : public XRenderEntity
{
public:
	typedef stdext::hash_map<XAnchorPos, XArea*> PosToAreaMap;
	friend class XTerrainLoader;
public:
	XTerrain();
	bool Init(const char* terrain_file);

	virtual void Render(XRII* rii, XRenderArgs* args);

	void Tick(const XVector3& pos, xuint32 time_delta);
	void TickIndex(const XVector3& pos);
	void ThreadLoadChunkArea(XChunkArea*);
protected:
	void ClearLod();
	int GetLod();
protected:
	float edges_side;//一个最小单位的边长
	int chunk_edges;//一个chunk 一条边含有的边数
	float chunk_side;//一个chunk的边长
	int chunk_area_edges;//一个chunk_area边含有的chunk数
	float chunk_area_side;//= chunk_side * chunk_area_edges
	int area_edges;//一个area边含有的chunk_area数量
	float area_side;// = chunk_area_side * area_edges
//area
	PosToAreaMap map_pos_area;
	//std::vector<>
//lod factor
	float factor_lod1;
	float factor_lod2;
//可视设置
	int visable_chunks;//可视长度
	int unload_length;//卸载距离
//
	int* lod;//(2 * visable_chunks + 1) * (2 * visable_chunks + 1)
	XAnchorPos anchor_pos;//当前点
	XTerrainLoader* ptr_terrain_loader;

	XMateriaEntity* material_entity;
	XTexture2D* terrain_texture;
};

#endif // XTerrain