// DlgExporterPage2.cpp : 实现文件
//

#include "stdafx.h"
#include "XAnimationExporterDll.h"
#include "DlgExporterPage2.h"
#include "DlgInsertAC.h"

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
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ANIMATION, &CDlgExporterPage2::OnLvnItemchangedListAnimation)
END_MESSAGE_MAP()


// CDlgExporterPage2 消息处理程序

void CDlgExporterPage2::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgInsertAC dlg(this);        // 定义对话框类CAdditionDlg的对象dlg   
	dlg.SetAction(CDlgInsertAC::NEW_ACTION);
	int nResponse = dlg.DoModal();   // 弹出对话框dlg，并将DoModal函数的返回值（退出时点击按钮的ID）赋值给nResponse   
	if (nResponse == IDOK)               // 判断返回值是否为OK按钮（其ID为IDOK，鸡啄米已经将它删除）   
	{
		UpdateKeyFrameList();
	}
}

void CDlgExporterPage2::OnBnClickedButtonEdit()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_listKeyFrame.GetFirstSelectedItemPosition();
	CDlgInsertAC dlg(this);        // 定义对话框类CAdditionDlg的对象dlg   
	dlg.SetAction(CDlgInsertAC::MODIFY_ACTION);
	int nResponse = dlg.DoModal();   // 弹出对话框dlg，并将DoModal函数的返回值（退出时点击按钮的ID）赋值给nResponse   
	if (nResponse == IDOK)               // 判断返回值是否为OK按钮（其ID为IDOK，鸡啄米已经将它删除）   
	{
		UpdateKeyFrameList();
	}
}

void CDlgExporterPage2::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CDlgExporterPage2::OnLvnItemchangedListAnimation(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CDlgExporterPage2::UpdateKeyFrameList()
{
	m_listKeyFrame.DeleteAllItems();

}

BOOL CDlgExporterPage2::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//为两个ListCtrl设置扩展属性
	DWORD dwStyle = m_listKeyFrame.GetExtendedStyle(); 
	dwStyle |= LVS_EX_GRIDLINES;//show grid 
	dwStyle |= LVS_EX_FULLROWSELECT;//select one row
	//	dwStyle |= LVS_EX_CHECKBOXES;//add check box with each row
	m_listKeyFrame.SetExtendedStyle(dwStyle);
	
	m_listKeyFrame.InsertColumn(0,"动画名",LVCFMT_LEFT,180);
	m_listKeyFrame.InsertColumn(1,"起始帧",LVCFMT_RIGHT,120);
	m_listKeyFrame.InsertColumn(2,"结束帧",LVCFMT_LEFT,120);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
