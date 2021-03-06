/**************************************************************************
/*		FileName: XRenderScene.h
/*		FileDesc: 场景管理
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/24 22:00:14
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XRENDERSCENE__H
#define __XRENDERSCENE__H

#include "XType.h"
#include "XMathUtil.h"
#include "XAsset.h"
#include "XCamera.h"

class XRenderEntity;

enum X_CLEAR_FLAG
{
	X_CLEAR_TARGET = 0x00000001l,  /* Clear target surface */
	X_CLEAR_ZBUFFER = 0x00000002l,  /* Clear target z buffer */
	X_CLEAR_STENCIL = 0x00000004l,  /* Clear stencil planes */
};



struct XLight
{
	XVector4 ambient;
	XVector4 diffuse;
	XVector4 specular;
	XVector4 emmisive;
};

struct XDirLight : public XLight
{
	XVector3 dir;
};

struct XPointLight : public XLight
{
	XVector3 pos;
};

struct XSpotLight : public XLight 
{

};


struct XSceneDesc
{
// public:
// 	XCamera* GetCameraPtr(){return &camera;}
// 	const XCamera& GetCamera(){return camera;}
// 	XViewPort* GetViewportPtr(){return &view_port;}
// 	const XViewPort& GetViewPort(){return view_port;}
// 	void SetCamera(const XCamera& cam){camera = cam;}
// 	void SetViewPort(const XViewPort& vp){view_port = vp;}
// protected:
	enum
	{
		MAX_RENDER_TARGET = 4,
		MAX_LIGHT_COUNT = 1,
	};
	XCamera camera;
	XViewPort view_port;
	int render_target_count;
	XAsset* ptr_asset_render_target[MAX_RENDER_TARGET];
	XAsset* ptr_asset_depth_stencil;//这里直接用的render_target保存的
	xint32 clear_color;
	xulong clear_buffer_flag;
//	bool is_device_target;
	XLight* light[MAX_LIGHT_COUNT];
	bool build_shadow[MAX_LIGHT_COUNT];

	XSceneDesc();
	~XSceneDesc();
	void ResetDesc();
};

class XRenderScene
{
public:
	void AddToRenderList(XRenderEntity* pRenderEntity);
	void ClassifyRenderList();
	//void SetViewPort(const XViewPort& vp){ptr_secne_desc->view_port = vp;/*secne_desc.SetViewPort(vp);*/}
	//void SetCamera(const XCamera& cam){ptr_secne_desc->camera = cam;/*secne_desc.SetCamera(cam);*/}
	void SetSceneDesc(XSceneDesc* desc){ptr_secne_desc = desc;}
	XSceneDesc* GetSceneDesc(){return ptr_secne_desc;}
	void Clear();
	void BeginScene();
	void EndScene();
public:
	//隔离层
	XStl::vector<XRenderEntity*> all_entities;//所有的渲染体
	XStl::vector<XRenderEntity*> shadow_cast_entities;//阴影投射者
	XStl::vector<XRenderEntity*> shadow_recv_entities;//阴影接受者
	XStl::vector<XRenderEntity*> opaque_entities;//不透明渲染体
	XStl::vector<XRenderEntity*> translucent_entities;//半透明
	XStl::vector<XRenderEntity*> post_progress_entities;//post progress
	
	//功能层
	XStl::vector<XRenderEntity*> occlusion_entities;//遮挡层，用于遮挡剔除

	//辅助层
	XStl::vector<XRenderEntity*> forground_entities;//前景层，包括UI之类的
	XStl::vector<XRenderEntity*> normal_entities;//普通层
	XStl::vector<XRenderEntity*> background_entities;//背景层，包括远景，天空盒之类
protected:
	XSceneDesc* ptr_secne_desc;
};

#endif // XRenderScene