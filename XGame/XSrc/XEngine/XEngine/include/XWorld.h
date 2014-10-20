/**************************************************************************
/*		FileName: XWorld.h
/*		FileDesc: 世界
/*		FileAuthor: YangCao
/*		CreateTime: 2014/10/06 22:37:01
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XWORLD__H
#define __XWORLD__H

#include "XType.h"
#include "XMath.h"
#include "XVector.h"
#include "XRenderScene.h"
#include "XTerrain.h"

class XRenderEntity;

class XWorld //: public XRenderEntity
{
public:
	enum X_WORLD_ENTITY
	{
		WE_TERRAIN,//地形
		WE_WATER,//水体
		WE_COVEX_HULL,//凸包
		WE_CLUSTER,//群聚生物
		WE_DYNAMIC_OBJ,//动态物体,蒙皮之类的
		WE_NATURE_EFFECT,//自然景观，例如下雪、下雨、以及各种特效
		WE_VEGETATION,//小型植被
		WE_NUM,
	};

	class XWorldLoaderEntity// : public XRenderEntity
	{
	public:
		virtual bool Init(XWorld* world, const char* entity_path){ ptr_world = world; return true;}
		virtual void Release(){}
		virtual void Tick(const XVector3& center, xint32 time_delta){}
		virtual void Render(XRenderScene* ptr_render_scene){}
	protected:
		XWorld* ptr_world;
	};

	class XTerrainLoaderEntity : public XWorldLoaderEntity
	{
	public:
		XTerrainLoaderEntity():ptr_terrain(NULL){}
		~XTerrainLoaderEntity(){}

		virtual bool Init(XWorld* world, const char* entity_path)
		{
			Release();
			ptr_terrain = new XTerrain();
			if (!ptr_terrain || !ptr_terrain->Init(entity_path))
			{
				return false;
			}
			
			return true;
		}
		virtual void Release()
		{
			if (ptr_terrain)
			{
				delete ptr_terrain;
			}
		}
		
		virtual void Tick(const XVector3& center, xint32 time_delta)
		{
			if (ptr_terrain)
			{
// 				XAnchorPos cp;
// 				cp.x = (int)center.x;
// 				cp.y = (int)center.y;
// 				cp.z = (int)center.z;
				ptr_terrain->Tick(center, time_delta);
			}
		}

		virtual void Render(XRenderScene* ptr_render_scene)
		{
			if (ptr_terrain)
			{
				ptr_render_scene->AddToRenderList(ptr_terrain);
			}
		}
	protected:
		XTerrain* ptr_terrain;
	};
public:
	XWorld();
	virtual ~XWorld(){}
public:
	virtual bool Init(const char* world_path, const XVector3& center);
	virtual void Release();
	virtual void Tick(xint32 time_delta);
	virtual void Render(XRenderScene* ptr_render_scene);
protected:
	XWorldLoaderEntity* CreateWorldLoaderEntity(int type);
public:
	const XVector3& GetWorldCenter(){return world_center;}
	void SetWorldCenter(const XVector3& v){world_center = v;}
protected:
	XWorldLoaderEntity* entity_loaders[WE_NUM];

	XVector3 world_center;
};

#endif // XWorld