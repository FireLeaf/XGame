/**************************************************************************
/*		FileName: XAnimationMesh.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/10/07 1:57:35
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XANIMATIONMESH__H
#define __XANIMATIONMESH__H

class XAnimationMesh : public XRenderEntity//��������
{

protected:
	stdext::hash_map<std::string, XSkinMesh*> ;//���ֵ���Ƥ
};

#endif // XAnimationMesh