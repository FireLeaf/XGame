// XAnimationExporterDll.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "XAnimationExporterDll.h"
#include "DlgAnimationExporterMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CXAnimationExporterDllApp

BEGIN_MESSAGE_MAP(CXAnimationExporterDllApp, CWinApp)
END_MESSAGE_MAP()


// CXAnimationExporterDllApp ����

CXAnimationExporterDllApp::CXAnimationExporterDllApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CXAnimationExporterDllApp ����

CXAnimationExporterDllApp theApp;


// CXAnimationExporterDllApp ��ʼ��

BOOL CXAnimationExporterDllApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}


extern "C" __declspec(dllexport) int	DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CWnd* cWnd=AfxGetApp()->GetMainWnd();
	DlgAnimationExporterMain daem(cWnd);
	daem.DoModal();
	return TRUE;
}
