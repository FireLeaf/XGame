/**************************************************************************
/*		FileName: XMesh.h
/*		FileDesc: ������
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/09 21/24/51
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XMESH__H
#define __XMESH__H

#include "XType.h"
#include "XAsset.h"
#include "XRenderEntity.h"
#include "XTemplate.h"
//#include ""
/*
P ���� position vec3
D ���� diffuse dword
T0 1���������� texture float2
T* * + 1 ����������
N �������� normal float3
B ������ binormal float3
T ���� float3
W Ȩ�� weights float4 
I ���� int4
*/

struct X3DVector2 
{
	float x,y;
};

struct X3DVector3
{
	float x,y,z;
};

//���ж��㣬UV������
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
	VERTEX_DECL_END()
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
	VERTEX_DECL_END(),
};


struct MeshVertexBufferPTX0N : public XBufferData<typename AnimationVertexPTXnN<1> > 
{
};

struct MeshIndexBuffer16 : public XBufferData<xushort>
{

};

class XMesh : public XRenderEntity
{
public:
	XMesh() : material_entity(NULL){}
	virtual void Render(XRII* rii, XRenderArgs* args);
public:
	bool InitMesh();
	bool LoadMesh(const char* file_name);
protected:
	XGeometryData<MeshVertexBufferPTX0N, MeshIndexBuffer16> geometry_data;
	XMateriaEntity* material_entity;
	float matWorld[16];
};

#endif // XMesh