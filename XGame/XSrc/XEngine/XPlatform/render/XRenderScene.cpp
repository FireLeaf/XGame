/**************************************************************************
/*		FileName: XRenderScene.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/10/06 23:06:01
/*		CopyRight: yc 
/*************************************************************************/

#include "XRenderScene.h"
#include "XRenderEntity.h"

XSceneDesc::XSceneDesc()
{
	ResetDesc();
}

XSceneDesc::~XSceneDesc()
{
	if(ptr_asset_depth_stencil) { delete ptr_asset_depth_stencil; ptr_asset_depth_stencil = NULL;}
	for (int i = 0; i < MAX_LIGHT_COUNT; i++)
	{
		if(light[i]) {delete light[i]; light[i] = NULL;};
	}
}

void XSceneDesc::ResetDesc()
{
	render_target_count = 0;
	for (int i = 0; i < MAX_RENDER_TARGET; i++)
	{
		ptr_asset_render_target[i] = NULL;
	}

	ptr_asset_depth_stencil = NULL;
	clear_color = 0xff0000ff;
	clear_buffer_flag = X_CLEAR_TARGET | X_CLEAR_ZBUFFER;
	for (int i = 0; i < MAX_LIGHT_COUNT; i++)
	{
		light[i] = NULL;
		build_shadow[i] = false;
	}
}

void XRenderScene::AddToRenderList(XRenderEntity* pRenderEntity)
{
	if (!pRenderEntity)
	{
		return;
	}

	all_entities.push_back(pRenderEntity);
}

void XRenderScene::BeginScene()
{
	all_entities.clear();
}

void XRenderScene::EndScene()
{

}

void XRenderScene::Clear()
{
	shadow_cast_entities.clear();
	shadow_recv_entities.clear();
	opaque_entities.clear();
	translucent_entities.clear();
	post_progress_entities.clear();
	occlusion_entities.clear();
	forground_entities.clear();
	normal_entities.clear();
	background_entities.clear();
}

void XRenderScene::ClassifyRenderList()
{
	XFrustum frustum;
	Clear();

	for (int i = 0; i < (int)all_entities.size(); i++)
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