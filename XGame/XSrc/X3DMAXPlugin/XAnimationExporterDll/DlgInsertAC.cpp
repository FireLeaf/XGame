// DlgInsertAC.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "XAnimationExporterDll.h"
#include "DlgInsertAC.h"
#include "MeshExporter.h"

// CDlgInsertAC �Ի���

IMPLEMENT_DYNAMIC(CDlgInsertAC, CDialog)

CDlgInsertAC::CDlgInsertAC(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInsertAC::IDD, pParent)
{
	m_iAction = NONE_ACTION;
}

CDlgInsertAC::~CDlgInsertAC()
{
}

void CDlgInsertAC::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgInsertAC, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgInsertAC::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgInsertAC::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgInsertAC ��Ϣ�������

void CDlgInsertAC::SetAction(int iAction, std::string name/* = ""*/, int iStarFrame/* = 0*/, int iEndFrame/* = 0*/)
{
	m_iAction = iAction;
	m_name = name;
	m_iStartFrame = iStarFrame;
	m_iEndFrame = iEndFrame;
}

void CDlgInsertAC::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//UpdateData(FALSE);
	CString strName;
	int iStartFrame = 0, iEndFrame = 0;
	GetDlgItemText(IDC_EDIT_NAME, strName);
	iStartFrame = GetDlgItemInt(IDC_EDIT_START_FRAME);
	iEndFrame = GetDlgItemInt(IDC_EDIT_END_FRAME);
	std::string name = strName.GetBuffer(0);
	if (iStartFrame > iEndFrame)
	{
		MessageBox("����֡����С�ڿ�ʼ֡", "����", MB_OK);
		return;
	}
	if (NEW_ACTION == m_iAction)
	{
		XMeshExporter::Get().AddActionClip(name, iStartFrame, iEndFrame);
	}
	else if (MODIFY_ACTION == m_iAction)
	{
		XMeshExporter::Get().UpdateActionClip(name, iStartFrame, iEndFrame);
	}

	OnOK();
}

void CDlgInsertAC::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

BOOL CDlgInsertAC::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetDlgItemText(IDC_EDIT_NAME, m_name.c_str());
	SetDlgItemInt(IDC_EDIT_START_FRAME, m_iStartFrame);
	SetDlgItemInt(IDC_EDIT_END_FRAME, m_iEndFrame);

	GetDlgItem(IDC_EDIT_NAME)->EnableWindow(m_iAction == NEW_ACTION);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
