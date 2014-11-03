/**************************************************************************
/*		FileName: MeshExporterUtil.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/05/28 23/33/17
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __MESHEXPORTERUTIL__H
#define __MESHEXPORTERUTIL__H

#include "cs/BipedApi.h"
#include "cs/BIPEXP.H"
#include "XMatrix.h"
#include "matrix3.h"

namespace MeshExporterUtil
{
	enum
	{
		BONE_INCLUDE_DUMMY = 1 << 0,
	};

	bool IsBone(INode* pNode, int flag = 0);
	
	INode* GetRootBoneNode(int flag = 0);

	INode* GetBoneRootNode(INode* pBoneNode, int flag = 0);

	bool TexturePathFind(std::string& inFileName, std::string& outFileName);

	std::string GetFileExt(std::string& inFileName);//获取文件的后缀

	Modifier * FindModifier(INode *pINode, Class_ID cid);

	Mesh* GetMesh(INode* pNode, int iMaxTime);

	void Matrix3ToXMatrix(const Matrix3& m3, XMatrix& xm);
}

#endif // MeshExporterUtil