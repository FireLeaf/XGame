/**************************************************************************
/*		FileName: XWorld.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/10/06 22:37:19
/*		CopyRight: yc 
/*************************************************************************/

#include "XWorld.h"
#include "XRenderEntity.h"

XWorld::XWorld() : world_center(0.0f, 0.0f, 0.0f)
{
	for(int i = 0; i < WE_NUM; i++)
	{
		entity_loaders[i] = NULL;
	}
}

void XWorld::Release()
{
	for (int i = 0; i < WE_NUM; i++)
	{
		if (entity_loaders[i])
		{
			entity_loaders[i]->Release();
			delete entity_loaders[i];
			entity_loaders[i] = NULL;
		}
	}
}

XWorld::XWorldLoaderEntity* XWorld::CreateWorldLoaderEntity(int type)
{
	switch(type)
	{
	case WE_TERRAIN:
		return new XTerrainLoaderEntity();
		break;
	default:
		break;
	}
	return NULL;
}

bool XWorld::Init(const char* world_path, const XVector3& center)
{
	Release();
	SetWorldCenter(center);
	for (int i = 0 ;i < WE_NUM; i++)
	{
		entity_loaders[i] = CreateWorldLoaderEntity(i);
		if(!entity_loaders[i] || !entity_loaders[i]->Init(this, world_path))
		{
			return false;
		}
	}
	return true;
}

void XWorld::Tick(xint32 time_delta)
{
	for (int i = 0 ;i < WE_NUM; i++)
	{
		if(entity_loaders[i])
		{
			entity_loaders[i]->Tick(world_center, time_delta);
		}
	}
}

void XWorld::Render(XRenderScene* ptr_render_scene)
{
	for (int i = 0; i < WE_NUM; i++)
	{
		if (entity_loaders[i])
		{
			entity_loaders[i]->Render(ptr_render_scene);
		}
	}
}