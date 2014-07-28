// DlgExporterPage2.cpp : 实现文件
//

#include "stdafx.h"
#include "XAnimationExporterDll.h"
#include "DlgExporterPage2.h"


// CDlgExporterPage2 对话框

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


// CDlgExporterPage2 消息处理程序

void CDlgExporterPage2::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDlgExporterPage2::OnBnClickedButtonEdit()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDlgExporterPage2::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
}
