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

struct XAnimActionClip//一个小动画片段
{
	xuint32 start;//动作片段起始时间
	xuint32 length;//动作片段的长度
	std::string anim_name;
	int frame_count;//帧数
	std::vector<XAnimKeySet*> anim_keyset_array;//每根骨头都有
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

struct XAnimAction//一个动作可以是由好几个clip组成的
{
	std::string action_name;
	xuint32 length;//是由全部
	bool play_loop;//是否无限循环
	//std::vector<XAnimActionClip*> action_clip_array;//所有的动作片
	XAnimActionClip* anim_action_clip;
	std::vector<XAnimationEvent*> action_event_array;//所有的事件
	xuint32 frame_tick;//每一个关键帧动画的时间
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

class XActionMgr : public XMotionMgr//关键帧动画
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
	XAnimAction* ptr_actions[MAX_SYNC_ACTION];//直到全部动作都播放完了才标志这个动作播完了
	float actions_weight[MAX_SYNC_ACTION];
	bool actions_enable[MAX_SYNC_ACTION];
	bool actions_over[MAX_SYNC_ACTION];
	xuint32 time_duration;//当前播放动作的时间
	XSkeltonFrame* ptr_skelton_frame;
};

struct XEmotionKey//表情
{
	//std::string emotion_name;
	//std::string skin_name;//指向的皮肤名字，这个直接指向特定的皮肤
	//XVector3
};

class XEmotionMgr : public XMotionMgr//表情和说话动画
{

};


class XAnimationController
{
public:
	XAnimationController(){}
protected:

};

#endif // XAnimationAction
