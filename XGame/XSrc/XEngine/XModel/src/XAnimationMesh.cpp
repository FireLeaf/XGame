/**************************************************************************
/*		FileName: XAnimationMesh.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/10/27 21:21:42
/*		CopyRight: yc 
/*************************************************************************/

#include "XAnimationMesh.h"
#include "XSkinMesh.h"
#include "XAnimationAction.h"
#include "XSkeltonFrame.h"

XAnimationMesh::XAnimationMesh()
{
	ptr_skelton_frame = NULL;
	ptr_play_action = NULL;
	ptr_play_emotion = NULL;
	arr_blend_matrix = NULL;
}

void XAnimationMesh::Tick(xuint32 time_delta, int flag)
{
	if (ptr_skelton_frame)
	{
		if (ptr_play_action)
		{
			int ret = ptr_play_action->ProcMotion(time_delta);
			if (ANIM_ACTION_OVER == ret)
			{
				delete ptr_play_action;
				if (list_queue_action.size())
				{
					ptr_play_action = (XActionMgr*)list_queue_action->begin();
					list_queue_action->pop_front();
				}
			}
		}
		ptr_skelton_frame->Tick(time_delta);
		ptr_skelton_frame->GetBlendMatrix(arr_blend_matrix, ptr_skelton_frame->GetBoneCount());
	}

// 	for (int i = 0; i < vector_skin_mesh.size(); i++)
// 	{
// 		if (vector_skin_mesh[i])
// 		{
// 			vector_skin_mesh[i]->
// 		}
// 	}
}

void XAnimationMesh::Render(XRII* rii, XRenderArgs* args)
{
	if (material_entity)
	{
		
	}

	for (int i = 0; i < (int)vector_skin_mesh.size(); i++)
	{
		vector_skin_mesh[i]->Render(rii, args);
	}
}

bool XAnimationMesh::BindSkin(XSkinMesh* skin_mesh)
{
	if(!skin_mesh) return false;
	const std::vector<std::string>& bind_bone_names = skin_mesh->GetBindBoneNames();
	for (int i = 0; i < (int)bind_bone_names.size(); i++)
	{
		if(!ptr_skelton_frame->FindBone(bind_bone_names[i].c_str()))
		{
			
			return false;
		}
	}
	return true;
}