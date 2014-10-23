/**************************************************************************
/*		FileName: XAnimationKey.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/10/07 1:54:54
/*		CopyRight: yc 
/*************************************************************************/
#ifndef __XANIMATIONKEY__H
#define __XANIMATIONKEY__H

#include "XMatrix.h"
#include "XVector.h"

enum X_KEY_TYPE
{
	X_SCALE_KEY,
	X_POS_KEY,
	X_ROT_KEY,
	X_MATRIX_KEY,
	X_KEY_COUNT,
};

struct XAnimationKey 
{
	X_KEY_TYPE type;
	float ti;
};

struct XScaleKey : public XAnimationKey
{
	//float sx, sy, sz;
	XVector3 scal;
	XScaleKey() : type(X_SCALE_KEY){}
};

struct XPosKey : public XAnimationKey
{
	//float px, py, pz;
	XVector3 pos;
	XPosKey() : type(X_POS_KEY){}
};

struct XRotKey : public XAnimationKey
{
	XQuaternion quat;
	XRotKey() : type(X_ROT_KEY){}
};

struct XMatKey : public XAnimationKey
{
	XMatrix mat;
	XMatKey() : type(X_MATRIX_KEY) {}
};

struct XAnimKeySet//关键帧的集合
{
	std::vector<XScaleKey> scale_key_array;
	std::vector<XPosKey> pos_key_array;
	std::vector<XRotKey> rot_key_array;
	std::vector<XMatKey> mat_key_array;
	int bone_id;//骨头id
};

struct XAnimActionClip//一个小动画片段
{
	float start;//动作片段起始时间
	float length;//动作片段的长度
	std::string anim_name;
	int play_count;//播放次数，-1表示无限循环
	std::vector<XAnimKeySet> anim_keyset_array;//每根骨头都有
};

struct XAnimAction//一个动作可以是由好几个clip组成的
{
	std::string action_name;
	float length;//是由全部
	std::vector<XAnimActionClip*> action_clip_array;
};

class XMotionMgr
{

};

#define MAX_SYNC_ACTION 8

class XActionMgr : public XMotionMgr//关键帧动画
{
public:
	void ProcMotion(float time_delta);
protected:
	XAnimAction* ptr_actions[MAX_SYNC_ACTION];//直到全部动作都播放完了才标志这个动作播完了
	float time_duration;//当前播放动作的时间
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

#endif // XAnimationKey