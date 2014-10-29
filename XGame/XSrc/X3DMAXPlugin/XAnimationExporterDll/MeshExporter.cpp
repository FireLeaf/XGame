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

void XMeshExporter::GetVertexBoneInfo(INode* pNode, ISkin* pSkin, Mesh* pMesh, int vertexIdx, int uvIdx, ExpAnimSkinVertex& vOut)
{
	ISkinContextData* pSkinCtx = pSkin->GetContextInterface(pNode);
	int nBones = pSkinCtx->GetNumAssignedBones(vertexIdx);
	memcpy(&vOut.pos, pMesh->verts[vertexIdx]/* * pNode->GetObjectTM(0, NULL)*/, sizeof(Point3));
	memcpy(&vOut.uv[0], pMesh->tVerts[uvIdx]);
	//vOut.iEffBone = nBones;
	if (nBones > MAX_EFFECT_BONE)
	{
		Assert(0);
	}
	for (int i = 0; i < MAX_EFFECT_BONE; i++)
	{
		if (i < nBones)
		{
			INode* pBone = pSkin->GetBone(pSkinCtx->GetAssignedBone(vertexIdx, i));
			vOut.w[i] = pSkinCtx->GetBoneWeight(vertexIdx, i);
			vOut.i[i] = m_skeltonFrame.FindBoneIndex(pBone);
		}
		else
		{
			vOut.w[i] = 0.0f;
			vOut.i[i] = -1;
		}
	}
}

void XMeshExporter::GetVertexBoneInfo(INode* pNode, Modifier* pPhyMod, Mesh* pMesh, int vertexIdx, int uvIdx, ExpAnimSkinVertex& vOut)
{
	IPhysiqueExport* phyInterface = (IPhysiqueExport*)pPhyMod->GetInterface(I_PHYINTERFACE);
	if (!phyInterface)
	{
		return;
	}

	IPhyContextExport* pPhysicMC = phyInterface->GetContextInterface(pNode);
	if (!pPhysicMC)
	{
		return;
	}
	pPhysicMC->ConvertToRigid(TRUE);
	pPhysicMC->AllowBlending(TRUE);
	const int numVertices = pPhysicMC->GetNumberVertices();

	IPhyVertexExport* vtxInterface = pPhysicMC->GetVertexInterface(vertexIdx);
	if (!vtxInterface)
	{
		return;
	}
	memcpy(&vOut.pos, pMesh->verts[vertexIdx]/* * pNode->GetObjectTM(0, NULL)*/, sizeof(Point3));
	memcpy(&vOut.uv[0], pMesh->tVerts[uvIdx]);

	int vtxType = vtxInterface->GetVertexType();
	if (RIGID_TYPE == vtxType)
	{
		//vOut.iEffBone = 1;
		INode* pBoneNode = ((IPhyRigidVertex*)vtxInterface)->GetNode();
		vOut.w[0].boneIdx = m_skeltonFrame.FindBoneIndex(pBoneNode);
		vOut.w[0].weight = 1.0f;
		for (int i = 1; i < MAX_EFFECT_BONE; i++)
		{
			vOut.i[i].boneIdx = -1;
			vOut.i[i].weight = 0.0f;
		}
	}
	else if(RIGID_BLENDED_TYPE == vtxType)
	{
		IPhyBlendedRigidVertex* vtxBlendInt = (IPhyBlendedRigidVertex*)vtxInterface;
		if ( vtxBlendInt->GetNumberNodes() > MAX_EFFECT_BONE)
		{
			Assert(0);
		}
		for (int i = 0; i < MAX_EFFECT_BONE; i++)
		{
			if (i < vtxBlendInt->GetNumberNodes())
			{
				INode* pBoneNode = vtxBlendInt->GetNode(i);
				int boneIdx = CSkeleton::Get().FindBoneIndex(pBoneNode);
				vOut.i[i].boneIdx = boneIdx;
				vOut.i[i].weight = vtxBlendInt->GetWeight(i);
			}
			else
			{
				vOut.i[i].boneIdx = -1;
				vOut.i[i].weight = 0.0f;
			}
			
		}
	}
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
	//mapSupport(mapIndex)

	//UVVert m_pExportMesh->mapVerts()
	//见链接http://liuxuvslisa.blog.sohu.com/164529178.html
	//当前只取map
	m_iTriCount = m_pExportMesh->getNumFaces();
	m_iVertCount = m_pExportMesh->getNumTVerts();
	m_pSkinVertices = new ExpAnimSkinVertex(m_iTriCount);
	m_pIndices = new unsigned short[m_iTriCount * 3];
	
// 	for (int i = 0; i < m_iVertCount; i++)
// 	{
// 		GetVertexBoneInfo(m_pExportNode, pSkin, m_pExportMesh, i, )
// 	}

	int v_index = 0;
	for (int i = 0; i < m_iTriCount; i++)
	{
		TVFace& tvFace = m_pExportMesh->tvFace[i];
		Face& face = m_pExportMesh->faces[i];
		for (int j = 0; j < 3; j++)
		{
			//vert.matID = face.getMatID();
			GetVertexBoneInfo(m_pExportNode, pSkin, m_pExportMesh, face.v[j], tvFace.getTVert(j), m_pSkinVertices[v_index]);
			v_index++;
		}
	}
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