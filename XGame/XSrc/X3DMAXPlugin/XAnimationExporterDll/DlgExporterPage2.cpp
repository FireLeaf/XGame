// DlgExporterPage2.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "XAnimationExporterDll.h"
#include "DlgExporterPage2.h"
#include "DlgInsertAC.h"

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
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ANIMATION, &CDlgExporterPage2::OnLvnItemchangedListAnimation)
END_MESSAGE_MAP()


// CDlgExporterPage2 ��Ϣ�������

void CDlgExporterPage2::OnBnClickedButtonAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDlgInsertAC dlg(this);        // ����Ի�����CAdditionDlg�Ķ���dlg   
	dlg.SetAction(CDlgInsertAC::NEW_ACTION);
	int nResponse = dlg.DoModal();   // �����Ի���dlg������DoModal�����ķ���ֵ���˳�ʱ�����ť��ID����ֵ��nResponse   
	if (nResponse == IDOK)               // �жϷ���ֵ�Ƿ�ΪOK��ť����IDΪIDOK���������Ѿ�����ɾ����   
	{
		UpdateKeyFrameList();
	}
}

void CDlgExporterPage2::OnBnClickedButtonEdit()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//m_listKeyFrame.GetFirstSelectedItemPosition();
	CDlgInsertAC dlg(this);        // ����Ի�����CAdditionDlg�Ķ���dlg   
	dlg.SetAction(CDlgInsertAC::MODIFY_ACTION);
	int nResponse = dlg.DoModal();   // �����Ի���dlg������DoModal�����ķ���ֵ���˳�ʱ�����ť��ID����ֵ��nResponse   
	if (nResponse == IDOK)               // �жϷ���ֵ�Ƿ�ΪOK��ť����IDΪIDOK���������Ѿ�����ɾ����   
	{
		UpdateKeyFrameList();
	}
}

void CDlgExporterPage2::OnBnClickedButtonDel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CDlgExporterPage2::OnLvnItemchangedListAnimation(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

void CDlgExporterPage2::UpdateKeyFrameList()
{
	m_listKeyFrame.DeleteAllItems();

}

BOOL CDlgExporterPage2::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//Ϊ����ListCtrl������չ����
	DWORD dwStyle = m_listKeyFrame.GetExtendedStyle(); 
	dwStyle |= LVS_EX_GRIDLINES;//show grid 
	dwStyle |= LVS_EX_FULLROWSELECT;//select one row
	//	dwStyle |= LVS_EX_CHECKBOXES;//add check box with each row
	m_listKeyFrame.SetExtendedStyle(dwStyle);
	
	m_listKeyFrame.InsertColumn(0,"������",LVCFMT_LEFT,180);
	m_listKeyFrame.InsertColumn(1,"��ʼ֡",LVCFMT_RIGHT,120);
	m_listKeyFrame.InsertColumn(2,"����֡",LVCFMT_LEFT,120);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
