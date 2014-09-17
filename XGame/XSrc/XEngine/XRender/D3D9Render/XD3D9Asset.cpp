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

		HRESULT hr = pD3D9Device->CreateTexture(m_formatDesc.width, m_formatDesc.height, pixelData.generate_level, 0, (D3DFORMAT)RenderUtil::GetTexFormat(m_formatDesc.tex_format), m_bDynamic ? D3DPOOL_DEFAULT : D3DPOOL_MANAGED, &m_pD3D92DTexture, NULL);
		if (FAILED(hr))
		{
			return;
		}
		
		int iPixelStride = RenderUtil::GetTexStride(m_formatDesc.tex_format);
		for (int i = 0 ; i < pixelData.generate_level; i++)
		{
			D3DLOCKED_RECT lockRc;
			m_pD3D92DTexture->LockRect(i, &lockRc, NULL, 0);
			int iPixBufferCount = pixelData.level_data[i].width * pixelData.level_data[i].height * iPixelStride;
			memcpy(lockRc.pBits, pixelData.level_data->ptr_pixel, iPixelStride);
			m_pD3D92DTexture->UnlockRect(i);
		}
		
	}
}