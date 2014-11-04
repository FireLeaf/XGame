/**************************************************************************
/*		FileName: XMatrix.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/25 21:51:06
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XMATRIX__H
#define __XMATRIX__H

#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include "XVector.h"

#define X_PI 3.14159f

class XMatrix
{
public:
	XMatrix(){memset(f, 0, sizeof(f));}
	XMatrix(float _11, float _12, float _13, float _14,
			float _21, float _22, float _23, float _24,
			float _31, float _32, float _33, float _34,
			float _41, float _42, float _43, float _44);
	XMatrix(const XMatrix& m){memcpy(f, m.f, sizeof(f));}
public:
	XMatrix& operator *= ( const XMatrix& );
	XMatrix& operator += ( const XMatrix& );
	XMatrix& operator -= ( const XMatrix& );
	XMatrix& operator *= ( float );
	XMatrix& operator /= ( float );

	// unary operators
	XMatrix operator + () const;
	XMatrix operator - () const;

	// binary operators
	XMatrix operator * ( const XMatrix& ) const;
	XMatrix operator + ( const XMatrix& ) const;
	XMatrix operator - ( const XMatrix& ) const;
	XMatrix operator * ( float ) const;
	XMatrix operator / ( float ) const;

	bool operator == ( const XMatrix& ) const;
	bool operator != ( const XMatrix& ) const;
	float operator () (int index) const;
	float& operator () (int index);
	float operator() (int x, int y) const;
	float& operator()(int x, int y);
public:
	float f[4][4];
};

class XQuaternion
{
public:
	XQuaternion():x(0.0f), y(0.0f), z(0.0f), w(1.0f){}
	XQuaternion(float _x, float _y, float _z, float _w):x(_x), y(_y), z(_z), w(_w){}
	XQuaternion(const XVector3& axis, float angle)
	{
		float sin_value = sinf(angle / 2.0f);
		w = cosf(angle / 2);
		x = axis.x * sin_value;
		y = axis.y * sin_value;
		z = axis.z * sin_value;
	}
public:

	XQuaternion operator + (const XQuaternion&) const;
	friend XQuaternion operator * (const XQuaternion& , float);
	//float operator * (const XQuaternion&) const;
	float Value(){return sqrtf(ValuePow2());}
	float ValuePow2(){return w * w + x * x + y * y + z * z;}
public:
	float x,y,z,w;
};

#endif // XMatrix