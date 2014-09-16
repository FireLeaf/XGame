/**************************************************************************
/*		FileName: XMesh.h
/*		FileDesc: 网格类
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/09 21/24/51
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XMESH__H
#define __XMESH__H

#include "XType.h"
//#include ""
/*
P 顶点 position vec3
D 漫反 diffuse dword
T0 1层纹理坐标 texture float2
T* * + 1 层纹理坐标
N 法线坐标 normal float3
B 副法线 binormal float3
T 切线 float3
W 权重 weights float4 
I 索引 int4
*/

//带有顶点，UV，法线
template<int N>
struct AnimationVertexPTXnN
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
};

template<int N>
struct AnimationVertexPTXnNBT
{
	float x,y,z;
	//float u[N], v[N];
	X3DVector2 uv[N];
	float nx, ny, nz;
	float bx, by, bz;
	float tx, ty, tz;
};

const static Vertex_Decl_Element PTX0BT_elements[] = 
{
	{X_DECLTYPE_FLOAT3, 0, 0, X_DECLUSAGE_POSITION, 0},
	{X_DECLTYPE_FLOAT2, 12, 0, X_DECLUSAGE_TEXCOORD, 0},
	{X_DECLTYPE_FLOAT3, 20, 0, X_DECLUSAGE_NORMAL, 0},
	{X_DECLTYPE_FLOAT3, 32, 0, X_DECLUSAGE_BINORMAL, 0},
	{X_DECLTYPE_FLOAT3, 44, 0, X_DECLUSAGE_TANGENT, 0},
};

template<int N>
struct AnimationVertexPTXnWI
{
	float x,y,z;
	X3DVector2 uv[N];
	float w[4];
	xbyte i[4];
};

const static Vertex_Decl_Element PTX0WI_elements[] = 
{
	{X_DECLTYPE_FLOAT3, 0, 0, X_DECLUSAGE_POSITION, 0},
	{X_DECLTYPE_FLOAT2, 12, 0, X_DECLUSAGE_TEXCOORD, 0},
	{X_DECLTYPE_FLOAT4, 20, 0, X_DECLUSAGE_BLENDWEIGHT, 0},
	{X_DECLTYPE_UBYTE4, 36, 0, X_DECLUSAGE_BLENDINDICES, 0},
};

template<int N>
struct AnimationVertexPTXnNBTWI
{
	float x,y,z;
	X3DVector2 uv[N];
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
};

template<int N>
struct AnimationVertexPTXnNBTWIP
{
	float x,y,z;
	X3DVector2 uv[N];
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
};


struct MeshVertexBufferPTX0N : public XBufferData<AnimationVertexPTXnN<1> > 
{
};

struct MeshIndexBuffer16 : public XBufferData<xushort>
{

};

class XMesh : public XRenderEntity
{
public:
	bool InitMesh();
	bool LoadMesh(const char* file_name);
protected:
	XGeometryData<MeshVertexBufferPTX0N, MeshIndexBuffer16> geometry_data;
};

#endif // XMesh