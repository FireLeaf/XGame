/**************************************************************************
/*		FileName: XRenderScene.h
/*		FileDesc: ��������
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

#endif // XRenderScene