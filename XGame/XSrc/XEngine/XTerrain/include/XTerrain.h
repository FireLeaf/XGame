/**************************************************************************
/*		FileName: XTerrain.h
/*		FileDesc: 无限型的地图
/*		FileAuthor: YangCao
/*		CreateTime: 2014/10/08 22:42:46
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XTERRAIN__H
#define __XTERRAIN__H

//带有顶点，UV，法线
template<int N>
struct TerrainVertexPTXnN
{
	float x,y,z;
	X3DVector2 uv[N];
	float nx, ny, nz;
};

const static Vertex_Decl_Element PTX0N_elements[] = 
{
	{X_DECLTYPE_FLOAT3, 0, 0, X_DECLUSAGE_POSITION, 0},
	{X_DECLTYPE_FLOAT2, 12, 0, X_DECLUSAGE_TEXCOORD, 0},
	{X_DECLTYPE_FLOAT3, 20, 0, X_DECLUSAGE_NORMAL, 0},
	VERTEX_DECL_END()
};

struct TerrainVertexBufferPTX0N : public XBufferData<typename TerrainVertexPTXnN<0> >
{

};

struct TerrainIndexBuffer16 : public XBufferData<xushort> 
{

};

struct XAnchorPos 
{
	int x,y,z;
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
	XAABB aabb;
};

struct XArea//
{
public:
	typedef stdext::hash_map<XAnchorPos, XChunkArea> PosToChunkAreaMap;
protected:
	XAnchorPos pos;
	PosToChunkAreaMap map_pos_chunkarea;
	XAABB aabb;
};

class XTerrain : public XRenderEntity
{
public:
	typedef stdext::hash_map<XAnchorPos, XArea*> PosToAreaMap;
public:
	XTerrain();
	virtual void Render(XRII* rii, XRenderArgs* args);
	void Tick(const XVector3& pos, xuint32 time_delta);
protected:
	float chunk_side;//一个chunk的边长
	int chunk_area_edges;//一个chunk_area边含有的chunk数
	float chunk_area_side;//= chunk_side * chunk_area_edges
	int area_edges;//一个area边含有的chunk_area数量
	float area_side;// = chunk_area_side * area_edges
//area
	PosToAreaMap map_pos_area;
//lod factor
	float factor_lod1;
	float factor_lod2;
//可视设置
	int visable_length;//可视长度
	int unload_length;//卸载距离
};

#endif // XTerrain