#ifndef _CDRAWTEXT_H
#define _CDRAWTEXT_H
#include<d3dx9.h>



class CDrawText{
private:
	ID3DXFont *m_pFont;//字体
	IDirect3DDevice9 *m_pDevice;//设备对象
	D3DXFONT_DESC m_dfd;//字体描述符
	RECT m_rc;//FPS所显示的区域
	RECT m_textRc;//一般文字显示区域
	DWORD m_dwFrameNum;//已显示帧数
	float m_fTimeStamp;//时间戳
	char m_szFPSINFO[40];//FPS信息
public:
	CDrawText(IDirect3DDevice9 *device,int height=25,int width=10);

	//根据所给信息画出文字
	void drawText(char *str,RECT *rect=NULL,int len=-1,D3DCOLOR color=0xFFFF0000,DWORD format=DT_CENTER,LPD3DXSPRITE pSprite=NULL);
	void drawText(char *str,byte a=10,int len=-1,D3DCOLOR color=0xFFFF0000,DWORD format=DT_CENTER,LPD3DXSPRITE pSprite=NULL);
	
	//计算并画出FPS
	void FPS(float timeDelta);

	//设备丢失及获得
	void LostDevice();
	void GenDevice();
};
#endif