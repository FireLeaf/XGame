/**************************************************************************
/*		FileName: XRenderMonitor.h
/*		FileDesc: ������Ⱦ��
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/02 20/57/09
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XRENDERMONITOR__H
#define __XRENDERMONITOR__H

#include "XRenderEntity.h"

enum X_RENDER_FLAG
{
	X_RENDER_OPAQUE = 1 << 0,//��͸����
	X_RENDER_TRANSPARENCY = 1 << 1,//͸���ģ���Ҫblend��
	X_RENDER_POST_PROCESS = 1 << 2,//��Ҫ����post process��
	X_RENDER_SHADOW = 1 << 3,//��Ҫ������Ӱ��
};

class XRenderMonitor
{
public:
	void AddToRenderList(XRenderEntity* pRenderEntity);
	void MonitorRender();
public:
	//�����
	XStl::vector<XRenderEntity*> all_entities;//���е���Ⱦ��
	XStl::vector<XRenderEntity*> shadow_cast_entities;//��ӰͶ����
	XStl::vector<XRenderEntity*> shadow_recv_entities;//��Ӱ������
	XStl::vector<XRenderEntity*> opaque_entities;//��͸����Ⱦ��
	XStl::vector<XRenderEntity*> translucent_entities;//��͸��
	XStl::vector<XRenderEntity*> post_progress_entities;//post progress

	//������
	XStl::vector<XRenderEntity*> ui_entities;//UI��
};

#endif // XRenderMonitor 