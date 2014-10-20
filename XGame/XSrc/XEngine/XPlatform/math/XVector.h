/**************************************************************************
/*		FileName: XVector3.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/25 21:06:39
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XVECTOR3__H
#define __XVECTOR3__H

class XVector2
{
public:
	union
	{
		float x,y;
		float v[2];
	};
};

class XVector3
{
public:
	XVector3() : x(0.0f), y(0.0f), z(0.0f){}
	XVector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z){}
	XVector3(float* v) : x(v[0]), y(v[1]), z(v[2]){}
	XVector3(const XVector3& v) : x(v.x), y(v.y), z(v.z){}
public:
	XVector3& operator = (const XVector3& v);
	bool operator == (const XVector3& v) const;
	bool operator != (const XVector3& v) const;
	XVector3& operator += (const XVector3& v);
	XVector3& operator -= (const XVector3& v);
	XVector3& operator /= (const float v);
	float operator * (const XVector3 v) const;
	friend XVector3 operator + (const XVector3& l, const XVector3& r);
	friend XVector3 operator - (const XVector3& l, const XVector3& r);
	friend XVector3 operator / (const XVector3& l, const float r);
public:
	float x,y,z;
};

class XVector4
{
public:
	XVector4() : x(0.0f), y(0.0f), z(0.0f), w(1.0f){}
	XVector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w){}
	XVector4(float* v) : x(v[0]), y(v[1]), z(v[2]), w(v[3]){}
	XVector4(const XVector4& v) : x(v.x), y(v.y), z(v.z), w(v.w){}
	XVector4(const XVector3& v) : x(v.x), y(v.y), z(v.z), w(1.0f){}
public:
	XVector4& operator = (const XVector4& v);
	bool operator == (const XVector4& v) const;
	bool operator != (const XVector4& v) const;
	XVector4& operator += (const XVector4& v);
	XVector4& operator -= (const XVector4& v);
	XVector4& operator /= (const float v);
	friend XVector4 operator + (const XVector4& l, const XVector4& r);
	friend XVector4 operator - (const XVector4& l, const XVector4& r);
	friend XVector4 operator / (const XVector4& l, const float r);
public:
	float x,y,z,w;
};

#endif // XVector3