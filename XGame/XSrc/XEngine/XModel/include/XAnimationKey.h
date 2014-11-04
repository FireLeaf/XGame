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
	XAnimationKey(X_KEY_TYPE kt): type(kt){}
};

struct XScaleKey : public XAnimationKey
{
	//float sx, sy, sz;
	XVector3 scal;
	XScaleKey() : XAnimationKey(X_SCALE_KEY){}
};

struct XPosKey : public XAnimationKey
{
	//float px, py, pz;
	XVector3 pos;
	XPosKey() : XAnimationKey(X_POS_KEY){}
};

struct XRotKey : public XAnimationKey
{
	XQuaternion quat;
	XRotKey() : XAnimationKey(X_ROT_KEY){}
};

struct XMatKey : public XAnimationKey
{
	XMatrix mat;
	XMatKey() : XAnimationKey(X_MATRIX_KEY) {}
};

struct XAnimKeySet//关键帧的集合
{
	std::vector<XScaleKey> scale_key_array;
	std::vector<XPosKey> pos_key_array;
	std::vector<XRotKey> rot_key_array;
	std::vector<XMatKey> mat_key_array;
	int bone_id;//骨头id
};

#endif // XAnimationKey