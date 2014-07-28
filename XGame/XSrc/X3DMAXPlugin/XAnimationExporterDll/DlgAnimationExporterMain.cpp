// DlgAnimationExporterMain.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "XAnimationExporterDll.h"
#include "DlgAnimationExporterMain.h"
#include "DlgExporterPage1.h"
#include "DlgExporterPage2.h"
#include "DlgExportPage3.h"
#include "DlgExportPage4.h"

// DlgAnimationExporterMain �Ի���

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


// DlgAnimationExporterMain ��Ϣ�������

BOOL DlgAnimationExporterMain::OnInitDialog()
{
	CDialog::OnInitDialog();
	UpdateData(FALSE);//
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	char* szItemName[] = {" �������", "��������", "����", "ģ����Ϣ", "5"};
	
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
	// �쳣: OCX ����ҳӦ���� FALSE
}

void DlgAnimationExporterMain::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ����ר�ô����/����û���
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

	// TODO: �ڴ˴������Ϣ����������
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
