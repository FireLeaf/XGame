/**************************************************************************
/*		FileName: XTerrain.h
/*		FileDesc: �����͵ĵ�ͼ
/*		FileAuthor: YangCao
/*		CreateTime: 2014/10/08 22:42:46
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XTERRAIN__H
#define __XTERRAIN__H

//���ж��㣬UV������
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

struct XChunk//һ��С����
{
	XAnchorPos pos;
};

struct XChunkArea//����һ���������ݿ�
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
	float chunk_side;//һ��chunk�ı߳�
	int chunk_area_edges;//һ��chunk_area�ߺ��е�chunk��
	float chunk_area_side;//= chunk_side * chunk_area_edges
	int area_edges;//һ��area�ߺ��е�chunk_area����
	float area_side;// = chunk_area_side * area_edges
//area
	PosToAreaMap map_pos_area;
//lod factor
	float factor_lod1;
	float factor_lod2;
//��������
	int visable_length;//���ӳ���
	int unload_length;//ж�ؾ���
};

#endif // XTerrain