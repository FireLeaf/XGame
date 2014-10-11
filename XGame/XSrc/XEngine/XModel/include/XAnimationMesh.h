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
public:
	typedef stdext::hash_map<std::string, XSkinMesh*> NameToSkinMap;
public:
	
protected:
	XSkeltonFrame* ptr_skelton_frame;//�������
	NameToSkinMap map_name_to_skin;//���ֵ���Ƥ
	//std::vector<XMotionMgr*> montion_ptr_array;//���ֶ����ļ���
	XActionMgr* ptr_play_action;//�ؼ�֡����
	XEmotionMgr* ptr_play_emotion;//����֮��Ķ���
};

#endif // XAnimationMesh