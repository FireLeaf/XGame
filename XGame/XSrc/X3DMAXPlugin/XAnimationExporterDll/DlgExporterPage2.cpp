// DlgExporterPage2.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "XAnimationExporterDll.h"
#include "DlgExporterPage2.h"


// CDlgExporterPage2 �Ի���

IMPLEMENT_DYNAMIC(CDlgExporterPage2, CDialog)

CDlgExporterPage2::CDlgExporterPage2(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgExporterPage2::IDD, pParent)
{

}

CDlgExporterPage2::~CDlgExporterPage2()
{
}

void CDlgExporterPage2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ANIMATION, m_listKeyFrame);
}


BEGIN_MESSAGE_MAP(CDlgExporterPage2, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CDlgExporterPage2::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_EDIT, &CDlgExporterPage2::OnBnClickedButtonEdit)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CDlgExporterPage2::OnBnClickedButtonDel)
END_MESSAGE_MAP()


// CDlgExporterPage2 ��Ϣ�������

void CDlgExporterPage2::OnBnClickedButtonAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CDlgExporterPage2::OnBnClickedButtonEdit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CDlgExporterPage2::OnBnClickedButtonDel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
