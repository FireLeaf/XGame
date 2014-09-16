/**************************************************************************
/*		FileName: XRenderUtil.h
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/16 22:31:59
/*		CopyRight: yc 
/*************************************************************************/

#ifdef _WIN_PLATFORM
#include "XRenderUtil.h"
#include "XAsset.h"

namespace RenderUtil
{
	int GetTexFormat(int format)
	{
		switch(format)
		{
		case XTEX_FORMAT_A8R8G8B8:
			return D3DFMT_X8R8G8B8;
			break;
		case XTEX_FORMAT_A16R16G16B16:
			break;
		default:
			break;
		}

		return D3DFMT_UNKNOWN;
	}

	int GetTexStride(int format)
	{
		switch(format)
		{
		case XTEX_FORMAT_A8R8G8B8:
			return 4;
			break;
		case XTEX_FORMAT_A16R16G16B16:
			break;
		default:
			break;
		}

		return 0;
	}
}

#endif