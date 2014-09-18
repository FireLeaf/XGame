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
		if (m_pD3D92DTexture)
		{
			m_pD3D92DTexture->Release();
			m_pD3D92DTexture = NULL;
		}

		IDirect3DDevice9* pD3D9Device = (IDirect3DDevice9*)x_ptr_render_context->GetRenderContext();
		if (!pD3D9Device)
		{
			Assert(0);
			return;
		}

		HRESULT hr = pD3D9Device->CreateTexture(m_formatDesc.width, m_formatDesc.height, pixelData.generate_level, 0, (D3DFORMAT)RenderUtil::GetTexFormat(m_formatDesc.tex_format), (D3DPOOL)RenderUtil::GetAssetManageType(m_bDynamic), &m_pD3D92DTexture, NULL);
		if (FAILED(hr))
		{
			Assert(0);
			return;
		}

		int iPixelStride = RenderUtil::GetTexStride(m_formatDesc.tex_format);
		for (int i = 0 ; i < pixelData.generate_level; i++)
		{
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

void XD3D9VertexPool::UpdateAsset(XAssetMonitor* pMonitor)
{
	if ( (!m_pD3D9VertexBuffer || m_bDirty) && m_VertexPoolDesc.buffer )
	{
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

		D3DVERTEXELEMENT9 decl[MAX_FVF_DECL_SIZE] = {D3DDECL_END(),};
		int cur_ele = 0;
		for (int i = 0; i < vecVertexElement.size(); i++)
		{
			for (int j = 0; j < vecVertexElement[i].vecElement.size(); j++)
			{
				const VertexElement::Element& ele = vecVertexElement[i].vecElement[j];
				D3DVERTEXELEMENT9 elem = 
				{
					vecVertexElement[i].stream_index,
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

void XD3D9VertexShader::UpdateAsset(XAssetMonitor* pMonitor)
{
	if (!m_pD3D9VertexShader)
	{
		IDirect3DDevice9* pD3D9Device = (IDirect3DDevice9*)x_ptr_render_context->GetRenderContext();
		if (!pD3D9Device)
		{
			return;
		}
		
		//D3DXCompileShader(m_VertexShaderDesc.shader_src.c_str(), m_VertexShaderDesc.shader_src.length())

	}
}