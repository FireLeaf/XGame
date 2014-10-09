/**************************************************************************
/*		FileName: XVector.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/25 21:18:30
/*		CopyRight: yc 
/*************************************************************************/

#include <math.h>
#include <float.h>
#include <assert.h>
#include "XVector.h"
#include "XType.h"

#define IS_FLOAT_ZERO(a) (fabsf(a) < FLT_EPSILON)

inline bool is_float_zero(float a)
{
	return fabsf(a) < FLT_EPSILON;
}

//vector3
XVector3& XVector3::operator = (const XVector3& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

bool XVector3::operator == (const XVector3& v) const
{
	return is_float_zero(v.x - x) && is_float_zero(v.y - y) && is_float_zero(v.z - z);
}

bool XVector3::operator != (const XVector3& v) const
{
	return !(*this == v);
}

XVector3& XVector3::operator += (const XVector3& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

XVector3& XVector3::operator -= (const XVector3& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

XVector3& XVector3::operator /= (const float v)
{
	if (v == 0.0f)
	{
		Assert(0);
		return *this;
	}
	x /= v;
	y /= v;
	z /= v;
	return *this;
}

float XVector3::operator *(const XVector3 v) const
{
	return x * v.x + y * v.y + z * v.z;
}

XVector3 operator + (const XVector3& l, const XVector3& r)
{
	XVector3 ret(l.x + r.x, l.y + r.y, l.z + r.z);
	return ret;
}
XVector3 operator - (const XVector3& l, const XVector3& r)
{
	XVector3 ret(l.x - r.x, l.y - r.y, l.z - r.z);
	return ret;
}

XVector3 operator / (const XVector3& l, const float r)
{
	XVector3 ret(l);
	ret /= r;
	return ret;
}

//vector4
XVector4& XVector4::operator = (const XVector4& v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	w = v.w;
	return *this;
}

bool XVector4::operator == (const XVector4& v) const
{
	return is_float_zero(v.x - x) && 
		is_float_zero(v.y - y) && 
		is_float_zero(v.z - z)&&
		is_float_zero(v.w - w);
}

bool XVector4::operator != (const XVector4& v) const
{
	return !(*this == v);
}

XVector4& XVector4::operator += (const XVector4& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

XVector4& XVector4::operator -= (const XVector4& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}

XVector4& XVector4::operator /= (const float v)
{
	if (v == 0.0f)
	{
		Assert(0);
		return *this;
	}
	x /= v;
	y /= v;
	z /= v;
	w /= v;
	return *this;
}

XVector4 operator + (const XVector4& l, const XVector4& r)
{
	XVector4 ret(l.x + r.x, l.y + r.y, l.z + r.z, l.w + r.w);
	return ret;
}
XVector4 operator - (const XVector4& l, const XVector4& r)
{
	XVector4 ret(l.x - r.x, l.y - r.y, l.z - r.z, l.w - r.w);
	return ret;
}

XVector4 operator / (const XVector4& l, const float r)
{
	XVector4 ret(l);
	ret /= r;
	return ret;
}