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
#include "XVector.h"
#include "XMath.h"
#include "XMathUtil.h"
#include "XType.h"
#include "XFile.h"
#include "iskin.h"

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
	XMatrix NodeInitOffsetTM;
	std::string name;
	std::string parent_name;
	int index;
	std::vector<int> child_bones;
	int parent_bone;
	INode* pNode;
	INode* pParentNode;
};

class XSkeleton
{
public:
// 	static XSkeleton& Get()
// 	{
// 		static XSkeleton instance;
// 		return instance;
// 	}
public:
	bool LoadSkeleton(const char* szFileName);
	int FindBoneIndex(const std::string& name)
	{
		for (int i = 0; i < (int)m_vecBones.size(); i++)
		{
			if (m_vecBones[i].name == name)
			{
				return i;
			}
		}
		return -1;
	}
	int FindBoneIndex(INode* pNode)
	{
		for (int i = 0; i < (int)m_vecBones.size(); i++)
		{
			if (m_vecBones[i].pNode == pNode)
			{
				return i;
			}
		}
		return -1;
	}
	void SaveBone(FILE* fp, INode* pNode, bool bRoot);
	void SaveSkeleton(const char* szFileName)
	{
		XFile fp;// = fopen(szFileName, "wb");
		if (fp.OpenFile(szFileName, "wb"))
		{
			for (int i = 0; i < (int)m_vecBones.size(); i++)
			{
				fp.QuickWriteValue(m_vecBones[i].parent_bone);
				fp.QuickWriteValue(m_vecBones[i].NodeInitOffsetTM);
				fp.QuickWriteValue(m_vecBones[i].child_bones.size());
				for (int j = 0; j < (int)m_vecBones[i].child_bones.size(); j++)
				{
					fp.QuickWriteValue(m_vecBones[i].child_bones[j]);
				}
			}
		}
	}

	void InitSkelton()
	{
		INode* pRootBone = MeshExporterUtil::GetRootBoneNode();
		if (pRootBone)
		{
			m_vecBones.clear();
			AddBone(pRootBone);
		}
	}

	void AddBone(INode* pNode, INode* pParentNode = NULL)
	{
		if (!pNode)
		{
			return;
		}
		Bone bone;
		bone.pNode = pNode;
		bone.name = pNode->GetName();
		bone.pParentNode = pParentNode;
		bone.parent_name = pParentNode ? pParentNode->GetName() : "";
		m_vecBones.push_back(bone);
		for (int i  = 0; i < (int)pNode->NumChildren(); i++)
		{
			AddBone(pNode->GetChildNode(i), pNode);
		}
	}

	void BuildBoneFrame()
	{
		for (int i = 0; i < (int)m_vecBones.size(); i++)
		{
			Bone& bone = m_vecBones[i];
			bone.index = i;
			if (!bone.pParentNode)
			{
				bone.parent_bone = -1;
				//XMathMatrixIdentity(bone.NodeInitOffsetTM);
				Matrix3 mat = bone.pNode->GetNodeTM(0);
				MeshExporterUtil::Matrix3ToXMatrix(mat, bone.NodeInitOffsetTM);
			}
			else
			{
				int parent_index = FindBoneIndex(bone.pParentNode);
				if (-1 != parent_index)
				{
					bone.index = i;
					bone.parent_bone = parent_index;
					Assert(parent_index >= 0 && parent_index < (int)m_vecBones.size());
					m_vecBones[parent_index].child_bones.push_back(i);
					Matrix3 matParent = bone.pParentNode->GetNodeTM(0);
					Matrix3 mat = bone.pNode->GetNodeTM(0);
					Matrix3 reltive = Inverse(mat * Inverse(matParent));
					MeshExporterUtil::Matrix3ToXMatrix(reltive, bone.NodeInitOffsetTM);
				}
				else
				{
					bone.parent_bone = -1;
					Assert(0);
				}
			}
		}
	}
	
	int GetBoneCount(){return (int)m_vecBones.size();}
	Bone* GetBone(int iIndex)
	{
		if (iIndex >= 0 || iIndex < (int)m_vecBones.size())
		{
			return &(m_vecBones[iIndex]);
		}
		return NULL;
	}
protected:
	std::vector<Bone> m_vecBones;
};

enum EXPORT_TYPE
{
	EXPORT_STATIC_MODEL,
	EXPORT_SKIN_MODEL,
};

enum EXPORT_OPTION
{
	OP_EXPORT_SKIN,
	OP_EXPORT_KEY,
	OP_EXPORT_SKELTON,
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
	typedef std::vector<ActionClip> ActionClipVector;
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
	void ExportModel();

	bool InitNode(INode* pNode);//初始化此node
	bool InitCurSelNode();

	void ExportSkinMtrl(std::string& file);//导出材质
	void ExportSkinMesh(std::string& file);//导出蒙皮
	void ExportSkelton(std::string& file);//导出骨架
	void ExportStaticMesh(std::string& file);//导出静态模型

	void ExportActionClip();//导出动作

public:
	void AddActionClip(const std::string& name, int iStartFrame, int iEndFrame);
	void DeleteActionClip(const std::string& name);
	void UpdateActionClip(const std::string& name, int iStartFrame, int iEndFrame);
protected:
	ActionClipVector::iterator FindActionClip(const std::string& name);
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
	ActionClipVector m_vecActionClip;//动作
};


#endif // MeshExporter