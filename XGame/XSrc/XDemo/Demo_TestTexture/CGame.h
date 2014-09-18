#ifndef _CGAME_H
#define _CGAME_H
#include <d3dx9.h>
#include <d3d9.h>
// IDirect3DDevice9 *GolbalGetDevice(){
// 	return 
// }

//bool (*ptr_display)(float timeDelta)
class CGame
{
private:
	IDirect3DDevice9 *m_pDevice;//d3d�豸����
	D3DPRESENT_PARAMETERS m_D3Dpp;//�豸�������
	typedef bool (*Render)(IDirect3DDevice9 *pDevice,float timeDelta);
	HWND m_hWnd;//���̾��
	HINSTANCE m_hInstance;//����ʾ��
	bool m_isDeviceLost;//�豸�Ƿ�ʧ
	bool InitWindows();//��ʼ������
	bool Init3DDevice(bool windowed);//��ʼ���豸����
public:

	CGame();//���캯��

	bool Init(HINSTANCE hInstance,bool windowed);//��ʼ������
	IDirect3DDevice9 *GetD3DDevice()
	{
		return m_pDevice;
	}
	bool LostDevice();//�豸��ʧʱ������
	bool GenDevice();//��ȡ��ʧ���豸
	bool Show(float timeDelta);//��ʾ
	bool SetUp();//��Ϸ���ػ��������
	int MesLoop();//��Ϸѭ��
	bool Update(float timeDelta);//������Ϸ
	LRESULT CALLBACK WindowProc(//������Ϣ������
		HWND hwnd,      // handle to window
		UINT uMsg,      // message identifier
		WPARAM wParam,  // first message parameter
		LPARAM lParam   // second message parameter
		);

	bool Clean();//��Ϸ����
	~CGame();//����

};
#endif