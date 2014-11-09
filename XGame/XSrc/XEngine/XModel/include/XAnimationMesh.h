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

class XAnimationMesh : public XRenderEntity//动画网格
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
	bool BindSkin(XSkinMesh* skin_mesh);//将蒙皮绑定到骨骼中
protected:
	XSkeltonFrame* ptr_skelton_frame;//骨骼框架
	NameToSkinMap map_name_to_skin;//名字到蒙皮
	XMateriaEntity* material_entity;//通用的材质实体，蒙皮可以有自己的材质
	SkinMeshVector vector_skin_mesh;
	//std::vector<XMotionMgr*> montion_ptr_array;//各种动作的集合
	XActionMgr* ptr_play_action;//关键帧动作
	std::list<XActionMgr*> list_queue_action;//后面的
	XEmotionMgr* ptr_play_emotion;//表情之类的动作
	//
	XMatrix* arr_blend_matrix;//融合之后的矩阵

	//模型联系 
	std::vector<XAnimationMesh*> child_model_array;
};

#endif // XAnimationMesh