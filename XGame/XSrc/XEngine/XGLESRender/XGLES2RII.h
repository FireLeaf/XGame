/**************************************************************************
/*		FileName: XD3D9RII.h
/*		FileDesc: d3d9 äÖÈ¾µÈ½Ó¿Ú
/*		FileAuthor: YangCao
/*		CreateTime: 2014/08/04 22/06/16
/*		CopyRight: yc 
/*************************************************************************/

#ifndef __XD3D9RII__H
#define __XD3D9RII__H

#include "XRII.h"

class XGLES2RII : public XRII
{
public:
	virtual void DrawIndexEntity(XVertexAttribute* attrib,
		std::vector<XVertexPool*>& vertex_pools,
		XIndexPool* indices, 
		XMateriaEntity* mtrl, 
		xint32 primitive_type, 
		xuint32 start_index, 
		xuint32 tri_count );
};

#endif // XD3D9RII