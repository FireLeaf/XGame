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

template<int N>
struct AnimationVertexPTXnN
{
	float x,y,z;
	XVector2 uv[N];
	float nx, ny, nz;
};

template<int N>
struct AnimationVertexPTXnNBT
{
	float x,y,z;
	//float u[N], v[N];
	XVector2 uv[N];
	float nx, ny, nz;
	float bx, by, bz;
	float tx, ty, tz;
};

template<int N>
struct AnimationVertexPTXnWI
{
	float x,y,z;
	XVector2 uv[N];
	float w[4];
	xbyte i[4];
};

template<int N>
struct AnimationVertexPTXnNWI
{
	float x,y,z;
	XVector2 uv[N];
	float nx, ny, nz;
	float w[4];
	xbyte i[4];
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


class XMeshExporter
{
public:
	enum SKIN_TYPE
	{
		SKIN_NONE,
		SKIN_SKIN,
		SKIN_PHYSIQUE,
	};
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

	bool InitNode(INode* pNode);//��ʼ����node

	void ExportSkinMtrl(std::string& file);//��������
	void ExportSkinMesh(std::string& file);//������Ƥ
	void ExportSkelton(std::string& file);//�����Ǽ�
	void ExportStaticMesh(std::string& file);//������̬ģ��
protected:
	void AnalyseSkin();
	void AnalysePhysque();
	void AnalyseStatic();
protected:
	std::string m_strExportFile;
	XSkeltonFrame m_skeltonFrame;
	int m_iTriCount;
	//int iMtrlCount;
	INode* m_pExportNode;
	Mesh* m_pExportMesh;
	SKIN_TYPE m_stType;
	AnimationVertexPTXnNWI<1>* m_pSkinVertices;
	AnimationVertexPTXnN<1>* m_pStaticVertices;
};


#endif // MeshExporter