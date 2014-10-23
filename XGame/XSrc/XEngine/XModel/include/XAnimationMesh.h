/**************************************************************************
/*		FileName: XAnimationMesh.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/10/07 1:57:35
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XANIMATIONMESH__H
#define __XANIMATIONMESH__H

class XAnimationMesh : public XRenderEntity//动画网格
{
public:
	typedef stdext::hash_map<std::string, XSkinMesh*> NameToSkinMap;
public:
	void Tick(xuint32 time_delta);
protected:
	XSkeltonFrame* ptr_skelton_frame;//骨骼框架
	NameToSkinMap map_name_to_skin;//名字到蒙皮
	//std::vector<XMotionMgr*> montion_ptr_array;//各种动作的集合
	XActionMgr* ptr_play_action;//关键帧动作
	XEmotionMgr* ptr_play_emotion;//表情之类的动作

	//
	XMatrix* arr_blend_matrix;//融合之后的矩阵
};

#endif // XAnimationMesh