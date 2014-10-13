/**************************************************************************
/*		FileName: XMath.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/10/13 21:29:19
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XMATH__H
#define __XMATH__H

#include "XVector.h"
#include "XMatrix.h"

struct XAABB
{
	XVector3 left_up;
	XVector3 right_down;
};


#endif // XMath