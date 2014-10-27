/************************************************************************
    file_name 	: skinmesh.h								 
	file_desc 	:														 
	file_author	: YangCao											 
	file_time	: 2013/12/16 22:40					 
**************************************************************************/

#ifndef _H_SKINMESH_H_
#define _H_SKINMESH_H_

#include <vector>
class Point3;
class Matrix;

struct Vertex_t 
{
	Point3 pos, normal,texcoord;
	int matID;
	int iEffBone;
	struct
	{
		int boneIdx;
		float weight;
	}Bone[4];
};

struct Bone_t
{
	Matrix NodeInitTM;
	char Name[32], ParentName[32];
};

class CSkeleton
{
public:
	static CSkeleton& Get()
	{
		static CSkeleton instance;
		return instance;
	}
public:
	bool LoadSkeleton(const char* szFileName);
	int FindBoneIndex(INode* pNode);
	void SaveBone(FILE* fp, INode* pNode, bool bRoot);
	void SaveSkeleton(const char* szFileName, INode* pRootNode);
protected:
	std::vector<Bone_t> m_vecBones;
};


struct SMVertex
{
	union{
		float x,y,z;
		D3DXVECTOR3 pos;
	};
	D3DXCOLOR diffuse;
	D3DXVECTOR3 tex1;
	D3DXVECTOR3 tex2;
	D3DXVECTOR3 normal;
	D3DXVECTOR3 binormal;
	D3DXVECTOR3 tangent;
};

#endif