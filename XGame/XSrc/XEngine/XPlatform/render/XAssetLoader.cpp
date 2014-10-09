/**************************************************************************
/*		FileName: XAssetLoader.cpp
/*		FileDesc: 
/*		FileAuthor: YangCao
/*		CreateTime: 2014/09/21 11:18:29
/*		CopyRight: yc 
/*************************************************************************/

#include "XAssetLoader.h"
#include "XFile.h"
#include "XMacro.h"
#include "XTemplate.h"

//BMP文件头
const char BMP_HEADER[]="BM";

//TGA非压缩文件头
const char TGA_UHEADER[]={0,0,2 ,0,0,0,0,0,0,0,0,0};

//TGA压缩文件头
const char TGA_CHEADER[]={0,0,10,0,0,0,0,0,0,0,0,0};

bool XAssetLoader::LoadAsset(XAsset* pAsset)
{
	if (!pAsset)
	{
		return false;
	}

	switch(pAsset->GetAssetType())
	{
	case ASSET_TEXTURE_2D:
		return Load2DTexture(pAsset);
		break;
	case ASSET_TEXTURE_CUBE:
		return LoadCubeTexture(pAsset);
		break;
	default:
		break;
	}
	return false;
}

/*
************************************************
************************************************
************************************************
************************************************
************************************************
************************************************
************************************************
|
|/
这里是第一个像素，往左边走
*/
void VFlip(unsigned char * ucpData, unsigned int uiHeight, unsigned int uiWidth, unsigned int uiBpp)
{
	unsigned char * ucpCopy = new unsigned char[uiWidth * uiHeight * uiBpp];
	if(!ucpCopy)
		return;
	for(unsigned int i = 0; i < uiHeight; i++)
		memcpy(ucpCopy + (uiWidth * uiBpp * i), ucpData + (uiWidth * uiBpp * (uiHeight - i-1/*注意这里要减一*/)), uiWidth * uiBpp);
	memcpy(ucpData, ucpCopy, uiWidth * uiHeight * uiBpp);
	delete [] ucpCopy;
}

bool LoadTGA(XFileMap& fm, XTextureData& data)
{
	xulong length = fm.Length();

	xbyte* pByteImageBuffer =new xbyte[length];

	if(pByteImageBuffer==NULL)
	{
		return false;
	}

	if (length != fm.Read(pByteImageBuffer, 1, length))
	{
		delete []pByteImageBuffer;
		return false;
	}

	bool bCompressed = false;
	if(memcmp(pByteImageBuffer,TGA_UHEADER,12)==0)
	{
		
	}
	else if(memcmp(pByteImageBuffer,TGA_CHEADER,12)==0)
	{
		bCompressed = true;
	}
	else
	{
		return false;
	}

	xuint32 uiBpp = 0;
	xuint32 uiType = 0;//GL_RGB,或者GL_RGBA
	xuint32 uiImageSize = 0;
	xbyte *pByteBuffer = pByteImageBuffer;
	xbyte *pByteData = NULL;

	//跳过固定序列,12个字节
	pByteBuffer += 12;

	//获取图片宽高
	int imWidth = (pByteBuffer[1] << 8) + pByteBuffer[0];//其实就是当前位置的那个字数据，两个字节
	int imHeight = (pByteBuffer[3] << 8) + pByteBuffer[2];//同理，第二个字

	uiBpp=pByteBuffer[4];//像素的位数，24，32等，实际上位数占了两个字节，但是现在位数没有超过128所以只加载一个字节即可
	pByteBuffer+=6;//指针步进6个字节

	XAssetLoader::DATATYPE dt;
	if(uiBpp==24)
		dt = XAssetLoader::RGBDATA;
	else if(uiBpp==32)
		dt = XAssetLoader::RGBADATA;
	else
	{
		delete []pByteImageBuffer;
		pByteImageBuffer=NULL;
		return false;
	}

	//图像的总大小，不是文件哦
	uiImageSize = imHeight * imWidth * (uiBpp / 8);

	pByteData = new xbyte[uiImageSize];

	if(!pByteData)
	{
		delete []pByteImageBuffer;
		return false;
	}

	/*
	tga有压缩和非压缩两种格式
	其中非压缩的就全部数据都是像素
	压缩的使用的是RLE压缩算法
	取第一个字节的最高位，假如是0，那么表示有（pixelNum=第一个字节的低6位+1）个不同颜色的像素值，
	也就是说其后面有pixelNum个都是颜色数据
	假如是1，那么表示，后面一个像素数据是重复的，重复次数是pixelNum次
	*/
	
	//如果是非压缩的
	if(!bCompressed)
	{
		for(xuint32 i=0;i<uiImageSize;i+=(uiBpp/8))
		{
			//BRG-->RGB
			pByteData[i]=pByteBuffer[i+2];
			pByteData[i+1]=pByteBuffer[i+1];
			pByteData[i+2]=pByteBuffer[i];

			if(uiBpp==32)
				pByteData[i+3]=pByteBuffer[i+3];
		}

	}//压缩的数据
	else
	{
		xuint32 uiDataPos=0;
		do 
		{
			//连续的不重复像素颜色值
			if(pByteBuffer[0]<128)
			{
				//取出不连续像素的总位数
				xuint32 uiCount=(pByteBuffer[0]+1)*(uiBpp/8);
				pByteBuffer++;
				for(xuint32 i=0;i<uiCount;i+=(uiBpp/8))
				{
					pByteData[uiDataPos]=pByteBuffer[2];
					pByteData[uiDataPos+1]=pByteBuffer[1];
					pByteData[uiDataPos+2]=pByteBuffer[0];

					pByteBuffer+=3;
					uiDataPos+=3;

					if(uiBpp==32)
					{
						pByteData[uiDataPos]=pByteBuffer[3];

						pByteBuffer++;
						uiDataPos++;
					}
				}
			}
			else
			{
				//取出重复次数
				xuint32 uiCount=(pByteBuffer[0]-127)*(uiBpp/8);
				pByteBuffer++;

				for(xuint32 i=0;i<uiCount;i+=(uiBpp/8))
				{
					pByteData[uiDataPos]=pByteBuffer[2];
					pByteData[uiDataPos+1]=pByteBuffer[1];
					pByteData[uiDataPos+2]=pByteBuffer[0];
					
					uiDataPos+=3;
					if(uiBpp==32)
					{
						pByteData[uiDataPos]=pByteBuffer[3];
						uiDataPos++;
					}	
				}

				pByteBuffer+=3;
				if(uiBpp==32)
					pByteBuffer++;
			}

		} while (uiDataPos<uiImageSize);
	}
	
	VFlip(pByteData, imHeight, imWidth, uiBpp/8);

	for (int i = 0; i < data.generate_level; i++)
	{
		data.level_data[i].width = xMax(imWidth / (i + 1), 1);
		data.level_data[i].height = xMax(imHeight / (i + 1), 1);
	//	bool b
		data.level_data[i].ptr_pixel = new xbyte*[data.level_data[i].height];
		if (!data.level_data[i].ptr_pixel)
		{
			delete []pByteData;
			delete []pByteImageBuffer;
			return false;
		}
		for (int j = 0; j < data.level_data[i].height; j++)
		{
			data.level_data[i].ptr_pixel[j] = new xbyte[data.level_data[i].width];
		}
	}
	delete []pByteData;
	delete []pByteImageBuffer;

	return true;
}

bool XAssetLoader::Load2DTexture(XAsset* pAsset)
{

	if (!pAsset || 
		ASSET_TEXTURE_2D != pAsset->GetAssetType()||
		TEXTURE_LOAD_FROM_FILE != ((XTexture2D*)pAsset)->GetFormatDesc().from
		)
	{
		return false;
	}
	XTexture2D* ptr_texture_2d = (XTexture2D*)pAsset;
	XFileMap fm;
	if(!fm.OpenFile(ptr_texture_2d->GetPixelData().m_textureFile.c_str(), "rb"))
	{
		return false;
	}
#ifdef _WIN_PLATFORM
	if(LoadTGA(fm, ptr_texture_2d->GetTextureData()))
	{
		return false;
	}
#elif defined(_XOS_PLATFORM)
#endif

	return true;
}

bool XAssetLoader::LoadCubeTexture(XAsset* pAsset)
{
	return true;
}