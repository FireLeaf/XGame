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
#include "Phyexp.h"


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

void XMeshExporter::ExportModel()
{
	std::string sk = m_strExportFile + ".sk";
	std::string sm = m_strExportFile + ".sm";
	ExportSkelton(sk);
	ExportSkinMesh(sm);
}

bool XMeshExporter::InitCurSelNode()
{
	Interface* ip = GetCOREInterface();
	if (ip)
	{
		return InitNode(ip->GetSelNode(0));
	}

	return false;
}

bool XMeshExporter::InitNode(INode* pNode)
{
	if (!pNode)
	{
		Assert(0);
		return false;
	}
	m_pExportNode = pNode;
	m_pExportMesh = MeshExporterUtil::GetMesh(pNode, 0);
	if (!m_pExportMesh)
	{
		Assert(0);
		return false;
	}

	m_skeltonFrame.InitSkelton();
	m_skeltonFrame.BuildBoneFrame();

	if (m_pSkinVertices)
	{
		delete []m_pSkinVertices;
		m_pSkinVertices = NULL;
	}

	if (m_pStaticVertices)
	{
		delete []m_pStaticVertices;
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
	memcpy(&vOut.uv[0], pMesh->tVerts[uvIdx], sizeof(vOut.uv[0]));
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
	memcpy(&vOut.uv[0], pMesh->tVerts[uvIdx], sizeof(vOut.uv[0]));

	int vtxType = vtxInterface->GetVertexType();
	if (RIGID_TYPE == vtxType)
	{
		//vOut.iEffBone = 1;
		INode* pBoneNode = ((IPhyRigidVertex*)vtxInterface)->GetNode();
		vOut.i[0]/*.boneIdx*/ = m_skeltonFrame.FindBoneIndex(pBoneNode);
		vOut.w[0]/*.weight*/ = 1.0f;
		for (int i = 1; i < MAX_EFFECT_BONE; i++)
		{
			vOut.i[i]/*.boneIdx*/ = -1;
			vOut.w[i]/*.weight*/ = 0.0f;
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
				int boneIdx = m_skeltonFrame.FindBoneIndex(pBoneNode);
				vOut.i[i]/*.boneIdx*/ = boneIdx;
				vOut.w[i]/*.weight*/ = vtxBlendInt->GetWeight(i);
			}
			else
			{
				vOut.i[i]/*.boneIdx*/ = -1;
				vOut.w[i]/*.weight*/ = 0.0f;
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
	m_pSkinVertices = new ExpAnimSkinVertex[m_iTriCount * 3];
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
			m_pIndices[v_index] = v_index;
			v_index++;
		}
	}
}

void XMeshExporter::AnalysePhysque()
{
}

void XMeshExporter::AnalyseStatic()
{

}

void XMeshExporter::ExportSkinMtrl(std::string& file)
{
	
}

void XMeshExporter::ExportSkinMesh(std::string& file)
{
	XFile fp;
	if (fp.OpenFile(file.c_str(), "wb"))
	{
		int count = m_iTriCount * 3;
		fp.QuickWriteValue(count);
		fp.Write(m_pSkinVertices, m_iTriCount * 3 * sizeof(ExpAnimSkinVertex), 1);
		fp.QuickWriteValue(count);
		fp.Write(m_pIndices, m_iTriCount * 3 * sizeof(unsigned short), 1);
	}
}

void XMeshExporter::ExportSkelton(std::string& file)
{
	m_skeltonFrame.SaveSkeleton(file.c_str());
}

void XMeshExporter::ExportActionClip()
{
	for (int i = 0; i < m_vecActionClip.size(); i++)
	{
		XFile fp;
		std::string file = m_strExportFile + m_vecActionClip[i].strClipName;
		file += ".ac";
		if (!fp.OpenFile(file.c_str(), "wb"))
		{
			Assert(0);
			return;
		}
		int iTickPerFrame = GetTicksPerFrame();
		for (int iBone = 0; iBone < m_skeltonFrame->GetBoneCount(); iBone++)
		{
			int keyCount = m_vecActionClip[i].iEndFrame - m_vecActionClip[i].iStartFrame + 1;
			XScaleKey* scale_key_array = new XScaleKey[keyCount];
			XPosKey* pos_key_array = new XPosKey[keyCount];
			XRotKey* rot_key_array = new XRotKey[keyCount];
			for (int j = m_vecActionClip[i].iStartFrame; j <= m_vecActionClip[i].iEndFrame; j++)
			{
				Bone* bone = m_skeltonFrame->GetBone(iBone);
				char bone_name[32] = {'\0'};
				strncpy(bone_name, bone->name.c_str(), sizeof(bone_name));
				fp
				if (!bone)
				{
					Assert(0);
				}
				else
				{
					TimeValue tv = j * iTickPerFrame;
					Matrix3 mat = bone->pNode->GetNodeTM(tv);
					if (bone->pParentNode)
					{
						Matrix3 matParent = bone->pParentNode->GetNodeTM(tv);
						mat = mat * Inverse(matParent);
					}
					XVector3 tran, scal;
					XQuaternion quat;

					MeshExporterUtil::DecompMatrix3(mat, tran, scal, quat);
					
					//std::vector<XMatKey> mat_key_array;
					

				}
			}
		}
	}
}