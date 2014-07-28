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

}