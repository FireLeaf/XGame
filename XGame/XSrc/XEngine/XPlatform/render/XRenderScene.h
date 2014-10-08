/**************************************************************************
/*		FileName: XRenderScene.h
/*		FileDesc: ��������
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
	X_RENDER_OPAQUE = 1 << 0,//��͸����
	X_RENDER_TRANSPARENCY = 1 << 1,//͸���ģ���Ҫblend��
	X_RENDER_POST_PROCESS = 1 << 2,//��Ҫ����post process��
	X_RENDER_SHADOW = 1 << 3,//��Ҫ������Ӱ��
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
	//�Ӿ���ü�
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
	//�����
	XStl::vector<XRenderEntity*> all_entities;//���е���Ⱦ��
	XStl::vector<XRenderEntity*> shadow_cast_entities;//��ӰͶ����
	XStl::vector<XRenderEntity*> shadow_recv_entities;//��Ӱ������
	XStl::vector<XRenderEntity*> opaque_entities;//��͸����Ⱦ��
	XStl::vector<XRenderEntity*> translucent_entities;//��͸��
	XStl::vector<XRenderEntity*> post_progress_entities;//post progress
	
	//���ܲ�
	XStl::vector<XRenderEntity*> occlusion_entities;//�ڵ��㣬�����ڵ��޳�

	//������
	XStl::vector<XRenderEntity*> forground_entities;//ǰ���㣬����UI֮���
	XStl::vector<XRenderEntity*> normal_entities;//��ͨ��
	XStl::vector<XRenderEntity*> background_entities;//�����㣬����Զ������պ�֮��
protected:
	XSceneDesc secne_desc;
};

#endif // XRenderScene