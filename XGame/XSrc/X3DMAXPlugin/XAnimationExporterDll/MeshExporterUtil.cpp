/**************************************************************************
/*		FileName: MeshExporterUtil.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/05/29 23/36/34
/*		CopyRight: yc 
/*************************************************************************/

#include "stdafx.h"
#include "MeshExporterUtil.h"
#include "Shlwapi.h"
#include "assert.h"

namespace MeshExporterUtil
{
	bool IsBone(INode* pNode, int flag)
	{
		if (!pNode)
		{
			return false;
		}

		ObjectState pObs=pNode->EvalWorldState(0);
		Class_ID id = pObs.obj->ClassID();
		SClass_ID sid = pObs.obj->SuperClassID();
		if (pObs.obj->ClassID()==Class_ID(BONE_CLASS_ID,0))
		{
			return true; 
		}
		if (pObs.obj->ClassID()==BONE_OBJ_CLASSID)
		{
			return true; 
		}
		if (pObs.obj->ClassID()==SKELOBJ_CLASS_ID)
		{
			return true; 
		}
		if (pObs.obj->ClassID() == BIPED_CLASS_ID)
		{
			return true;
		}

		if (flag && BONE_INCLUDE_DUMMY && pObs.obj->ClassID() == Class_ID(DUMMY_CLASS_ID, 0))
		{
			return true;
		}

		Control* ct = pNode->GetTMController();
		if(ct)
		{
			return (ct->ClassID() == BIPSLAVE_CONTROL_CLASS_ID) || (ct->ClassID() == BIPBODY_CONTROL_CLASS_ID);
		}
		return false;

	}

	INode* GetRootBoneNode(int flag)
	{
		Interface* ip = GetCOREInterface();
		if (!ip)
		{
			return NULL;
		}
		INode *pNode = ip->GetRootNode();
		if(!pNode)
		{
			return NULL;
		}

		for (int i = 0; i < pNode->NumberOfChildren(); i++)
		{
			INode* pTmp = pNode->GetChildNode(i);
			if(pTmp && IsBone(pTmp, flag))
			{
				return pTmp;
			}
		}

		return NULL;
	}

	INode* GetBoneRootNode(INode* pBoneNode, int flag)
	{
		Interface* ip = GetCOREInterface();
		if (!ip)
		{
			return NULL;
		}
		INode *pRootNode = ip->GetRootNode();
		if(!pRootNode)
		{
			return NULL;
		}

		INode* pTmp = pBoneNode;
		while (pTmp && IsBone(pTmp, flag))
		{
			INode* pNode = pBoneNode->GetParentNode();
			if (!pNode || pRootNode == pNode)
			{
				return pTmp;
			}
			pTmp = pNode;
		}
		return NULL;
	}

	bool TexturePathFind(std::string &inFileName, std::string& outFileName)
	{
		Interface* ip = GetCOREInterface();
		if (!ip)
		{
			return false;
		}
		std::string strTestFile;
		//检查此刻路径是否有效
		if (PathFileExists(inFileName.c_str()))
		{
			outFileName = inFileName;
			return true;
		}

		//检查模型目录下
		std::string strModelPath = ip->GetCurFilePath();
		if (strModelPath.length())
		{
			strTestFile = strModelPath + std::string("\\") + inFileName;
			if (PathFileExists(strTestFile.c_str()))
			{
				outFileName = strTestFile;
				return true;
			}
		}
		//检查3DMAX路径下
		for (int i = 0; i < ip->GetCurAssetDirCount(MaxSDK::AssetManagement::kBitmapAsset ); i++)
		{
			strTestFile = ip->GetCurAssetDir(i, MaxSDK::AssetManagement::kBitmapAsset);
			strTestFile += std::string("\\") + inFileName;
			if (PathFileExists(strTestFile.c_str()))
			{
				outFileName = strTestFile;
				return true;
			}
		}

		return false;
	}

	std::string GetFileExt(std::string& inFileName)
	{
		std::string outExt;
		int iStrLength = inFileName.length();
		for (int i = 0; i < iStrLength; i++)
		{
			if ('.' == inFileName[iStrLength - i - 1])
			{
				if (0 == i)
				{
					return outExt;
				}

				outExt = inFileName.substr(iStrLength - i, i);
				return outExt;
			}
		}
		return outExt;
	}

	std::string GetFileName(std::string& inFileName)
	{
		std::string outFileName;
		int iStrLength = inFileName.length();
		int iEndPos = iStrLength - 1;
		for (int i = 0; i < iStrLength; i++)
		{
			if ('.' == inFileName[iStrLength - i - 1])
			{
				iEndPos = iStrLength - i - 1;
			}

		}
		return outFileName;
	} 

	Modifier * FindModifier(INode *pINode, Class_ID cid){

		Object * pObject = pINode->GetObjectRef();

		if(pObject == 0) return 0;

		// 循环检测所有的DerivedObject
		while(pObject->SuperClassID() == GEN_DERIVOB_CLASS_ID)
		{
			IDerivedObject * pDerivedObject = static_cast<IDerivedObject *>(pObject);
			for(int stackId = 0; stackId < pDerivedObject->NumModifiers(); stackId++)
			{
				Modifier * pModifier = pDerivedObject->GetModifier(stackId);
				//检测ClassID是不是Skin修改器
				if(pModifier->ClassID() == cid) 
				{
					return pModifier;;//return (ISkin*) pModifier->GetInterface(I_SKIN);
				}
			}
			//下一//个Derived Object    
			pObject = pDerivedObject->GetObjRef();
		}
		return 0;
	}

	struct NullView : public View
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
		BOOL bNeedDelete = FALSE;
		ObjectState os = pNode->EvalWorldState(iMaxTime);
		Object* pObj = os.obj;
		if (pObj)
		{
			TriObject* triObject= (TriObject*)pObj->ConvertToType(iMaxTime, Class_ID(TRIOBJ_CLASS_ID, 0));
			if (!triObject)
			{
				assert(0);
			}
			if (triObject)
			{
				GeomObject* pGeoObj = (GeomObject*)pObj;
				Mesh* pMesh = pGeoObj->GetRenderMesh(iMaxTime, pNode, view, bNeedDelete);
				return pMesh;
			}
		}

		return NULL;
	}
	
	void Matrix3ToXMatrix(const Matrix3& m3, XMatrix& xm)
	{
		for (int i = 0; i < 4; i++)
		{
			Point3 pt = m3[i];
			xm.f[i][0] = pt.x;
			xm.f[i][1] = pt.y;
			xm.f[i][2] = pt.z;
			xm.f[i][3] = ((i == 3) ? 1.0f : 0.0f);
		}
	}

	void DecompMatrix3(Matrix3 mat, XVector3& tran, XVector3& scal, XQuaternion& quat)
	{
		AffineParts ap;
		decomp_affine(mat, &ap);
		tran.x = ap.t.x;
		tran.y = ap.t.y;
		tran.z = ap.t.z;

		scal.x = ap.k.x;
		scal.y = ap.k.y;
		scal.z = ap.k.z;

		quat.x = -ap.q.x;
		quat.y = -ap.q.y;
		quat.z = -ap.q.z;
		quat.w = ap.q.w;
	}
}