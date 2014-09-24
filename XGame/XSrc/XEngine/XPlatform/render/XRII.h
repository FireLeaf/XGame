/*******************************************************************************
	FILE:		XRII.h
	
	DESCRIPTTION:	render ingore interface
	
	CREATED BY: YangCao, 2014/08/04

	Copyright (C) - All Rights Reserved with Coconat
*******************************************************************************/

#ifndef __COCONAT_XRII_H_
#define __COCONAT_XRII_H_

#include "XAsset.h"

enum X_PRIMITIVETYPE
{
	X_PT_POINTLIST             = 1,
	X_PT_LINELIST              = 2,
	X_PT_LINESTRIP             = 3,
	X_PT_TRIANGLELIST          = 4,
	X_PT_TRIANGLESTRIP         = 5,
	X_PT_TRIANGLEFAN           = 6, 
	X_PT_INVALID,
};

class XRII
{
public:
	virtual void DrawIndexEntity(XVertexAttribute* attrib,
		std::vector<XVertexPool*>& vertex_pools,
		XIndexPool* indices, 
		XMateriaEntity* mtrl, 
		xint32 primitive_type, 
		xuint32 start_index, 
		xuint32 tri_count ) = 0;
};

#endif