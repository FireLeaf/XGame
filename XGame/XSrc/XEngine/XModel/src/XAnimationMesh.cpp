/**************************************************************************
/*		FileName: XAnimationMesh.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/10/27 21:21:42
/*		CopyRight: yc 
/*************************************************************************/

#include "XAnimationMesh.h"

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
		ptr_skelton_frame->Tick(time_delta);
	}

	for ()
	{
	}
}