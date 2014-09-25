/**************************************************************************
/*		FileName: XMatrix.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/25 21:51:06
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XMATRIX__H
#define __XMATRIX__H

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

	friend XMatrix operator * ( float, const XMatrix& );

	bool operator == ( const XMatrix& ) const;
	bool operator != ( const XMatrix& ) const;
	float operator () (int index);
	float& operator () (int index);
public:
	float f[4][4];
};

#endif // XMatrix