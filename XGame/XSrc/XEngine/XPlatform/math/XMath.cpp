/**************************************************************************
/*		FileName: XMath.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/25 22:37:33
/*		CopyRight: yc 
/*************************************************************************/

#include "XMath.h"
#include "XVector.h"
#include "XMatrix.h"

float XMathVector3Dot(const XVector3& v1, const XVector3& v2)
{
	return v1.x * v2.x + v1.y + v2.y + v1.z + v2.z;
}

void XMathVector3Cross(const XVector3& v1, const XVector3& v2, XVector3& v3)
{
	v3.x = v1.y * v2.z - v1.z * v2.y;
	v3.y = v1.z * v2.x - v1.x * v2.z;
	v3.z = v1.x * v2.y - v1.y * v2.x;
}

float XMathVector3Length(const XVector3& v)
{
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

float XMathVector3LengthUsq(const XVector3& v)
{
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

void XMathVector3Normalize(XVector3& v)
{
	float length = XMathVector3Length(v);
	if (length < FLT_EPSILON)
	{
		return;
	}
	v.x /= length;
	v.y /= length;
	v.z /= length;
}

void XMathMatrixIdentity(XMatrix& m)
{
	memset(&m, 0, sizeof(m));
	m.f[0][0] = m.f[1][1] = m.f[2][2] = m.f[3][3] = 1.0f;
}

void XMathMatrixTranslation(XMatrix& m, float x, float y, float z)
{
	XMathMatrixIdentity(m);
	m.f[4][0] = x;
	m.f[4][1] = y;
	m.f[4][2] = z;
}

void XMathMatrixScaling(XMatrix& m, float sx, float sy, float sz)
{
	XMathMatrixIdentity(m);
	m.f[0][0] = sx;
	m.f[1][1] = sy;
	m.f[2][2] = sz;
}

float XMathMatrixDeterminant(const XMatrix& m)
{
	//return m.f[]
	return 0.0f;
}

void XMathMatrixInverse(const XMatrix& m, XMatrix n)
{

}