#include"CDrawText.h"
#include <stdio.h>

CDrawText::CDrawText(IDirect3DDevice9 *Device,int height,int width){
	ZeroMemory(&m_dfd,sizeof(D3DXFONT_DESC));
	m_dfd.Height=height;
	m_dfd.Width=width;
	m_dfd.CharSet=DEFAULT_CHARSET;
	strcpy(m_dfd.FaceName,"华文彩云");
	m_dfd.Italic=0;
	m_dfd.MipLevels=0;
	m_dfd.OutputPrecision=0;
	m_dfd.PitchAndFamily=0;
	m_dfd.Quality=0;
	m_dfd.Weight=500;
	m_pDevice=Device;

	m_dwFrameNum=0;
	this->m_fTimeStamp=0;
	memset(m_szFPSINFO,0,40);

	m_rc.left=10;
	m_rc.top=10;
	m_rc.bottom=45;
	m_rc.right=230;
	
	m_textRc.left=10;
	m_textRc.top=500;
	m_textRc.right=610;
	m_textRc.bottom=530;

	if(m_pDevice==NULL){
		PostQuitMessage(0);
	}
	D3DXCreateFontIndirect(m_pDevice,&m_dfd,&m_pFont);

	if(m_pFont==NULL){
		PostQuitMessage(0);
	}
}

void CDrawText::drawText(char *str,RECT *rect,int len,D3DCOLOR color,DWORD format,LPD3DXSPRITE pSprite){
	if(m_pFont)
	{
		if(rect)
			m_pFont->DrawTextA(pSprite,str,len,rect,format,color);
		else
			m_pFont->DrawTextA(pSprite,str,len,&m_textRc,format,color);
	}else{
		MessageBox(NULL,"字体创建失败","提示",MB_OK);
	}
}

RECT TEXTRECT(byte a) {
	RECT rc={25,a*30,725,(a+1)*30};
	return rc;
}

void CDrawText::drawText(char *str,byte a,int len,D3DCOLOR color,DWORD format,LPD3DXSPRITE pSprite)
{
	if(m_pFont)
	{
		RECT rc=TEXTRECT(a);
		m_pFont->DrawTextA(pSprite,str,len,&rc,format,color);

	}else{
		MessageBox(NULL,"字体创建失败","提示",MB_OK);
	}
}


void CDrawText::FPS(float timeDelta){
	
	float GAMEFPS=0;
	
	m_dwFrameNum++;
	m_fTimeStamp+=timeDelta;
	if(m_fTimeStamp>=1.0f){
		GAMEFPS=(float)m_dwFrameNum/m_fTimeStamp;
		m_fTimeStamp=0.0f;
		m_dwFrameNum=0;
		
		sprintf(m_szFPSINFO,"当前FPS为:%5.2f",GAMEFPS);
	}

	this->drawText(m_szFPSINFO,&m_rc);
	
}


void CDrawText::LostDevice(){
	m_pFont->OnLostDevice();
}

void CDrawText::GenDevice(){
	m_pFont->OnResetDevice();
}