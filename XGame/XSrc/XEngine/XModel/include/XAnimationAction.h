/**************************************************************************
/*		FileName: XAnimationKey.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/10/07 1:54:54
/*		CopyRight: yc 
/*************************************************************************/
#ifndef __XANIMATIONKEY__H
#define __XANIMATIONKEY__H

struct XAnimActionClip//一个小动画片段
{
	xuint32 start;//动作片段起始时间
	xuint32 length;//动作片段的长度
	std::string anim_name;
	int play_count;//播放次数，-1表示无限循环
	std::vector<XAnimKeySet> anim_keyset_array;//每根骨头都有
};

struct XAnimAction//一个动作可以是由好几个clip组成的
{
	bool enable;
	std::string action_name;
	xuint32 length;//是由全部
	std::vector<XAnimActionClip*> action_clip_array;
};

class XMotionMgr
{

};

#define MAX_SYNC_ACTION 8

class XActionMgr : public XMotionMgr//关键帧动画
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
	XAnimAction* ptr_actions[MAX_SYNC_ACTION];//直到全部动作都播放完了才标志这个动作播完了
	float actions_weight[MAX_SYNC_ACTION];
	xuint32 time_duration;//当前播放动作的时间
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

#endif // XAnimationKey