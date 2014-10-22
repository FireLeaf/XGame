/*******************************************************************************
	FILE:		XCamera.h
	
	DESCRIPTTION:	
	
	CREATED BY: YangCao, 2014/10/16

	Copyright (C) - All Rights Reserved with Coconat
*******************************************************************************/

#ifndef __COCONAT_XCAMERA_H_
#define __COCONAT_XCAMERA_H_

#include "XMath.h"
#include "XMatrix.h"
#include "XVector.h"
#include "XMathUtil.h"

class XCamera
{
public:
	void SetEye(const XVector3& eye){dirty = true; this->eye = eye;}
	void SetUp(const XVector3& up){dirty = true; this->up = up;}
	void SetAt(const XVector3& at){dirty = true; this->at = at;}

	void CalcViewMatrix(){XMathMatrixLookAtLH(mat_view, eye, up, at);dirty = false;}
	XMatrix GetViewMatrix(){if(dirty){CalcViewMatrix();} return mat_view;}
public:
	XMatrix mat_view;
	XVector3 eye;
	XVector3 up;
	XVector3 at;
	bool dirty;
};

struct XViewPort
{
public:
	XMatrix GetProjMatrix(){XMathMatrixPerspectiveFovLH(mat_proj, fov, (float)width / (float)height, z_near, z_far); return mat_proj;}
public:
	xuint32		width;
	xuint32		height;       /* Viewport Dimensions */
	float		z_near;         /* Min/max of clip Volume */
	float		z_far;
	float		fov;
	XMatrix		mat_proj;
};

struct XFrustum
{
public:
	// ”æ∞ÃÂ≤√ºÙ
	bool DetectPoint(const XVector3 &point);
	bool DetectCuboid(const XVector3 &center,float width,float length,float height);
	bool DetectCube(const XVector3 &center,float size);
	bool DetectRectangle(const XVector3 &center,float width,float length);
	bool DetectSquare(const XVector3 &center,float size);
	bool DetectAABB(const XAABB& aabb){return true;}
};

#endif