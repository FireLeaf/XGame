/**************************************************************************
/*		FileName: XAnimationMesh.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/10/07 1:57:35
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XANIMATIONMESH__H
#define __XANIMATIONMESH__H

#include "XRenderEntity.h"
class XSkinMesh;
class XSkeltonFrame;
class XActionMgr;
class XEmotionMgr;

enum X_ANIMATION_FLAG
{

};

class XAnimationMesh : public XRenderEntity//��������
{
public:
	typedef stdext::hash_map<std::string, XSkinMesh*> NameToSkinMap;
public:
	XAnimationMesh();

	void Tick(xuint32 time_delta, int flag);
	virtual void Render(XRII* rii, XRenderArgs* args);

public:
	void PlayAnimationAction();
protected:
	XSkeltonFrame* ptr_skelton_frame;//�������
	NameToSkinMap map_name_to_skin;//���ֵ���Ƥ
	//std::vector<XMotionMgr*> montion_ptr_array;//���ֶ����ļ���
	XActionMgr* ptr_play_action;//�ؼ�֡����
	XEmotionMgr* ptr_play_emotion;//����֮��Ķ���
	//
	XMatrix* arr_blend_matrix;//�ں�֮��ľ���

	//ģ����ϵ 
	std::vector<XAnimationMesh*> child_model_array;
};

#endif // XAnimationMesh