/**************************************************************************
/*		FileName: XSkinMesh.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/10/07 2:00:57
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XSKINMESH__H
#define __XSKINMESH__H

#include "XTemplate.h"

template<int N>
struct AnimationVertexPTXnWI
{
	float x,y,z;
	XVector2 uv[N];
	float w[4];
	xbyte i[4];
};

const static Vertex_Decl_Element PTX0WI_elements[] = 
{
	{X_DECLTYPE_FLOAT3, 0, 0, X_DECLUSAGE_POSITION, 0},
	{X_DECLTYPE_FLOAT2, 12, 0, X_DECLUSAGE_TEXCOORD, 0},
	{X_DECLTYPE_FLOAT4, 20, 0, X_DECLUSAGE_BLENDWEIGHT, 0},
	{X_DECLTYPE_UBYTE4, 36, 0, X_DECLUSAGE_BLENDINDICES, 0},
	VERTEX_DECL_END(),
};

template<int N>
struct AnimationVertexPTXnNBTWI
{
	float x,y,z;
	XVector2 uv[N];
	float nx, ny, nz;
	float bx, by, bz;
	float tx, ty, tz;
	float w[4];
	xbyte i[4];
};

const static Vertex_Decl_Element PTX0NBTWI_elements[] = 
{
	{X_DECLTYPE_FLOAT3, 0, 0, X_DECLUSAGE_POSITION, 0},
	{X_DECLTYPE_FLOAT2, 12, 0, X_DECLUSAGE_TEXCOORD, 0},
	{X_DECLTYPE_FLOAT2, 20, 0, X_DECLUSAGE_TEXCOORD, 1},
	{X_DECLTYPE_FLOAT3, 28, 0, X_DECLUSAGE_NORMAL, 0},
	{X_DECLTYPE_FLOAT3, 40, 0, X_DECLUSAGE_BINORMAL, 0},
	{X_DECLTYPE_FLOAT3, 52, 0, X_DECLUSAGE_TANGENT, 0},
	{X_DECLTYPE_FLOAT4, 64, 0, X_DECLUSAGE_BLENDWEIGHT, 0},
	{X_DECLTYPE_UBYTE4, 80, 0, X_DECLUSAGE_BLENDINDICES, 0},
	VERTEX_DECL_END(),
};

template<int N>
struct AnimationVertexPTXnNBTWIP
{
	float x,y,z;
	XVector2 uv[N];
	float nx, ny, nz;
	float bx, by, bz;
	float tx, ty, tz;
	float w[4];
	xbyte i[4];

	float x1,y1,z1;
};

const static Vertex_Decl_Element PTX0NBTWIP_elements[] = 
{
	{X_DECLTYPE_FLOAT3, 0, 0, X_DECLUSAGE_POSITION, 0},
	{X_DECLTYPE_FLOAT2, 12, 0, X_DECLUSAGE_TEXCOORD, 0},
	{X_DECLTYPE_FLOAT2, 20, 0, X_DECLUSAGE_TEXCOORD, 1},
	{X_DECLTYPE_FLOAT3, 28, 0, X_DECLUSAGE_NORMAL, 0},
	{X_DECLTYPE_FLOAT3, 40, 0, X_DECLUSAGE_BINORMAL, 0},
	{X_DECLTYPE_FLOAT3, 52, 0, X_DECLUSAGE_TANGENT, 0},
	{X_DECLTYPE_FLOAT4, 64, 0, X_DECLUSAGE_BLENDWEIGHT, 0},
	{X_DECLTYPE_UBYTE4, 80, 0, X_DECLUSAGE_BLENDINDICES, 0},
	VERTEX_DECL_NEXT_STREAM(),
	{X_DECLTYPE_FLOAT3, 0, 0, X_DECLUSAGE_POSITION, 0},
	VERTEX_DECL_END(),
};

struct SkinMeshVertexBufferPTX1WI : public XBufferData<typename AnimationVertexPTXnWI<1> >
{

};

struct SkinMeshIndexBuffer16 : public XBufferData<xushort>
{

};

class XSkinMesh : public XRenderEntity//蒙皮网格
{
public:
	virtual void Render(XRII* rii, XRenderArgs* args);
protected:
	std::vector<std::string> bind_bone_names;//需要绑定到的骨骼名字
	std::vector<int> bind_bone_idxes;//骨骼的索引

	XGeometryData<SkinMeshVertexBufferPTX1WI, SkinMeshIndexBuffer16> geometry_data;
	XMateriaEntity* material_entity;
};

#endif // XSkinMesh