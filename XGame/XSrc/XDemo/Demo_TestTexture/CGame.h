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
	IDirect3DDevice9 *m_pDevice;//d3d设备对象
	D3DPRESENT_PARAMETERS m_D3Dpp;//设备对象参数
	typedef bool (*Render)(IDirect3DDevice9 *pDevice,float timeDelta);
	HWND m_hWnd;//进程句柄
	HINSTANCE m_hInstance;//进程示例
	bool m_isDeviceLost;//设备是否丢失
	bool InitWindows();//初始化窗口
	bool Init3DDevice(bool windowed);//初始化设备对象
public:

	CGame();//构造函数

	bool Init(HINSTANCE hInstance,bool windowed);//初始化函数
	IDirect3DDevice9 *GetD3DDevice()
	{
		return m_pDevice;
	}
	bool LostDevice();//设备丢失时处理函数
	bool GenDevice();//获取丢失的设备
	bool Show(float timeDelta);//显示
	bool SetUp();//游戏加载或进行配置
	int MesLoop();//游戏循环
	bool Update(float timeDelta);//更新游戏
	LRESULT CALLBACK WindowProc(//进程消息处理函数
		HWND hwnd,      // handle to window
		UINT uMsg,      // message identifier
		WPARAM wParam,  // first message parameter
		LPARAM lParam   // second message parameter
		);

	bool Clean();//游戏清理
	~CGame();//析构

};
#endif