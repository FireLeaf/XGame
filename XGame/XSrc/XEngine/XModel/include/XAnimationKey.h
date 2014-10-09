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
};

struct XAnimationKey 
{
	X_KEY_TYPE type;
};

struct XScaleKey : public XAnimationKey
{
	float sx, sy, sz;
	XScaleKey() : type(X_SCALE_KEY){}
};

struct XPosKey : public XAnimationKey
{
	float px, py, pz;
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

struct XKeySet
{
	XAnimationKey** ptr_key_arrays;
};


struct XAnimAction
{
	
};
#endif // XAnimationKey