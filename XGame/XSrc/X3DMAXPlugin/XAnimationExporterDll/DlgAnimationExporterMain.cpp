// DlgAnimationExporterMain.cpp : 实现文件
//

#include "stdafx.h"
#include "XAnimationExporterDll.h"
#include "DlgAnimationExporterMain.h"
#include "DlgExporterPage1.h"
#include "DlgExporterPage2.h"
#include "DlgExportPage3.h"
#include "DlgExportPage4.h"

// DlgAnimationExporterMain 对话框

#define CREATE_PAGE(id, dlgname) pDlg = new dlgname;\
											if (!pDlg || !pDlg->Create(id, GetDlgItem(IDC_TAB1)))\
											{\
												return FALSE;\
											}\
											pDlg->MoveWindow(&rcw);\
											m_pDlgPages[iDlgNum++] = pDlg;

IMPLEMENT_DYNAMIC(DlgAnimationExporterMain, CDialog)

DlgAnimationExporterMain::DlgAnimationExporterMain(CWnd* pParent /*=NULL*/)
	: CDialog(DlgAnimationExporterMain::IDD, pParent)
{
	for (int i = 0; i < MAX_PAGE; i++)
	{
		m_pDlgPages[i] = NULL;
	}
}

DlgAnimationExporterMain::~DlgAnimationExporterMain()
{
}

void DlgAnimationExporterMain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tabExporter);
}


BEGIN_MESSAGE_MAP(DlgAnimationExporterMain, CDialog)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &DlgAnimationExporterMain::OnTcnSelchangeTab1)
	ON_WM_MOVE()
END_MESSAGE_MAP()


// DlgAnimationExporterMain 消息处理程序

BOOL DlgAnimationExporterMain::OnInitDialog()
{
	CDialog::OnInitDialog();
	UpdateData(FALSE);//
	// TODO:  在此添加额外的初始化

	char* szItemName[] = {" 骨骼框架", "动画序列", "纹理", "模型信息", "5"};
	
	for (int i = 0; i < MAX_PAGE; i++)
	{
		m_tabExporter.InsertItem(i, szItemName[i]);
	}

	RECT rc;
	m_tabExporter.GetClientRect(&rc);
	ClientToScreen(&rc);
	RECT rcDlg = {0,0,0,0};
	//GetClientRect(&rcDlg);
	//GetWindowRect(&rcDlg);
	//ClientToScreen(&rcDlg);
	RECT rcw = {rcDlg.left + rc.left, rcDlg.top + rc.top + 21, rcDlg.left + rc.right, rcDlg.top + rc.bottom};
	CDialog* pDlg = NULL;
	int iDlgNum = 0;
	CREATE_PAGE(IDD_DIALOG1, CDlgExporterPage1);
	pDlg->ShowWindow(SW_SHOW);
	CREATE_PAGE(IDD_DIALOG2, CDlgExporterPage2);
	CREATE_PAGE(IDD_DIALOG3, CDlgExportPage3);
	CREATE_PAGE(IDD_DIALOG4, CDlgExportPage4);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void DlgAnimationExporterMain::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	int iCurIndex = m_tabExporter.GetCurSel();
	for (int i = 0; i < MAX_PAGE && i < m_tabExporter.GetItemCount(); i++)
	{
		if (m_pDlgPages[i])
		{
			m_pDlgPages[i]->ShowWindow(i == iCurIndex ? SW_SHOW : SW_HIDE);
		}
	}
}

BOOL DlgAnimationExporterMain::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	for (int i = 0; i < MAX_PAGE; i++)
	{
		if (m_pDlgPages[i])
		{
			delete m_pDlgPages[i];
		}
	}
	return CDialog::DestroyWindow();
}

void DlgAnimationExporterMain::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

	// TODO: 在此处添加消息处理程序代码
	if (m_tabExporter.m_hWnd)
	{
		RECT rc;
		m_tabExporter.GetClientRect(&rc);
		ClientToScreen(&rc);
		RECT rcDlg = {0,0,0,0};
		//GetClientRect(&rcDlg);
		//GetWindowRect(&rcDlg);
		//ClientToScreen(&rcDlg);
		RECT rcw = {rcDlg.left + rc.left + 35, rcDlg.top + rc.top + 51, rcDlg.left + rc.right, rcDlg.top + rc.bottom};
		for (int i = 0; i < MAX_PAGE; i++)
		{
			if (m_pDlgPages[i])
			{
				m_pDlgPages[i]->MoveWindow(&rcw);
			}
		}
	}
}
