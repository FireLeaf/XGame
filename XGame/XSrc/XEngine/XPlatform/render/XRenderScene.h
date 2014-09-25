/**************************************************************************
/*		FileName: XRenderScene.h
/*		FileDesc: 场景管理
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/24 22:00:14
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XRENDERSCENE__H
#define __XRENDERSCENE__H

class XRenderScene
{
public:
		virtual void AddToRenderList(XRenderEntity* pRenderEntity) = 0;
public:
	//隔离层
	XStl::vector<XRenderEntity*> all_entities;//所有的渲染体
	XStl::vector<XRenderEntity*> shadow_cast_entities;//阴影投射者
	XStl::vector<XRenderEntity*> shadow_recv_entities;//阴影接受者
	XStl::vector<XRenderEntity*> opaque_entities;//不透明渲染体
	XStl::vector<XRenderEntity*> translucent_entities;//半透明
	XStl::vector<XRenderEntity*> post_progress_entities;//post progress

	//辅助层
	XStl::vector<XRenderEntity*> ui_entities;//UI层
};

#endif // XRenderScene