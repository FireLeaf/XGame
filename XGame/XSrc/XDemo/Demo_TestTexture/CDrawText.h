#ifndef _CDRAWTEXT_H
#define _CDRAWTEXT_H
#include<d3dx9.h>



class CDrawText{
private:
	ID3DXFont *m_pFont;//����
	IDirect3DDevice9 *m_pDevice;//�豸����
	D3DXFONT_DESC m_dfd;//����������
	RECT m_rc;//FPS����ʾ������
	RECT m_textRc;//һ��������ʾ����
	DWORD m_dwFrameNum;//����ʾ֡��
	float m_fTimeStamp;//ʱ���
	char m_szFPSINFO[40];//FPS��Ϣ
public:
	CDrawText(IDirect3DDevice9 *device,int height=25,int width=10);

	//����������Ϣ��������
	void drawText(char *str,RECT *rect=NULL,int len=-1,D3DCOLOR color=0xFFFF0000,DWORD format=DT_CENTER,LPD3DXSPRITE pSprite=NULL);
	void drawText(char *str,byte a=10,int len=-1,D3DCOLOR color=0xFFFF0000,DWORD format=DT_CENTER,LPD3DXSPRITE pSprite=NULL);
	
	//���㲢����FPS
	void FPS(float timeDelta);

	//�豸��ʧ�����
	void LostDevice();
	void GenDevice();
};
#endif