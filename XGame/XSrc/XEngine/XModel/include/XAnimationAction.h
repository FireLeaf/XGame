/**************************************************************************
/*		FileName: XAnimationAction.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/11/04 22:25:23
/*		CopyRight: yc 
/*************************************************************************/
#ifndef __XANIMATIONACTION__H
#define __XANIMATIONACTION__H

#include "XAnimationKey.h"

struct XAnimActionClip//һ��С����Ƭ��
{
	xuint32 start;//����Ƭ����ʼʱ��
	xuint32 length;//����Ƭ�εĳ���
	std::string anim_name;
	int play_count;//���Ŵ�����-1��ʾ����ѭ��
	std::vector<XAnimKeySet> anim_keyset_array;//ÿ����ͷ����
};

struct XAnimAction//һ�������������ɺü���clip��ɵ�
{
	bool enable;
	std::string action_name;
	xuint32 length;//����ȫ��
	std::vector<XAnimActionClip*> action_clip_array;
};

class XMotionMgr
{

};

#define MAX_SYNC_ACTION 8

class XActionMgr : public XMotionMgr//�ؼ�֡����
{
public:
	void ProcMotion(xuint32 time_delta, XMatrix* blend_matrix)
	{
		for (int i = 0; i < MAX_SYNC_ACTION; i++)
		{
			if (ptr_actions[i] && ptr_actions[i]->enable &&actions_weight[i] > 0.0f)
			{

			}
		}
	}
protected:
	XAnimAction* ptr_actions[MAX_SYNC_ACTION];//ֱ��ȫ���������������˲ű�־�������������
	float actions_weight[MAX_SYNC_ACTION];
	xuint32 time_duration;//��ǰ���Ŷ�����ʱ��
};

struct XEmotionKey//����
{
	//std::string emotion_name;
	//std::string skin_name;//ָ���Ƥ�����֣����ֱ��ָ���ض���Ƥ��
	//XVector3
};

class XEmotionMgr : public XMotionMgr//�����˵������
{

};


class XAnimationController
{
public:
	XAnimationController(){}
protected:

};

#endif // XAnimationAction
