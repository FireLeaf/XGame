/**************************************************************************
/*		FileName: XMatrix.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/25 22:05:34
/*		CopyRight: yc 
/*************************************************************************/

#include "XMatrix.h"

#define IS_FLOAT_ZERO(a) (fabsf(a) < FLT_EPSILON)

XMatrix::XMatrix(float _11, float _12, float _13, float _14, 
				 float _21, float _22, float _23, float _24, 
				 float _31, float _32, float _33, float _34, 
				 float _41, float _42, float _43, float _44)
{
	f[0][0] = _11; f[0][1] = _12; f[0][2] = _13; f[0][3] = _14;
	f[1][0] = _21; f[1][1] = _22; f[1][2] = _23; f[1][3] = _24;
	f[2][0] = _31; f[2][1] = _32; f[2][2] = _33; f[2][3] = _34;
	f[3][0] = _41; f[3][1] = _42; f[3][2] = _43; f[3][3] = _44;
}

XMatrix& XMatrix::operator *= (const XMatrix& m)
{
	f[0][0] = f[0][0] * m.f[0][0] + f[0][1] * m.f[1][0] +
			  f[0][2] + m.f[2][0] + f[0][3] * m.f[3][0];
	f[0][1] = f[0][0] * m.f[0][1] + f[0][1] * m.f[1][1] +
			  f[0][2] + m.f[2][1] + f[0][3] * m.f[3][1];
	f[0][2] = f[0][0] * m.f[0][2] + f[0][1] * m.f[1][2] +
			  f[0][2] + m.f[2][2] + f[0][3] * m.f[3][2];
	f[0][3] = f[0][0] * m.f[0][3] + f[0][1] * m.f[1][3] +
			  f[0][2] + m.f[2][3] + f[0][3] * m.f[3][3];

	f[1][0] = f[1][0] * m.f[0][0] + f[1][1] * m.f[1][0] +
			  f[1][2] + m.f[2][0] + f[1][3] * m.f[3][0];
	f[1][1] = f[1][0] * m.f[0][1] + f[1][1] * m.f[1][1] +
			  f[1][2] + m.f[2][1] + f[1][3] * m.f[3][1];
	f[1][2] = f[1][0] * m.f[0][2] + f[1][1] * m.f[1][2] +
			  f[1][2] + m.f[2][2] + f[1][3] * m.f[3][2];
	f[1][3] = f[1][0] * m.f[0][3] + f[1][1] * m.f[1][3] +
			  f[1][2] + m.f[2][3] + f[1][3] * m.f[3][3];

	f[2][0] = f[2][0] * m.f[0][0] + f[2][1] * m.f[1][0] +
			  f[2][2] + m.f[2][0] + f[2][3] * m.f[3][0];
	f[2][1] = f[2][0] * m.f[0][1] + f[2][1] * m.f[1][1] +
			  f[2][2] + m.f[2][1] + f[2][3] * m.f[3][1];
	f[2][2] = f[2][0] * m.f[0][2] + f[2][1] * m.f[1][2] +
			  f[2][2] + m.f[2][2] + f[2][3] * m.f[3][2];
	f[2][3] = f[2][0] * m.f[0][3] + f[2][1] * m.f[1][3] +
			  f[2][2] + m.f[2][3] + f[2][3] * m.f[3][3];

	f[3][0] = f[3][0] * m.f[0][0] + f[3][1] * m.f[1][0] +
			  f[3][2] + m.f[2][0] + f[3][3] * m.f[3][0];
	f[3][1] = f[3][0] * m.f[0][1] + f[3][1] * m.f[1][1] +
			  f[3][2] + m.f[2][1] + f[3][3] * m.f[3][1];
	f[3][2] = f[3][0] * m.f[0][2] + f[3][1] * m.f[1][2] +
			  f[3][2] + m.f[2][2] + f[3][3] * m.f[3][2];
	f[3][3] = f[3][0] * m.f[0][3] + f[3][1] * m.f[1][3] +
			  f[3][2] + m.f[2][3] + f[3][3] * m.f[3][3];

	return *this;
}

XMatrix& XMatrix::operator += (const XMatrix& m)
{
	f[0][0] += m.f[0][0];f[0][1] += m.f[0][1];
	f[0][2] += m.f[0][2];f[0][3] += m.f[0][3];
	f[1][0] += m.f[1][0];f[1][1] += m.f[1][1];
	f[1][2] += m.f[1][2];f[1][3] += m.f[1][3];
	f[2][0] += m.f[2][0];f[2][1] += m.f[2][1];
	f[2][2] += m.f[2][2];f[2][3] += m.f[2][3];
	f[3][0] += m.f[3][0];f[3][1] += m.f[3][1];
	f[3][2] += m.f[3][2];f[3][3] += m.f[3][3];
	return *this;
}

XMatrix& XMatrix::operator -= (const XMatrix& m)
{
	f[0][0] -= m.f[0][0];f[0][1] -= m.f[0][1];
	f[0][2] -= m.f[0][2];f[0][3] -= m.f[0][3];
	f[1][0] -= m.f[1][0];f[1][1] -= m.f[1][1];
	f[1][2] -= m.f[1][2];f[1][3] -= m.f[1][3];
	f[2][0] -= m.f[2][0];f[2][1] -= m.f[2][1];
	f[2][2] -= m.f[2][2];f[2][3] -= m.f[2][3];
	f[3][0] -= m.f[3][0];f[3][1] -= m.f[3][1];
	f[3][2] -= m.f[3][2];f[3][3] -= m.f[3][3];
	return *this;
}

XMatrix& XMatrix::operator * (float v)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			f[i][j] *= v;
		}
	}

	return *this;
}

XMatrix& XMatrix::operator / (float v)
{
	if (IS_FLOAT_ZERO(v))
	{
		Assert(0);
		return *this;
	}
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			f[i][j] /= v;
		}
	}

	return *this;
}

// unary operators
XMatrix XMatrix::operator + () const
{
	return *this;
}

XMatrix XMatrix::operator - () const
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			f[i][j] = -f[i][j];
		}
	}
	return *this;
}

XMatrix XMatrix::operator * ( const XMatrix& m) const
{
	XMatrix ret(*this);
	ret *= m;
	return ret;
}

XMatrix XMatrix::operator + ( const XMatrix& m) const
{
	XMatrix ret(*this);
	ret += m;
	return ret;
}

XMatrix XMatrix::operator - ( const XMatrix& m) const
{
	XMatrix ret(*this);
	ret -= m;
	return ret;
}

XMatrix XMatrix::operator * ( float v) const
{
	XMatrix ret(*this);
	ret *= v;
	return ret;
}

XMatrix XMatrix::operator / ( float v) const
{
	XMatrix ret(*this);
	ret /= v;
	return ret;
}

XMatrix operator * ( float v, const XMatrix& m)
{
	XMatrix ret(m);
	m *= v;
	return ret;
}

bool  XMatrix::operator == ( const XMatrix& m) const
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (!(IS_FLOAT_ZERO(f[i][j] - m.f[i][j])))
			{
				return false;
			}
		}
	}
}

bool  XMatrix::operator != ( const XMatrix& m) const
{
	return !(*this == m);
}

float  XMatrix::operator () (int index)
{
	if (index > 15)
	{
		Assert(0);
		return 0.0f;
	}
	int a = index / 4, b = index % 4;
	return f[a][b];
}

float&  XMatrix::operator () (int index)
{
	if (index > 15)
	{
		Assert(0);
		return 0.0f;
	}
	int a = index / 4, b = index % 4;
	return f[a][b];
}