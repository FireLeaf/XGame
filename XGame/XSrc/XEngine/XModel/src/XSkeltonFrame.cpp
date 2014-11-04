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

}

void XSkeltonFrame::UpdateSkelton(XBone* ptr_skelton, XBone* ptr_parent_skelton)
{
	if (!ptr_skelton)
	{
		return;
	}

	if (ptr_parent_skelton)
	{
		ptr_skelton->abs_matrix = ptr_skelton->init_matrix * ptr_parent_skelton->abs_matrix;
	}
	else
	{
		ptr_skelton->abs_matrix = ptr_skelton->init_matrix;
	}

	//更新兄弟
	if(-1 != ptr_skelton->next_sbling)
		UpdateSkelton(bone_ptr_array[ptr_skelton->next_sbling], ptr_parent_skelton);
	//更新孩子
	if(-1 != ptr_skelton->first_child_index)
		UpdateSkelton(bone_ptr_array[ptr_skelton->first_child_index], ptr_skelton);
}