/**************************************************************************
/*		FileName: XMathUtil.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/10/13 21:29:30
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XMATHUTIL__H
#define __XMATHUTIL__H
class XVector3;
class XVector4;
class XMatrix;
class XQuaternion;

//vector
float XMathVector3Dot(const XVector3& v1, const XVector3& v2);
void XMathVector3Cross(XVector3& v3, const XVector3& v1, const XVector3& v2);
float XMathVector3Length(const XVector3& v);
float XMathVector3LengthPow2(const XVector3& v);
void XMathVector3Normalize(XVector3& v);

//matrix
void XMathMatrixZero(XMatrix& m);
void XMathMatrixIdentity(XMatrix& m);
void XMathMatrixTranslation(XMatrix& m, float x, float y, float z);
void XMathMatrixScaling(XMatrix& m, float sx, float sy, float sz);
void XMathMatrixTranspose(const XMatrix& m);
float XMathMatrixDeterminant(const XMatrix& m);//行列式的值
void XMathMatrixInverse(const XMatrix& m, XMatrix n); 
void XMathMatrixRotationX(XMatrix& m, float angle);
void XMathMatrixRotationY(XMatrix& m, float angle);
void XMathMatrixRotationZ(XMatrix&m, float angle);
void XMathMatrixRotationAxis(XMatrix& m, XVector3 axis, float angle);
void XMathMatrixLookAtLH(XMatrix& m, const XVector3& eye, const XVector3& up, const XVector3& at);
void XMathMatrixLookAtRH(XMatrix& m, const XVector3& eye, const XVector3& up, const XVector3& at);
void XMathMatrixPerspectiveFovLH(XMatrix& m, float fovy,float Aspect,float zn, float zf);
void XMathMatrixPerspectiveFovRH(XMatrix& m, float fovy,float Aspect,float zn, float zf);

//quaternion
void XMathQuatNormalize(XQuaternion& q);
void XMathQuatToMatrix(XMatrix& m, const XQuaternion& q);
void XMathQuatSlerp(XQuaternion& q, const XQuaternion& q1, const XQuaternion& q2, float t);//球面线性插值
void XMathQuatLerp(XQuaternion& q, const XQuaternion& q1, const XQuaternion& q2, float t);//线性插值

//vector-matrix
void XMathVector3Transform(const XMatrix& m, const XVector3& v, XVector3& o);
void XMathVector3TransformCoord(const XMatrix& m, const XVector3& v, XVector3& o);

int XRandomInt();
int XRandomInt(int aMin, int bMax);
float XRandomFloat();
float XRandomFloat(float aMin, float bMax);
void XRandomVector3();

#endif // XMathUtil


