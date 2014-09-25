/**************************************************************************
/*		FileName: XD3D9RII.h
/*		FileDesc: d3d9 äÖÈ¾µÈ½Ó¿Ú
/*		FileAuthor: YangCao
/*		CreateTime: 2014/08/04 22/06/16
/*		CopyRight: yc 
/*************************************************************************/

#include "XD3D9RII.h"
#include "XRenderUtil.h"
#include "XD3D9RenderContext.h"
#include "XD3D9Asset.h"

void XD3D9RII::DrawIndexEntity(XVertexAttribute* attrib,
							   XStl::vector<XVertexPool*>& vertex_pools,
							   XIndexPool* indices, 
							   XMateriaEntity* mtrl, 
							   xint32 primitive_type, 
							   xuint32 start_index, 
							   xuint32 tri_count )
{
	x_ptr_d3ddevice->SetIndices(((XD3D9IndexPool*)indices)->GetD3D9IndexBuffer());
	for (int i = 0; i < vertex_pools.size(); i++)
	{
		if(vertex_pools[i])
			x_ptr_d3ddevice->SetStreamSource(i, ((XD3D9VertexPool*)(vertex_pools[i]))->GetD3D9VertexBuffer(), 0, attrib->GetVertexAttributeDesc().vecVertexElement[i].stride);
	}
	x_ptr_d3ddevice->SetVertexDeclaration(((XD3D9VertexAttribute*)attrib)->GetD3D9VertexDecl());
	if(mtrl->vertex_shader)
		x_ptr_d3ddevice->SetVertexShader(((XD3D9VertexShader*)mtrl->vertex_shader)->GetD3D9VertexShader());
	if(mtrl->pixel_shader)
		x_ptr_d3ddevice->SetPixelShader(((XD3D9PixelShader*)mtrl->pixel_shader)->GetD3D9PixelShader());
	x_ptr_d3ddevice->DrawIndexedPrimitive(
		(D3DPRIMITIVETYPE)RenderUtil::GetPrimitiveType(primitive_type),
		0, 0, vertex_pools[0]->GetVertexPoolDesc().count / attrib->GetVertexAttributeDesc().vecVertexElement[0].stride, start_index, tri_count
		);
}
