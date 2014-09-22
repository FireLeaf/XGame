/**************************************************************************
/*		FileName: XD3D9Asset.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/07/29 20/13/43
/*		CopyRight: yc 
/*************************************************************************/

#include "XAsset.h"
#include "XD3D9Header.h"
#include "XD3D9Asset.h"
#include "XD3D9RenderContext.h"
#include "XD3D9RenderUtil.h"
#include "XTemplate.h"

void XD3D9Texture2D::UpdateAsset(XAssetMonitor* pMonitor)
{
	if (m_bDirty)
	{
		m_bDirty = false;

		IDirect3DDevice9* pD3D9Device = (IDirect3DDevice9*)x_ptr_render_context->GetRenderContext();
		if (!pD3D9Device)
		{
			Assert(0);
			return;
		}

		bool bReCreate = false;

		if (m_pD3D92DTexture)
		{
			D3DSURFACE_DESC desc;
			m_pD3D92DTexture->GetLevelDesc(0, &desc);
			if (desc.Width != m_formatDesc.width
				|| desc.Height != m_formatDesc.height
				|| desc.Pool != (D3DPOOL)RenderUtil::GetAssetManageType(m_bDynamic)
				|| desc.Format != (D3DFORMAT)RenderUtil::GetTexFormat(m_formatDesc.tex_format)
				)
			{
				bReCreate = true;
				m_pD3D92DTexture->Release();
				m_pD3D92DTexture = NULL;
			}

		}
		if (bReCreate)
		{
			HRESULT hr = pD3D9Device->CreateTexture(m_formatDesc.width, m_formatDesc.height, pixelData.generate_level, 0, (D3DFORMAT)RenderUtil::GetTexFormat(m_formatDesc.tex_format), (D3DPOOL)RenderUtil::GetAssetManageType(m_bDynamic), &m_pD3D92DTexture, NULL);
			if (FAILED(hr))
			{
				Assert(0);
				return;
			}
		}
		
		int iPixelStride = RenderUtil::GetTexStride(m_formatDesc.tex_format);
		for (int i = 0 ; i < pixelData.generate_level; i++)
		{
			if (!pixelData.level_data[i].ptr_pixel)
			{
				continue;
			}
			D3DSURFACE_DESC dc;
			m_pD3D92DTexture->GetLevelDesc(i, &dc);
			D3DLOCKED_RECT lockRc;
			m_pD3D92DTexture->LockRect(i, &lockRc, NULL, 0);
			//int iPixBufferCount = pixelData.level_data[i].width * pixelData.level_data[i].height * iPixelStride;
			//memcpy(lockRc.pBits, pixelData.level_data->ptr_pixel, iPixelStride);
			xbyte* pix_buffer = (xbyte*)lockRc.pBits;
			xint32 lock_max_pix = lockRc.Pitch / iPixelStride;
			xint32 pix_pitch = pixelData.level_data[i].width * iPixelStride;
			for (int j = 0; j < dc.Height && j < pixelData.level_data[i].height; j++)
			{
				memcpy(pix_buffer + j * lockRc.Pitch, pixelData.level_data[i].ptr_pixel + j * pix_pitch, xMin(lock_max_pix * iPixelStride, pix_pitch));
			}
			m_pD3D92DTexture->UnlockRect(i);
		}
		
	}
}

void XD3D9TextureCube::UpdateAsset(XAssetMonitor* pMonitor)
{
	if (m_bDirty)
	{
		m_bDirty = false;

		IDirect3DDevice9* pD3D9Device = (IDirect3DDevice9*)x_ptr_render_context->GetRenderContext();
		if (!pD3D9Device)
		{
			Assert(0);
			return;
		}

		bool bReCreate = false;

		if (m_pD3D9CubeTexture)
		{
			xbool bRelease = false;
			D3DSURFACE_DESC desc[X_CUBEMAP_FACE_NUM];
			m_pD3D9CubeTexture->GetLevelDesc(0, desc);
			for (int i = 0; i < X_CUBEMAP_FACE_NUM; i++)
			{
				if (desc[i].Width != m_formatDesc.width
				|| desc[i].Height != m_formatDesc.height
				|| desc[i].Pool != (D3DPOOL)RenderUtil::GetAssetManageType(m_bDynamic)
				|| desc[i].Format != (D3DFORMAT)RenderUtil::GetTexFormat(m_formatDesc.tex_format)
				)
				{
					bRelease = true;
				}
			}

			if (bRelease)
			{
			}

		}

	}
}

void XD3D9VertexPool::UpdateAsset(XAssetMonitor* pMonitor)
{
	if ( (!m_pD3D9VertexBuffer || m_bDirty) && m_VertexPoolDesc.buffer )
	{
		m_bDirty = false;

		if (m_pD3D9VertexBuffer)
		{
			m_pD3D9VertexBuffer->Release();
			m_pD3D9VertexBuffer = NULL;
		}

		IDirect3DDevice9* pD3D9Device = (IDirect3DDevice9*)x_ptr_render_context->GetRenderContext();
		if (!pD3D9Device)
		{
			return;
		}

		HRESULT hr = pD3D9Device->CreateVertexBuffer(m_VertexPoolDesc.count, 0, 0, (D3DPOOL)RenderUtil::GetAssetManageType(m_bDynamic), &m_pD3D9VertexBuffer, NULL);
		if (FAILED(hr))
		{
			Assert(0);
			return;
		}
		void* pData = NULL;
		m_pD3D9VertexBuffer->Lock(0, 0, (void**)&pData, 0);
		memcpy(pData, m_VertexPoolDesc.buffer, m_VertexPoolDesc.count);
		m_pD3D9VertexBuffer->Unlock();
	}
}

void XD3D9IndexPool::UpdateAsset(XAssetMonitor* pMonitor)
{
	if ( (!m_pD3D9IndexBuffer || m_bDirty) && m_IndexPoolDesc.buffer )
	{
		m_bDirty = false;
		if (m_pD3D9IndexBuffer)
		{
			m_pD3D9IndexBuffer->Release();
			m_pD3D9IndexBuffer = NULL;
		}

		IDirect3DDevice9* pD3D9Device = (IDirect3DDevice9*)x_ptr_render_context->GetRenderContext();
		if (!pD3D9Device)
		{
			return;
		}

		HRESULT hr = pD3D9Device->CreateIndexBuffer(m_IndexPoolDesc.count, 0, (D3DFORMAT)RenderUtil::GetIndexBits(m_IndexPoolDesc.type), (D3DPOOL)RenderUtil::GetAssetManageType(m_bDynamic), &m_pD3D9IndexBuffer, NULL);
		if (FAILED(hr))
		{
			Assert(0);
			return;
		}
		void* pData = NULL;
		m_pD3D9IndexBuffer->Lock(0, 0, (void**)&pData, 0);
		memcpy(pData, m_IndexPoolDesc.buffer, m_IndexPoolDesc.count);
		m_pD3D9IndexBuffer->Unlock();
	}
}

void XD3D9VertexAttribute::UpdateAsset(XAssetMonitor* pMonitor)
{
	if ( !m_pD3D9VertexDeclaration )
	{
		IDirect3DDevice9* pD3D9Device = (IDirect3DDevice9*)x_ptr_render_context->GetRenderContext();
		if (!pD3D9Device)
		{
			return;
		}
		const XVertexAttributeDesc& vad = m_VertexAttributeDesc;
		D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE] = {D3DDECL_END(),};
		int cur_ele = 0;
		for (int i = 0; i < vad.vecVertexElement.size(); i++)
		{
			for (int j = 0; j < vad.vecVertexElement[i].vecElement.size(); j++)
			{
				const Vertex_Decl_Element& ele = vad.vecVertexElement[i].vecElement[j];
				D3DVERTEXELEMENT9 elem = 
				{
					vad.vecVertexElement[i].stream_index,
					ele.offset,
					RenderUtil::GetDeclType(ele.data_type),
					ele.method_normalized,
					RenderUtil::GetDeclUsage(ele.usage),
					ele.stream
				};
				memcpy(&decl[cur_ele++], &elem, sizeof(D3DVERTEXELEMENT9));
				if (cur_ele == MAX_FVF_DECL_SIZE -1)
				{
					break;
				}
			}
		}
		D3DVERTEXELEMENT9 end = D3DDECL_END();
		memcpy(&decl[cur_ele], &end, sizeof(D3DVERTEXELEMENT9));
		
		HRESULT hr = pD3D9Device->CreateVertexDeclaration(decl, &m_pD3D9VertexDeclaration);
		if (FAILED(hr))
		{
			Assert(0);
			return;
		}
	}
}

void XD3D9ShaderParamTable::SetValue(const char* name, XTexture* texture)
{
	
	const ShaderConstDesc* ptr_desc = GetConstDesc(name);
	if (ptr_desc)
	{
		if(!texture)
			x_ptr_d3ddevice->SetTexture(ptr_desc->reg_index, NULL);
		else
		{
			switch(texture->m_emAssetType)
			{
			case ASSET_TEXTURE_2D:
				x_ptr_d3ddevice->SetTexture(ptr_desc->reg_index, ((XD3D9Texture2D*)texture)->GetD3D9Texture());
				break;
			case ASSET_TEXTURE_CUBE:
				x_ptr_d3ddevice->SetTexture(ptr_desc->reg_index, ((XD3D9TextureCube*)texture)->GetD3D9CubeTexture());
				break;
			case  ASSET_TEXTURE_RENDER:
				x_ptr_d3ddevice->SetTexture(ptr_desc->reg_index, ((XD3D9RenderTarget*)texture)->GetD3D9RenderTarget());
				break;
			default:
				x_ptr_d3ddevice->SetTexture(ptr_desc->reg_index, NULL);
				break;
			}
		}
	}
}

void XD3D9ShaderParamTable::SetValue(const char* name, const void* val, const int size)
{
	const ShaderConstDesc* ptr_desc = GetConstDesc(name);
	if (ptr_desc)
	{
		m_pD3DXConstantTable->SetValue(x_ptr_d3ddevice, (D3DXHANDLE)ptr_desc->handle, val, size);
	}
}

void XD3D9ShaderParamTable::SetValue(const char* name, const float* vals, const int count)
{
	const ShaderConstDesc* ptr_desc = GetConstDesc(name);
	if (ptr_desc)
	{
		m_pD3DXConstantTable->SetFloatArray(x_ptr_d3ddevice, (D3DXHANDLE)ptr_desc->handle, vals, count);
	}
}

void XD3D9ShaderParamTable::SetValue(const char* name, const float val)
{
	//SetValue(name, &val, 1);
	const ShaderConstDesc* ptr_desc = GetConstDesc(name);
	if (ptr_desc)
	{
		m_pD3DXConstantTable->SetFloat(x_ptr_d3ddevice, (D3DXHANDLE)ptr_desc->handle, val);
	}
}

void XD3D9ShaderParamTable::SetValue(const char* name, const int* vals, const int count)
{
	const ShaderConstDesc* ptr_desc = GetConstDesc(name);
	if (ptr_desc)
	{
		m_pD3DXConstantTable->SetIntArray(x_ptr_d3ddevice, (D3DXHANDLE)ptr_desc->handle, vals, count);
	}
}

void XD3D9ShaderParamTable::SetValue(const char* name, const int val)
{
	const ShaderConstDesc* ptr_desc = GetConstDesc(name);
	if (ptr_desc)
	{
		m_pD3DXConstantTable->SetInt(x_ptr_d3ddevice, (D3DXHANDLE)ptr_desc->handle, val);
	}
}

void XD3D9ShaderParamTable::SetValue(const char* name, const xbool* vals, const int count)
{
	const ShaderConstDesc* ptr_desc = GetConstDesc(name);
	if (ptr_desc)
	{
		m_pD3DXConstantTable->SetBoolArray(x_ptr_d3ddevice, (D3DXHANDLE)ptr_desc->handle, (const BOOL*)vals, count);
	}
}

void XD3D9ShaderParamTable::SetValue(const char* name, const xbool val)
{
	const ShaderConstDesc* ptr_desc = GetConstDesc(name);
	if (ptr_desc)
	{
		m_pD3DXConstantTable->SetBool(x_ptr_d3ddevice, (D3DXHANDLE)ptr_desc->handle, val);
	}
}



void XD3D9VertexShader::UpdateAsset(XAssetMonitor* pMonitor)
{
	if (!m_pD3D9VertexShader)
	{
		if (m_pShaderParamTable) 
		{
			delete m_pShaderParamTable;
			m_pShaderParamTable = NULL;
		}
		m_pShaderParamTable = new XD3D9ShaderParamTable;
		if (!m_pShaderParamTable)
		{
			Assert(0);
			return;
		}
		IDirect3DDevice9* pD3D9Device = (IDirect3DDevice9*)x_ptr_render_context->GetRenderContext();
		if (!pD3D9Device)
		{
			return;
		}
		
		D3DXMACRO macro[VertexShaderDesc::MAX_SHADER_MARCO] = {NULL,};
		for (int i = 0; i < m_VertexShaderDesc.marcos.size(); i++)
		{
			macro[i].Name = m_VertexShaderDesc.marcos[i].name.c_str();
			macro[i].Definition = m_VertexShaderDesc.marcos[i].definition.c_str();
		}
		int flag = D3DXSHADER_OPTIMIZATION_LEVEL3;
#ifdef _DEBUG
		flag = D3DXSHADER_DEBUG;
#endif
		ID3DXBuffer* pShaderBuffer = NULL;
		ID3DXBuffer* pErrorBuffer = NULL;
		HRESULT hr = D3DXCompileShader(m_VertexShaderDesc.shader_src.c_str(), 
			m_VertexShaderDesc.shader_src.length(), macro, NULL, 
			m_VertexShaderDesc.entry.c_str(), m_VertexShaderDesc.profile.c_str(), 
			flag, &pShaderBuffer, &pErrorBuffer, &(((XD3D9ShaderParamTable*)m_pShaderParamTable)->m_pD3DXConstantTable)
			);
		
		if (pErrorBuffer)
		{
			if(pShaderBuffer)
			{
				pShaderBuffer->Release();
			}
			const char* err_info = (const char*)pErrorBuffer->GetBufferPointer();
			return;
		}

		if (FAILED(hr))
		{
			Assert(0);
			return;
		}

		hr = pD3D9Device->CreateVertexShader((DWORD*)pShaderBuffer->GetBufferPointer(), &m_pD3D9VertexShader);
		if (FAILED(hr))
		{
			pShaderBuffer->Release();
			return;
		}
		pShaderBuffer->Release();
	}
}

void XD3D9PixelShader::UpdateAsset(XAssetMonitor* pMonitor)
{
	if (!m_pD3D9PixelShader)
	{
		IDirect3DDevice9* pD3D9Device = (IDirect3DDevice9*)x_ptr_render_context->GetRenderContext();
		if (!pD3D9Device)
		{
			return;
		}

		D3DXMACRO macro[VertexShaderDesc::MAX_SHADER_MARCO] = {NULL,};
		for (int i = 0; i < m_PixelShaderDesc.marcos.size(); i++)
		{
			macro[i].Name = m_PixelShaderDesc.marcos[i].name.c_str();
			macro[i].Definition = m_PixelShaderDesc.marcos[i].definition.c_str();
		}
		int flag = D3DXSHADER_OPTIMIZATION_LEVEL3;
#ifdef _DEBUG
		flag = D3DXSHADER_DEBUG;
#endif
		ID3DXBuffer* pShaderBuffer = NULL;
		ID3DXBuffer* pErrorBuffer = NULL;
		HRESULT hr = D3DXCompileShader(m_PixelShaderDesc.shader_src.c_str(), 
			m_PixelShaderDesc.shader_src.length(), macro, NULL, 
			m_PixelShaderDesc.entry.c_str(), m_PixelShaderDesc.profile.c_str(), 
			flag, &pShaderBuffer, &pErrorBuffer, &(((XD3D9ShaderParamTable*)m_pShaderParamTable)->m_pD3DXConstantTable)
			);

		if (pErrorBuffer)
		{
			if(pShaderBuffer)
			{
				pShaderBuffer->Release();
			}
			const char* err_info = (const char*)pErrorBuffer->GetBufferPointer();
			return;
		}

		if (FAILED(hr))
		{
			Assert(0);
			return;
		}

		hr = pD3D9Device->CreatePixelShader((DWORD*)pShaderBuffer->GetBufferPointer(), &m_pD3D9PixelShader);
		if (FAILED(hr))
		{
			pShaderBuffer->Release();
			return;
		}
		pShaderBuffer->Release();
	}
}