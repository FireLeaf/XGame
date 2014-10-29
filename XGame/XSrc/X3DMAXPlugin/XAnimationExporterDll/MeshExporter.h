/**************************************************************************
/*		FileName: MeshExporter.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/05/28 23/29/46
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __MESHEXPORTER__H
#define __MESHEXPORTER__H

#include "MeshExporterUtil.h"

#define MAX_EFFECT_BONE 4

template<int N>
struct AnimationVertexPTXnN
{
	XVector3 pos;
	XVector2 uv[N];
	XVector3 normal;
};

template<int N>
struct AnimationVertexPTXnNBT
{
	XVector3 pos;
	//float u[N], v[N];
	XVector2 uv[N];
	XVector3 normal;
	XVector3 binormal;
	XVector3 tangent;
};

template<int N>
struct AnimationVertexPTXnWI
{
	XVector3 pos;
	XVector2 uv[N];
	float w[MAX_EFFECT_BONE];
	xbyte i[MAX_EFFECT_BONE];
};

template<int N>
struct AnimationVertexPTXnNWI
{
	XVector3 pos;
	XVector2 uv[N];
	XVector3 normal;
	float w[MAX_EFFECT_BONE];
	xbyte i[MAX_EFFECT_BONE];
};

template<int N>
struct AnimationVertexPTXnNBTWI
{
	XVector3 pos;
	XVector2 uv[N];
	XVector3 normal;
	XVector3 binormal;
	XVector3 tangent;
	float w[MAX_EFFECT_BONE];
	xbyte i[MAX_EFFECT_BONE];
};

template<int N>
struct AnimationVertexPTXnNBTWIP
{
	XVector3 pos;
	XVector2 uv[N];
	XVector3 normal;
	XVector3 binormal;
	XVector3 tangent;
	float w[MAX_EFFECT_BONE];
	xbyte i[MAX_EFFECT_BONE];

	float x1,y1,z1;
};

struct ActionClip 
{
	std::string strClipName;
	int iStartFrame;
	int iEndFrame;
};

struct Bone
{
	Matrix NodeInitTM;
	char Name[32], ParentName[32];
	std::vector<int> child_bones;
	int parent_bone;
};

class XSkeleton
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

	void InitSkelton()
	{
		INode* pRootBone = MeshExporterUtil::GetRootBoneNode();
		if (pRootBone)
		{
			m_vecBones.clear();
			INode* pRootBone->
		}
	}
protected:
	std::vector<Bone> m_vecBones;
};

class XMeshExporter
{
public:
	enum SKIN_TYPE
	{
		SKIN_NONE,
		SKIN_SKIN,
		SKIN_PHYSIQUE,
	};
	typedef AnimationVertexPTXnNWI<1> ExpAnimSkinVertex;
public:
	static XMeshExporter& Get()
	{
		static XMeshExporter inst;
		return inst;
	}
public:
	XMeshExporter();
	void SetExporterFilePath(const char* file){m_strExportFile = file;}
	void ExporterTestModel();

	bool InitNode(INode* pNode);//初始化此node

	void ExportSkinMtrl(std::string& file);//导出材质
	void ExportSkinMesh(std::string& file);//导出蒙皮
	void ExportSkelton(std::string& file);//导出骨架
	void ExportStaticMesh(std::string& file);//导出静态模型

	void ExportActionClip();//导出动作
protected:
	void AnalyseSkin();
	void AnalysePhysque();
	void AnalyseStatic();

	void GetVertexBoneInfo(INode* pNode, Modifier* pPhyMod, Mesh* pMesh, int vertexIdx, int uvIdx, ExpAnimSkinVertex& vOut);
	void GetVertexBoneInfo(INode* pNode, ISkin* pSkin, Mesh* pMesh, int vertexIdx, int uvIdx, ExpAnimSkinVertex& vOut);

	void GetKeyFrameNormal(INode* pNode);
	void GetKeyFrameByIGame(INode* pNode);
protected:
	std::string m_strExportFile;
	XSkeleton m_skeltonFrame;
	int m_iTriCount;
	int m_iVertCount;//顶点数量
	//int iMtrlCount;
	INode* m_pExportNode;
	Mesh* m_pExportMesh;
	SKIN_TYPE m_stType;
	ExpAnimSkinVertex* m_pSkinVertices;
	AnimationVertexPTXnN<1>* m_pStaticVertices;
	unsigned short* m_pIndices;
	XVector3* m_pVertNormals;//每个顶点的法线
	XVector3* m_pFaceNormals;//每个面的法线，和tvFace顺序一致
	std::vector<int>* m_pVertFaceRefIndexs;//每个顶点被访问的
	std::vector<ActionClip> m_vecActionClip;//动作
};


#endif // MeshExporter