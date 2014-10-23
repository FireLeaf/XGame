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

struct XAnimKeySet//�ؼ�֡�ļ���
{
	std::vector<XScaleKey> scale_key_array;
	std::vector<XPosKey> pos_key_array;
	std::vector<XRotKey> rot_key_array;
	std::vector<XMatKey> mat_key_array;
	int bone_id;//��ͷid
};

struct XAnimActionClip//һ��С����Ƭ��
{
	float start;//����Ƭ����ʼʱ��
	float length;//����Ƭ�εĳ���
	std::string anim_name;
	int play_count;//���Ŵ�����-1��ʾ����ѭ��
	std::vector<XAnimKeySet> anim_keyset_array;//ÿ����ͷ����
};

struct XAnimAction//һ�������������ɺü���clip��ɵ�
{
	std::string action_name;
	float length;//����ȫ��
	std::vector<XAnimActionClip*> action_clip_array;
};

class XMotionMgr
{

};

#define MAX_SYNC_ACTION 8

class XActionMgr : public XMotionMgr//�ؼ�֡����
{
public:
	void ProcMotion(float time_delta);
protected:
	XAnimAction* ptr_actions[MAX_SYNC_ACTION];//ֱ��ȫ���������������˲ű�־�������������
	float time_duration;//��ǰ���Ŷ�����ʱ��
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

#endif // XAnimationKey