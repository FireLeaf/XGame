/**************************************************************************
/*		FileName: XSkeltonFrame.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/10/27 21:01:37
/*		CopyRight: yc 
/*************************************************************************/
#include "XMatrix.h"
#include "XSkeltonFrame.h"

void XSkeltonFrame::Tick(xuint32 time_delta)
{
	if(bone_ptr_array.size())
		UpdateSkelton(bone_ptr_array[0], NULL);
}

void XSkeltonFrame::UpdateSkelton(XBone* ptr_bone, XBone* ptr_parent_bone)
{
	if (!ptr_bone)
	{
		return;
	}

	if (ptr_parent_bone)
	{
		ptr_bone->abs_matrix = ptr_bone->tran_matrix * ptr_parent_bone->abs_matrix;
	}
	else
	{
		ptr_bone->abs_matrix = ptr_bone->tran_matrix;
	}

	//更新兄弟
	if(-1 != ptr_bone->next_sbling)
		UpdateSkelton(bone_ptr_array[ptr_bone->next_sbling], ptr_parent_bone);
	//更新孩子
	if(-1 != ptr_bone->first_child_index)
		UpdateSkelton(bone_ptr_array[ptr_bone->first_child_index], ptr_bone);
}

void XSkeltonFrame::GetBlendMatrix(XMatrix* blend_matrices, int max_matrix)
{
	for (int i = 0; i < max_matrix && i < (int)bone_ptr_array.size(); i++)
	{
		blend_matrices[i] = bone_ptr_array[i]->inv_offset_matrix * bone_ptr_array[i]->abs_matrix;
	}
}

XBone* XSkeltonFrame::FindBone(const char* name)
{
	for (int i = 0; i < (int)bone_ptr_array.size(); i++)
	{
		if (0 == strcmp(name, bone_ptr_array[i]->name.c_str()))
		{
			return bone_ptr_array[i];
		}
	}
	return NULL;
}