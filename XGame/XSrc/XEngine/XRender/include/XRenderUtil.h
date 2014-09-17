/**************************************************************************
/*		FileName: XRenderUtil.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/16 22:31:59
/*		CopyRight: yc 
/*************************************************************************/

namespace RenderUtil
{
	int GetTexFormat(int format);
	int GetTexStride(int format);
	int GetAssetManageType(bool bDynamic);
	int GetIndexBits(int bits);
	int GetValueTypeSize(int declType);
	int GetDeclType(int declType);
	int GetDeclUsage(int declUsage);

}