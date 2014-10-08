/**************************************************************************
/*		FileName: XRenderScene.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/10/06 23:06:01
/*		CopyRight: yc 
/*************************************************************************/

#include "XRenderScene.h"

void XRenderScene::AddToRenderList(XRenderEntity* pRenderEntity)
{
	if (!pRenderEntity)
	{
		return;
	}

	all_entities.push_back(pRenderEntity);
}

void XRenderScene::ClassifyRenderList()
{
	XFrustum frustum;
	shadow_cast_entities.clear();
	shadow_recv_entities.clear();
	opaque_entities.clear();
	translucent_entities.clear();
	post_progress_entities.clear();
	occlusion_entities.clear();
	forground_entities.clear();
	normal_entities.clear();
	background_entities.clear();
	for (int i = 0; i < all_entities.size(); i++)
	{
		XRenderEntity* entity = all_entities[i];
		if (!frustum.DetectAABB(entity->GetAABB()))
		{
			continue;
		}
		xuint32 render_flag = entity->GetRenderFlag();
		if (render_flag & X_RENDER_OPAQUE)
			opaque_entities.push_back(entity);
		if(render_flag & X_RENDER_TRANSPARENCY)
			translucent_entities.push_back(entity);
		if(render_flag & X_RENDER_POST_PROCESS)
			post_progress_entities.push_back(entity);
		if(render_flag & X_ACTOR_OCCLUSION)
			occlusion_entities.push_back(entity);
		if(render_flag & X_ACTOR_SHADOW_CAST)
			shadow_cast_entities.push_back(entity);
		if(render_flag & X_ACTOR_SHADOW_RECV)
			shadow_recv_entities.push_back(entity);
		if(render_flag & X_ACTOR_BACKGROUND)
			background_entities.push_back(entity);
		if(render_flag & X_ACTOR_FOREGROUND)
			background_entities.push_back(entity);
		if(render_flag & X_ACTOR_NORMAL)
			normal_entities.push_back(entity);
	}
}