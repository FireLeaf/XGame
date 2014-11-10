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
#include "XAnimationEvent.h"
#include "XSkeltonFrame.h"

struct XAnimActionClip//һ��С����Ƭ��
{
	xuint32 start;//����Ƭ����ʼʱ��
	xuint32 length;//����Ƭ�εĳ���
	std::string anim_name;
	int frame_count;//֡��
	std::vector<XAnimKeySet*> anim_keyset_array;//ÿ����ͷ����
	void ProcClip(int frame, float factor, float weight, XSkeltonFrame* skelton)
	{

		if (frame == frame_count)
		{
			for (int i = 0; i < (int)anim_keyset_array.size(); i++)
			{
				XAnimKeySet* key_set =anim_keyset_array[i] ;
				Assert(key_set);
				XBone* bone = skelton->GetBone(key_set->bone_id);
				Assert(bone);
				XMatrix scal;
				XMatrix tran;
				XMatrix rot;
				XScaleKey& sk = key_set->scale_key_array[frame_count - 1];
				XPosKey& pk = key_set->pos_key_array[frame_count - 1];
				XRotKey& rk = key_set->rot_key_array[frame_count - 1];
				XMathMatrixScaling(scal, sk.scal.x, sk.scal.y, sk.scal.z);
				XMathMatrixTranslation(tran, pk.pos.x, pk.pos.y, pk.pos.z);
				XMathQuatToMatrix(rot, rk.quat);
				bone->tran_matrix += scal * rot * tran;
			}
		}
		else if (frame < frame_count)
		{
			for (int i = 0; i < (int)anim_keyset_array.size(); i++)
			{
				XAnimKeySet* key_set =anim_keyset_array[i] ;
				Assert(key_set);
				XBone* bone = skelton->GetBone(key_set->bone_id);
				Assert(bone);
				XMatrix scal1, scal2;
				XMatrix tran1, tran2;
				XMatrix rot1, rot2;
				XScaleKey& sk1 = key_set->scale_key_array[frame - 1];
				XPosKey& pk1 = key_set->pos_key_array[frame - 1];
				XRotKey& rk1 = key_set->rot_key_array[frame - 1];
				XScaleKey& sk2 = key_set->scale_key_array[frame];
				XPosKey& pk2 = key_set->pos_key_array[frame];
				XRotKey& rk2 = key_set->rot_key_array[frame];
				XMathMatrixScaling(scal1, sk1.scal.x, sk1.scal.y, sk1.scal.z);
				XMathMatrixTranslation(tran1, pk1.pos.x, pk1.pos.y, pk1.pos.z);
				XMathQuatToMatrix(rot1, rk1.quat);
				XMathMatrixScaling(scal2, sk2.scal.x, sk2.scal.y, sk2.scal.z);
				XMathMatrixTranslation(tran2, pk2.pos.x, pk2.pos.y, pk2.pos.z);
				XMathQuatToMatrix(rot2, rk2.quat);
				bone->tran_matrix += (scal1 * rot1 * tran1 * factor + scal2 * rot2 * tran2 * (1 - factor));
			}
		}
	}
};

enum ANIM_CTION_STATUS
{
	ANIM_ACTION_CONTINUE,
	ANIM_ACTION_OVER,
};

struct XAnimAction//һ�������������ɺü���clip��ɵ�
{
	std::string action_name;
	xuint32 length;//����ȫ��
	bool play_loop;//�Ƿ�����ѭ��
	//std::vector<XAnimActionClip*> action_clip_array;//���еĶ���Ƭ
	XAnimActionClip* anim_action_clip;
	std::vector<XAnimationEvent*> action_event_array;//���е��¼�
	xuint32 frame_tick;//ÿһ���ؼ�֡������ʱ��
public:
	int ProcAction(xuint32 acc_time, xuint32 old_acc_time, float weight, XSkeltonFrame* skelton)
	{
		if (play_loop)
		{
			xuint32 ti = acc_time % length;
			
		}
		else if (acc_time >= length)
		{
			anim_action_clip->ProcClip(anim_action_clip->frame_count, 1.0f, weight, skelton);
			return ANIM_ACTION_OVER;
		}
		else
		{
			int frame = acc_time / frame_tick;
			int time_delta = acc_time % frame_tick;
			Assert(frame < anim_action_clip->frame_count);
			float factor = (float)time_delta / frame_tick;
			anim_action_clip->ProcClip(frame, factor, weight, skelton);
		}
		return ANIM_ACTION_CONTINUE;
	}
protected:

};

class XMotionMgr
{

};

#define MAX_SYNC_ACTION 8

class XActionMgr : public XMotionMgr//�ؼ�֡����
{
public:
	XActionMgr(XSkeltonFrame* skelton_frame);
public:
	void ProcMotion(xuint32 time_delta)
	{

		xuint32 old_duration = time_duration;
		time_duration += time_delta;
		float weights = 0.0f;
		for (int i = 0; i < MAX_SYNC_ACTION; i++)
		{
			if (ptr_actions[i] && actions_enable[i] && actions_weight[i] > 0.0f)
			{
				weights += actions_weight[i];
			}
		}

		if(IS_FLOAT_ZERO(weights))
		{
			return;
		}

		for (int i = 0; i < MAX_SYNC_ACTION; i++)
		{
			if (ptr_actions[i] && actions_enable[i] && actions_weight[i] > 0.0f)
			{
				//weights += actions_weight[i];
				float weight = actions_weight[i] / weights;
				ptr_actions[i]->ProcAction(time_duration, old_duration, weight, ptr_skelton_frame);
			}
		}
	}
protected:
	XAnimAction* ptr_actions[MAX_SYNC_ACTION];//ֱ��ȫ���������������˲ű�־�������������
	float actions_weight[MAX_SYNC_ACTION];
	bool actions_enable[MAX_SYNC_ACTION];
	bool actions_over[MAX_SYNC_ACTION];
	xuint32 time_duration;//��ǰ���Ŷ�����ʱ��
	XSkeltonFrame* ptr_skelton_frame;
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
