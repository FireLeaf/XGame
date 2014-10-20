/**************************************************************************
/*		FileName: XMathUtil.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/10/13 21:29:40
/*		CopyRight: yc 
/*************************************************************************/

#include "XMath.h"
#include "XVector.h"
#include "XMatrix.h"
#include <math.h>
#include "XType.h"

#define IS_FLOAT_ZERO(a) (fabsf(a) < FLT_EPSILON)

float XMathVector3Dot(const XVector3& v1, const XVector3& v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

void XMathVector3Cross(XVector3& v3, const XVector3& v1, const XVector3& v2)
{
	v3.x = v1.y * v2.z - v1.z * v2.y;
	v3.y = v1.z * v2.x - v1.x * v2.z;
	v3.z = v1.x * v2.y - v1.y * v2.x;
}

float XMathVector3Length(const XVector3& v)
{
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

float XMathVector3LengthPow2(const XVector3& v)
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

void XMathMatrixZero(XMatrix& m)
{
	memset(&m, 0, sizeof(m));
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
	XMathMatrixZero(m);
	m.f[0][0] = sx;
	m.f[1][1] = sy;
	m.f[2][2] = sz;
}

inline float in_XClacDeter3X3(float m11, float m12, float m13, 
							  float m21, float m22, float m23,
							  float m31, float m32, float m33)
{
	return m11 * (m22 * m33 - m23 * m32) + m12 * (m23 * m31 - m21 * m33) + m13 * (m21 * m32 - m22 * m31);
}

float XMathMatrixDeterminant(const XMatrix& m)
{
	//return m.f[]
	return m(0, 0) * in_XClacDeter3X3(m(1, 1), m(1, 2), m(1, 3), m(2, 1), m(2, 2), m(2, 3), m(3, 1), m(3, 2), m(3, 3))
		+ m(0, 1) * in_XClacDeter3X3(m(1, 0), m(1, 2), m(1, 3), m(2, 0), m(2, 2), m(2, 3), m(3, 0), m(3, 2), m(3, 3))
		+ m(0, 2) * in_XClacDeter3X3(m(1, 0), m(1, 1), m(1, 3), m(2, 0), m(2, 1), m(2, 3), m(3, 0), m(3, 1), m(3, 3))
		+ m(0, 3) * in_XClacDeter3X3(m(1, 0), m(1, 1), m(1, 2), m(2, 0), m(2, 1), m(2, 2), m(3, 0), m(3, 1), m(3, 2));
}

void XMathMatrixInverse(XMatrix& m,const XMatrix& n)
{
	float determin = XMathMatrixDeterminant(n);
	if (IS_FLOAT_ZERO(determin))
	{
		Assert(0);
		m = n;
		return;
	}
	m(0, 0) = in_XClacDeter3X3(m(1, 1), m(1, 2), m(1, 3), m(2, 1), m(2, 2), m(2, 3), m(3, 1), m(3, 2), m(3, 3));
	m(0, 1) = in_XClacDeter3X3(m(1, 0), m(1, 2), m(1, 3), m(2, 0), m(2, 2), m(2, 3), m(3, 0), m(3, 2), m(3, 3));
	m(0, 2) = in_XClacDeter3X3(m(1, 0), m(1, 1), m(1, 3), m(2, 0), m(2, 1), m(2, 3), m(3, 0), m(3, 1), m(3, 3));
	m(0, 3) = in_XClacDeter3X3(m(1, 0), m(1, 1), m(1, 2), m(2, 0), m(2, 1), m(2, 2), m(3, 0), m(3, 1), m(3, 2));

	m(1, 0) = in_XClacDeter3X3(m(0, 1), m(0, 2), m(0, 3), m(2, 1), m(2, 2), m(2, 3), m(3, 1), m(3, 2), m(3, 3));
	m(1, 1) = in_XClacDeter3X3(m(0, 0), m(0, 2), m(0, 3), m(2, 0), m(2, 2), m(2, 3), m(3, 0), m(3, 2), m(3, 3));
	m(1, 2) = in_XClacDeter3X3(m(0, 0), m(0, 1), m(0, 3), m(2, 0), m(2, 1), m(2, 3), m(3, 0), m(3, 1), m(3, 3));
	m(1, 3) = in_XClacDeter3X3(m(0, 0), m(0, 1), m(0, 2), m(2, 0), m(2, 1), m(2, 2), m(3, 0), m(3, 1), m(3, 2));

	m(2, 0) = in_XClacDeter3X3(m(0, 1), m(0, 2), m(0, 3), m(1, 1), m(1, 2), m(1, 3), m(3, 1), m(3, 2), m(3, 3));
	m(2, 1) = in_XClacDeter3X3(m(0, 0), m(0, 2), m(0, 3), m(1, 0), m(1, 2), m(1, 3), m(3, 0), m(3, 2), m(3, 3));
	m(2, 2) = in_XClacDeter3X3(m(0, 0), m(0, 1), m(0, 3), m(1, 0), m(1, 1), m(1, 3), m(3, 0), m(3, 1), m(3, 3));
	m(2, 3) = in_XClacDeter3X3(m(0, 0), m(0, 1), m(0, 2), m(1, 0), m(1, 1), m(1, 2), m(3, 0), m(3, 1), m(3, 2));

	m(3, 0) = in_XClacDeter3X3(m(0, 1), m(0, 2), m(0, 3), m(1, 1), m(1, 2), m(1, 3), m(2, 1), m(2, 2), m(2, 3));
	m(3, 1) = in_XClacDeter3X3(m(0, 0), m(0, 2), m(0, 3), m(1, 0), m(1, 2), m(1, 3), m(2, 0), m(2, 2), m(2, 3));
	m(3, 2) = in_XClacDeter3X3(m(0, 0), m(0, 1), m(0, 3), m(1, 0), m(1, 1), m(1, 3), m(2, 0), m(2, 1), m(2, 3));
	m(3, 3) = in_XClacDeter3X3(m(0, 0), m(0, 1), m(0, 2), m(1, 0), m(1, 1), m(1, 2), m(2, 0), m(2, 1), m(2, 2));

	m /= determin;
}

void XMathMatrixRotationX(XMatrix& m, float angle)
{
	XMathMatrixZero(m);
	m(0, 0) = m(3, 3) = 1.0f;
	m(1, 1) = m(2, 2)= cosf(angle);
	m(1, 2) = sinf(angle);
	m(2, 1) = -m(1, 2);
}

void XMathMatrixRotationY(XMatrix& m, float angle)
{
	XMathMatrixZero(m);
	m(0, 0) = m(3, 3) = 1.0f;
	m(1, 1) = m(2, 2)= cosf(angle);
	m(1, 2) = -sinf(angle);
	m(2, 1) = -m(1, 2);
}

void XMathMatrixRotationZ(XMatrix&m, float angle)
{
	XMathMatrixZero(m);
	m(0, 0) = m(3, 3) = 1.0f;
	m(1, 1) = m(2, 2)= cosf(angle);
	m(1, 2) = -sinf(angle);
	m(2, 1) = -m(1, 2);
}

void XMathMatrixRotationAxis(XMatrix& m, XVector3 axis, float angle)
{
	XMathMatrixZero(m);
	XMathVector3Normalize(axis);
	float sf = sinf(angle);
	float cf = cosf(angle);
	m(0, 3) = m(1, 3) = m(2, 3) = 0.0f;
	m(3, 3) = 1.0f;

	m(0, 0) = axis.x * axis.x * (1 - cf) + cf;
	m(1, 0) = axis.x * axis.y *(1 - cf) + axis.z * sf;
	m(2, 0) = axis.x * axis.z * (1.0f - cf) - axis.y * sf;

	m(0, 1) = axis.x * axis.y *(1 - cf) - axis.z * sf;
	m(1, 1) = axis.y * axis.y * (1 - cf) + cf;
	m(2, 1) = axis.y * axis.z * (1.0f - cf) + axis.x * sf;

	m(0, 2) = axis.x * axis.z *(1 - cf) + axis.y * sf;
	m(1, 2) = axis.y * axis.z * (1.0f - cf) - axis.x * sf;
	m(2, 2) = axis.z * axis.z * (1 - cf) + cf;
}

/*
*zaxis = normal(At - Eye)
xaxis = normal(cross(Up, zaxis))
yaxis = cross(zaxis, xaxis)

xaxis.x           yaxis.x           zaxis.x          0
xaxis.y           yaxis.y           zaxis.y          0
xaxis.z           yaxis.z           zaxis.z          0
-dot(xaxis, eye)  -dot(yaxis, eye)  -dot(zaxis, eye)  1

*/
void XMathMatrixLookAtLH(XMatrix& m, const XVector3& eye, const XVector3& up, const XVector3& at)
{
	XMathMatrixZero(m);
	XVector3 zAxis = at - eye;
	XMathVector3Normalize(zAxis);
	XVector3 xAxis;// = *up;
	XMathVector3Cross(xAxis, up, zAxis);
	XMathVector3Normalize(xAxis);
	XVector3 yAxis;
	XMathVector3Cross(yAxis, zAxis, xAxis);

	m(0, 3) = m(1, 3) = m(2, 3) = 0.0f;
	m(3, 3) = 1.0f;

	m(0, 0) = xAxis.x;m(1, 0) = xAxis.y;m(2, 0) = xAxis.z;
	m(0, 1) = yAxis.x;m(1, 1) = yAxis.y;m(2, 1) = yAxis.z;
	m(0, 2) = zAxis.x;m(1, 2) = zAxis.y;m(2, 2) = zAxis.z;

	m(3, 0) = -(xAxis * eye);
	m(3, 1) = -(yAxis * eye);
	m(3, 2) = -(zAxis * eye);
}

/*
*zaxis = normal(Eye - At)
xaxis = normal(cross(Up, zaxis))
yaxis = cross(zaxis, xaxis)

xaxis.x           yaxis.x           zaxis.x          0
xaxis.y           yaxis.y           zaxis.y          0
xaxis.z           yaxis.z           zaxis.z          0
-dot(xaxis, eye)  -dot(yaxis, eye)  -dot(zaxis, eye)  1

*/
void XMathMatrixLookAtRH(XMatrix& m, const XVector3& eye, const XVector3& up, const XVector3& at)
{
	XMathMatrixZero(m);
	XVector3 zAxis = eye - at;
	XMathVector3Normalize(zAxis);
	XVector3 xAxis;// = *up;
	XMathVector3Cross(xAxis, up, zAxis);
	XMathVector3Normalize(xAxis);
	XVector3 yAxis;
	XMathVector3Cross(yAxis, zAxis, xAxis);

	m(0, 3) = m(1, 3) = m(2, 3) = 0.0f;
	m(3, 3) = 1.0f;

	m(0, 0) = xAxis.x;m(1, 0) = xAxis.y;m(2, 0) = xAxis.z;
	m(0, 1) = yAxis.x;m(1, 1) = yAxis.y;m(2, 1) = yAxis.z;
	m(0, 2) = zAxis.x;m(1, 2) = zAxis.y;m(2, 2) = zAxis.z;

	m(3, 0) = -(xAxis * eye);
	m(3, 1) = -(yAxis * eye);
	m(3, 2) = -(zAxis * eye);
}

void XMathMatrixPerspectiveFovLH(XMatrix& m, float fovy, float Aspect, float zn, float zf)
{
	XMathMatrixZero(m);
	float yScale = tan(X_PI / 2.0f - fovy / 2.0f);//atanf(fovy / 2);
	float xScale = yScale / Aspect;

	m(0, 0) = xScale;
	m(1, 1) = yScale;
	m(2, 2) = zf / (zf - zn);
	m(2, 3) = 1.0f;
	m(3, 2) = - zn * zf / (zf - zn);
}

void XMathMatrixPerspectiveFovRH(XMatrix& m, float fovy,float Aspect,float zn, float zf)
{
	XMathMatrixZero(m);
	float yScale = tan(X_PI / 2.0f - fovy / 2.0f);//atanf(fovy / 2);
	float xScale = yScale / Aspect;

	m(0, 0) = xScale;
	m(1, 1) = yScale;
	m(2, 2) = zf / (zf - zn);
	m(2, 3) = -1.0f;
	m(3, 2) = zn * zf / (zf - zn);
}

void XMathVector3Transform(const XMatrix& m, const XVector3& v, XVector3& o)
{
	XVector4 v4(v);
	float w = v4.x * m(0, 3) + v4.y * m(1, 3) + v4.z * m(2, 3) + v4.w * m(3, 3);
	o.x = v4.x * m(0, 0) + v4.y * m(1, 0) + v4.z * m(2, 0) + v4.w * m(3, 0);
	o.y = v4.x * m(0, 1) + v4.y * m(1, 1) + v4.z * m(2, 1) + v4.w * m(3, 1);
	o.z = v4.x * m(0, 0) + v4.y * m(1, 2) + v4.z * m(2, 2) + v4.w * m(3, 2);
	if (IS_FLOAT_ZERO(w))
	{
		w = 1.0f;
		Assert(0);
	}
	o /= w;
}

void XMathVector3TransformCoord(const XMatrix& m, const XVector3& v, XVector3& o)
{
	o.x = v.x * m(0, 0) + v.y * m(1, 0) + v.z * m(2, 0);
	o.y = v.x * m(0, 1) + v.y * m(1, 1) + v.z * m(2, 1);
	o.z = v.x * m(0, 0) + v.y * m(1, 2) + v.z * m(2, 2);
}

int XRandomInt()
{
	return rand();
}

int XRandomInt(int aMin, int bMax)
{
	if (aMin >= bMax)
	{
		return aMin;
	}

	return ((rand() % (bMax - aMin)) + aMin);
}

float XRandomFloat()
{
	return (float)XRandomInt() / (float)RAND_MAX;
}

float XRandomFloat(float aMin, float bMax)
{
	if (aMin>= bMax)
	{
		return aMin;
	}

	return (aMin + (bMax - aMin) * XRandomFloat());
}