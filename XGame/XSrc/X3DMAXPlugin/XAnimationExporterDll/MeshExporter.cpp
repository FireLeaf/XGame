/**************************************************************************
/*		FileName: MeshExporter.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/05/28 23/42/25
/*		CopyRight: yc 
/*************************************************************************/

#include "stdafx.h"
#include "MeshExporter.h"
#include "MeshExporterUtil.h"

XMeshExporter::XMeshExporter()
{
	m_iTriCount = 0;
	m_pExportNode = NULL;
	m_pExportMesh = NULL;
	m_pSkinVertices = NULL;
	m_pStaticVertices = NULL;
	m_stType = SKIN_NONE;
}

void XMeshExporter::ExporterTestModel()
{
	Interface* ip = GetCOREInterface();
	if (!ip)
	{
		return;
	}
	FILE* fp = fopen(m_strExportFile.c_str(), "wb");
	if (!fp)
	{
		return;
	}
	INode* pNode = ip->GetSelNode(0);
	if (!pNode)
	{
		return;
	}

	Mesh* pMesh = MeshExporterUtil::GetMesh(pNode, 0);
	if (!pMesh)
	{
		return;
	}

	int iVertexNum = pMesh->numVerts;
	fwrite(&iVertexNum, sizeof(int), 1, fp);
	//PosVertex* vertices = new PosVertex[iVertexNum];
	fwrite(pMesh->verts, sizeof(Point3), iVertexNum, fp);

	int iFaceNum = pMesh->numFaces;
	fwrite(&iFaceNum, sizeof(iFaceNum), 1, fp);

	unsigned short* indices = new unsigned short[iFaceNum * 3];
	for (int i = 0; i < iFaceNum; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			indices[i * 3 + j] = (unsigned short)pMesh->faces[i].v[j];
		}
	}

	fwrite(indices, sizeof(unsigned short), iFaceNum * 3, fp);
	fclose(fp);
}

bool XMeshExporter::InitNode(INode* pNode)
{
	if (!pNode)
	{
		Assert(0);
		return false;
	}
	m_pExportNode = pNode;
	m_pExportMesh = MeshExporterUtil::GetMesh(pNode);
	if (!m_pExportMesh)
	{
		Assert(0);
		return false;
	}

	if (m_pSkinVertices)
	{
		delete m_pSkinVertices[];
		m_pSkinVertices = NULL;
	}

	if (m_pStaticVertices)
	{
		delete m_pStaticVertices[];
		m_pStaticVertices = NULL;
	}

	m_stType = SKIN_NONE;

	if(MeshExporterUtil::FindModifier(pNode, SKIN_CLASSID))
	{
		m_stType = SKIN_SKIN;
		AnalyseSkin();
	}
	else if (MeshExporterUtil::FindModifier(pNode, Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B)))
	{
		m_stType = SKIN_PHYSIQUE;
		AnalysePhysque();
	}
	else
	{
		AnalyseStatic();
	}


	m_iTriCount = m_pExportMesh->getNumFaces();

	return true;
}

void XMeshExporter::AnalyseSkin()
{
	if (!m_pExportNode || !m_pExportMesh)
	{
		return;
	}

	Modifier* pModifier = MeshExporterUtil::FindModifier(m_pExportNode, SKIN_CLASSID);
	ISkin* pSkin = (ISkin*)pModifier->GetInterface(I_SKIN);
	if (!pSkin)
	{
		Assert(0);
		return;
	}
	UVVert m_pExportMesh->mapVerts()

}

void XMeshExporter::ExportSkinMtrl(INode* node, std::string& file)
{

}

void XMeshExporter::ExportSkinMesh(INode* node, std::string& file)
{

}

void XMeshExporter::ExportSkelton(INode* node, std::string& file)
{

}