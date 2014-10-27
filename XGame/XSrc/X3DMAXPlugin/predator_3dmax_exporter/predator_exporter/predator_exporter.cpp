// predator_exporter.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "predator_exporter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "Max.h"
#include "resource.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"
#include "skinmesh.h"
#include <iskin.h>
#include <modstack.h> 
#include <stdmat.h> 
#include <cs/Phyexp.h>
#include <IGame/IGame.h> 
#include "cs/bipexp.h"
//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// Cpredator_exporterApp

BEGIN_MESSAGE_MAP(Cpredator_exporterApp, CWinApp)
END_MESSAGE_MAP()


// Cpredator_exporterApp 构造

Cpredator_exporterApp::Cpredator_exporterApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 Cpredator_exporterApp 对象

Cpredator_exporterApp theApp;


// Cpredator_exporterApp 初始化

BOOL Cpredator_exporterApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

void OnSaveSkeleton()
{
	CSkeleton& sk = CSkeleton::Get();
	Interface* ip = GetCOREInterface();
	const char* szFileName = GetSaveFileName();
	if (szFileName)
	{
		sk.SaveSkeleton(szFileName, ip->GetSelNode(0));
	}
}

void OnExportAnimation()
{
	const char* szFileName = GetSaveFileName();
	FILE* fp = fopen(szFileName, "wb");
	Interface* ip = GetCOREInterface();
	Interval range = ip->GetAnimRange();
	TimeValue tAniTime = range.End() - range.Start();
	TimeValue tTime = range.Start();
	int nFrame = tAniTime / GetTicksPerFrame();
	fwrite(&nFrame, sizeof(int), 1， fp);
	
	CSkeleton& sk = CSkeleton::Get();
	for (int i = 0; i < nFrame; i++, tTime += GetTicksPerFrame())
	{
		for (int iBone = 0; iBone < sk.m_vecBones.size(); iBone++)
		{
			Bone_t& bone = sk->m_vecBones[iBone];
			INode* pBoneNode = ip->GetINodeByName(bone->Name);
			Matrix mat = pBoneNode->GetNodeTM(tTime);
			pBoneNode
		}
	}
}

ISkin* FindSkinModifier(INode*	pNode)
{
	if (!pNode)
	{
		return NULL;
	}
	Object* pObject = pNode->GetObjectRef();
	if (!pObject)
	{
		return;
	}

	while(GEN_DERIVOB_CLASS_ID == pObject->SuperClassID())
	{
		IDerivedObject* pDerivedObject = static_cast<IDerivedObject*>(pObject);
		for (int stackid = 0; stackid < pDerivedObject->NumModifiers(); stackid++)
		{
			Modifier* pModifier = pDerivedObject->GetModifier(stackid);
			if (SKIN_CLASSID == pModifier->ClassID())
			{
				return (ISkin*)pModifier->GetInterface(I_SKIN);
			}
		}

		pObject = pDerivedObject->GetObjRef();
	}

	return NULL;
}
class NullView : public View
{
	Point2 ViewToScreen(Point3 p)
	{ return Point2(p.x,p.y); }
	NullView() {
		worldToView.IdentityMatrix();
		screenW=640.0f; screenH = 480.0f;
	}
};


Mesh* GetMesh(INode* pNode, int iMaxTime)
{
	NullView view;
	bool bNeedDelete = false;
	ObjectState os = pNode->EvalWorldState(iMaxTime);
	Object* pObj = os.obj;
	if (pObj)
	{
		TriObject* triObject= (TriObject*)pObj->ConvertToType(iMaxTime, TRIOBJ_CLASS_ID);
		if (triObject)
		{
			GeomObject* pGeoObj = (GeomObject*)pObj;
			Mesh* pMesh = pGeoObj->GetRenderMesh(iMaxTime, pNode, view, bNeedDelete);
			return pMesh;
		}
	}

	return NULL;
}

void GetVertexBoneInfo(INode* pNode, ISkin* pSkin, Mesh* pMesh, int vertexIdx, int uvIdx, Vertex_t& vOut)
{
	CSkeleton& sk = CSkeleton::Get();
	ISkinContextData* pSkinCtx = pSkin->GetContextInterface(pNode);
	int nBones = pSkinCtx->GetNumAssignedBones(vertexIdx);
	vOut.pos = pMesh->verts[vertexIdx] * pNode->GetObjectTM(0, NULL);
	vOut.texcoord = pMesh->texCoords[uvIdx];
	vOut.iEffBone = nBones;
	for (int i = 0; i < nBones; i++)
	{
		INode* pBone = pSkin->GetBone(pSkinCtx->GetAssignedBone(vertexIdx, i));
		vOut.Bone[i].weight = pSkinCtx->GetBoneWeight(vertexIdx, i);
		vOut.Bone[i].boneIdx = sk.FindBoneIndex(pBone);
	}
}

void GetVertexBoneInfo(INode* pNode, Modifier* pMod, Mesh* pMesh, int vertexIdx, int uvIdx, Vertex_t& vOut)
{
	CSkeleton& sk = CSkeleton::Get();
	IPhysiqueExport* phyInterface = (IPhysiqueExport*)pMod->GetInterface(I_PHYINTERFACE);
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
	vOut.pos = pMesh->verts[vertexIdx] * pNode->GetObjectTM(0, NULL);
	vOut.texcoord = pMesh->texCoords[uvIdx];

	int vtxType = vtxInterface->GetVertexType();
	if (RIGID_TYPE == vtxType)
	{
		vOut.iEffBone = 1;
		INode* pBoneNode = ((IPhyRigidVertex*)vtxInterface)->GetNode();
		vOut.Bone[0].boneIdx = sk.FindBoneIndex(pBoneNode);
		vOut.Bone[0].weight = 1.0f;
	}
	else if(RIGID_BLENDED_TYPE == vtxType)
	{
		IPhyBlendedRigidVertex* vtxBlendInt = (IPhyBlendedRigidVertex*)vtxInterface;

		for (int i = 0; i < vtxBlendInt->GetNumberNodes(); i++)
		{
			INode* pBoneNode = vtxBlendInt->GetNode(i);
			int boneIdx = CSkeleton::Get().FindBoneIndex(pBoneNode);
			vOut.Bone[vOut.iEffBone].boneIdx = boneIdx;
			vOut.Bone[vOut.iEffBone].weight = vtxBlendInt->GetWeight(i);
			vOut.iEffBone++;
		}
	}
}

void SaveStdMaterial(FILE* fp, StdMat* pMtl)
{
	if (!fp || !pMtl)
	{
		return;
	}

	
}

void SaveMaterial(const char* fileMat, INode* pNode)
{
	FILE* fp = fopen(fileMat, "wb");
	if (!fp)
	{
		return;
	}
	Mtl* pMtl = pNode->GetMtl();
	int iSubMtl = pMtl->NumSubMtls();
	if (0 == iSubMtl)
	{
		SaveStdMaterial(fp, (StdMat*)pMtl);
	}
	else
	{
		for (int i = 0; i < iSubMtl; i++)
		{
			SaveStdMaterial(fp, (StdMat*)pMtl->GetSubMtl(i));
		}
	}
}

bool IsBone(INode* pNode)
{
	if (!pNode)
	{
		return false;
	}

	ObjectState os = pNode->EvalWorldState(0);
	if (!os.obj)
	{
		return false;
	}
	
	if (os.obj->ClassID() == Class_ID(BONE_OBJ_CLASSID/*BONE_CLASS_ID*/, 0)) return true;
	if(os.obj->ClassID() == Class_ID(DUMMY_CLASS_ID, 0)) return false;
	Control* ct = pNode->GetTMController;
	return ct->ClassID() == BIPSLAVE_CONTROL_CLASS_ID || BIPBODY_CONTROL_CLASS_ID;

}

INode* GetRootBone(INode* pNode)
{
	ISkin* pSkin = FindSkinModifier(pNode);

}

void SaveMesh(const char* fileMesh, INode* pNode, Mesh* pMesh)
{
	ISkin* pSkin = FindSkinModifier(pNode);
	FILE* fp = fopen(fileMesh, "wb");

	int iFaceNum = pMesh->numFaces;
	fwrite(&iFaceNum, sizeof(iFaceNum), 1, fp);
	for (int i = 0; i < iFaceNum; i++)
	{
		TVFace& tvFace = pMesh->tvFace[i];
		Face& face = pMesh->faces[i];
		for (int j = 0; j < 3; j++)
		{
			Vertex_t vert;
			vert.matID = face.getMatID();
			GetVertexBoneInfo(pNode, pSkin, pMesh, face.v[j], tvFace.getTVert(j), vert);
			fwrite(&vert, sizeof(vert), 1, fp);
		}
	}
}

void ExportKeyFrame(INode* pNode)
{
	Control* posCtl = pNode->GetTMController()->GetPositionController();
	Control* scalCtl = pNode->GetTMController()->GetScaleController();
	Control* rotCtl = pNode->GetTMController()->GetRotationController();

	IKeyControl* posIkc = GetKeyControlInterface(posCtl);
	IKeyControl* scalIkc = GetKeyControlInterface(scalCtl);
	IKeyControl* rotIkc = GetKeyControlInterface(rotCtl);

	if (posCtl->ClassID() == Class_ID(LININTERP_POSITION_CLASS_ID, 0))
	{
		for (int i = 0; i < posIkc->GetNumKeys(); i++)
		{
			ILinPoint3Key key;
			posIkc->GetKey(i, &key);
			IBezPoint3Key bkey;
			bkey.val;
			IBezQuatKey qkey;
		}
	}
}

void ExportKeyFrameByIGame(INode* pNode)
{
	IGameScene* pIGame = GetIGameInterface();
	IGameConversionManager* pCM = GetConversionManager();
	pCM->SetCoordSystem(IGameConversionManager::IGAME_D3D);
	pIGame->InitialiseIGame(true);
	IGameNode* pGameNode = pIGame->GetIGameNode(pNode);
	IGameKeyTab poskeys;
	IGameControl* pGameControl = pGameNode->GetIGameControl();
	pGameControl->GetLinearKeys(poskeys, IGAME_POS);
	for (int i = 0; i < poskeys.Count(); i++)
	{
		poskeys[i].linearKey ;
	}
	pGameControl->GetLinearKeys(poskeys, IGAME_ROT);
	pGameControl->GetTCBKeys(poskeys, IGAME_POS);
	poskeys[0].bezierKey.
}

// void ExportUVInfo(Mesh* pMesh, int indent)
// {
// 	int numTVerts = pMesh->getNumTVerts();
// 	UVVert
// }

extern "C" __declspec(dllexport) int	predator_do_export(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts=FALSE, DWORD options=0)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return TRUE;
}

