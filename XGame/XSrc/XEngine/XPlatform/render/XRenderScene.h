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
#include "XMath.h"

class XRenderEntity;

enum X_CLEAR_BUFFFER
{
	X_CLEAR_TARGET = 0x00000001l,  /* Clear target surface */
	X_CLEAR_ZBUFFER = 0x00000002l,  /* Clear target z buffer */
	X_CLEAR_STENCIL = 0x00000004l,  /* Clear stencil planes */
};

enum X_RENDER_FLAG
{
	X_RENDER_OPAQUE = 1 << 0,//不透明的
	X_RENDER_TRANSPARENCY = 1 << 1,//透明的，需要blend的
	X_RENDER_POST_PROCESS = 1 << 2,//需要参与post process的
	X_RENDER_SHADOW = 1 << 3,//需要参与阴影的
};

enum X_RENDER_ACTOR
{
	X_ACTOR_NORMAL = 1 << 16,
	X_ACTOR_SHADOW_CAST = 1  << 17,
	X_ACTOR_SHADOW_RECV = 1 << 18,
	X_ACTOR_TRANSLUCENT = 1 << 19,
	X_ACTOR_ALPHATEST = 1 << 20,
	X_ACTOR_POST_PROGRESS = 1 << 21,
	X_ACTOR_FOREGROUND = 1 << 22,
	X_ACTOR_BACKGROUND = 1 << 23,
	X_ACTOR_OCCLUSION = 1 << 24,
};

class XCamera
{
public:
	void SetEye(const XVector3& eye){dirty = true; this->eye = eye;}
	void SetUp(const XVector3& up){dirty = true; this->up = up;}
	void SetAt(const XVector3& at){dirty = true; this->at = at;}
	
	void CalcViewMatrix(){XMathMatrixLookAtLH(mat_view, eye, up, at);dirty = false;}
	XMatrix GetViewMatrix(){if(dirty){CalcViewMatrix();} return mat_view;}
protected:
	XMatrix mat_view;
	XVector3 eye;
	XVector3 up;
	XVector3 at;
	bool dirty;
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

struct XViewPort
{
public:
	XMatrix GetProjMatrix(){XMathMatrixPerspectiveFovLH(mat_proj, fov, (float)width / (float)height, z_near, z_far); return mat_proj;}
public:
	xuint32		width;
	xuint32		height;       /* Viewport Dimensions */
	float		z_near;         /* Min/max of clip Volume */
	float		z_far;
	float		fov;
	XMatrix		mat_proj;
};

struct XFrustum
{
public:
	//视景体裁剪
	bool DetectPoint(const XVector3 &point);
	bool DetectCuboid(const XVector3 &center,float width,float length,float height);
	bool DetectCube(const XVector3 &center,float size);
	bool DetectRectangle(const XVector3 &center,float width,float length);
	bool DetectSquare(const D3DXVECTOR3 &center,float size);
	bool DetectAABB(const XAABB &aabb);
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
	XAsset* ptr_asset_depth_stencil;
	xint32 clear_color;
	xulong clear_buffer_flag;
	bool is_device_target;
	XLight light[MAX_LIGHT_COUNT];
	bool build_shadow[MAX_LIGHT_COUNT];
};

class XRenderScene
{
public:
	void AddToRenderList(XRenderEntity* pRenderEntity);
	void ClassifyRenderList();
	void SetViewPort(const XViewPort& vp){secne_desc.view_port = vp;/*secne_desc.SetViewPort(vp);*/}
	void SetCamera()(const XCamera& cam){secne_desc.camera = cam;/*secne_desc.SetCamera(cam);*/}
	XSceneDesc& GetSceneDesc(){return secne_desc;}
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
	XSceneDesc secne_desc;
};

#endif // XRenderScene