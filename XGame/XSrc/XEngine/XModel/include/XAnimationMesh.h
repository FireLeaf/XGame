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
	typedef std::vector<XSkinMesh*> SkinMeshVector;
public:
	XAnimationMesh();

	void Tick(xuint32 time_delta, int flag);
	virtual void Render(XRII* rii, XRenderArgs* args);

public:

// 	void AddSkin(const char* skin_name, XSkinMesh* new_skin);
// 	const XSkinMesh* GetSkin(const char* skin_name) const;
// 	void ReplaceSkinMesh(XSkinMesh* old_skin, XSkinMesh* new_skin);
// 	void ReplaceSkinMesh(const char* skin_name, XSkinMesh* new_skin);
// 	void RemoveSkinMesh(XSkinMesh* skin);

	int AddSkin(XSkinMesh* new_skin);
	const XSkinMesh* GetSkin(int skin_index) const;
	bool ReplaceSkinMesh(XSkinMesh* old_skin, XSkinMesh* new_skin);
	bool ReplaceSkinMesh(int skin_index, XSkinMesh* new_skin);
	bool RemoveSkinMesh(XSkinMesh* skin);
	bool RemoveSkinMesh(int skin_index);

	void PlayAnimationAction(const char* action_name);

protected:
	bool BindSkin(XSkinMesh* skin_mesh);//����Ƥ�󶨵�������
protected:
	XSkeltonFrame* ptr_skelton_frame;//�������
	NameToSkinMap map_name_to_skin;//���ֵ���Ƥ
	XMateriaEntity* material_entity;//ͨ�õĲ���ʵ�壬��Ƥ�������Լ��Ĳ���
	SkinMeshVector vector_skin_mesh;
	//std::vector<XMotionMgr*> montion_ptr_array;//���ֶ����ļ���
	XActionMgr* ptr_play_action;//�ؼ�֡����
	std::list<XActionMgr*> list_queue_action;//�����
	XEmotionMgr* ptr_play_emotion;//����֮��Ķ���
	//
	XMatrix* arr_blend_matrix;//�ں�֮��ľ���

	//ģ����ϵ 
	std::vector<XAnimationMesh*> child_model_array;
};

#endif // XAnimationMesh