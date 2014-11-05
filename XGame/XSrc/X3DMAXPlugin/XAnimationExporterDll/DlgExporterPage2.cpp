// DlgExporterPage2.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "XAnimationExporterDll.h"
#include "DlgExporterPage2.h"
#include "DlgInsertAC.h"
#include "MeshExporter.h"

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
	ON_NOTIFY(HDN_ITEMDBLCLICK, 0, &CDlgExporterPage2::OnHdnItemdblclickListAnimation)
	ON_EN_CHANGE(IDC_EDIT_FRAME_RATE, &CDlgExporterPage2::OnEnChangeEditFrameRate)
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
	POSITION pss = m_listKeyFrame.GetFirstSelectedItemPosition();
	if (!pss)
	{
		return;
	}

	int iCurFirstIndex = m_listKeyFrame.GetNextSelectedItem(pss);
	if (iCurFirstIndex < 0 || iCurFirstIndex >= m_listKeyFrame.GetItemCount())
	{
		return;
	}
	
	CString name = m_listKeyFrame.GetItemText(iCurFirstIndex, 0);
	CString number = m_listKeyFrame.GetItemText(iCurFirstIndex, 1);
	int iStartFrame = atoi(number.GetBuffer(0));
	number = m_listKeyFrame.GetItemText(iCurFirstIndex, 2);
	int iEndFrame = atoi(number.GetBuffer(0));
	CDlgInsertAC dlg(this);        // ����Ի�����CAdditionDlg�Ķ���dlg   
	dlg.SetAction(CDlgInsertAC::MODIFY_ACTION, std::string(name.GetBuffer(0)), iStartFrame, iEndFrame);
	int nResponse = dlg.DoModal();   // �����Ի���dlg������DoModal�����ķ���ֵ���˳�ʱ�����ť��ID����ֵ��nResponse   
	if (nResponse == IDOK)               // �жϷ���ֵ�Ƿ�ΪOK��ť����IDΪIDOK���������Ѿ�����ɾ����   
	{
		UpdateKeyFrameList();
	}
}

void CDlgExporterPage2::OnBnClickedButtonDel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION pss = m_listKeyFrame.GetFirstSelectedItemPosition();
	if (!pss)
	{
		return;
	}

	int iCurFirstIndex = m_listKeyFrame.GetNextSelectedItem(pss);
	if (iCurFirstIndex < 0 || iCurFirstIndex >= m_listKeyFrame.GetItemCount())
	{
		return;
	}

	if (IDOK == MessageBox("ȷ��ɾ���˶���Ƭ��", "����", MB_OKCANCEL))
	{
		CString name = m_listKeyFrame.GetItemText(iCurFirstIndex, 0);
		XMeshExporter::Get().DeleteActionClip(std::string(name.GetBuffer(0)));
		UpdateKeyFrameList();
	}
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
	const XMeshExporter::ActionClipVector& acv = XMeshExporter::Get().GetActionClipVector();
	for (int i = 0; i < (int)acv.size(); i++)
	{
		m_listKeyFrame.InsertItem(i, acv[i].strClipName.c_str());
		CString strInfo;
		strInfo.Format("%d", acv[i].iStartFrame);
		m_listKeyFrame.SetItemText(i, 1, strInfo.GetBuffer(0));
		strInfo.Format("%d", acv[i].iEndFrame);
		m_listKeyFrame.SetItemText(i, 2, strInfo.GetBuffer(0));
	}
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

	SetDlgItemInt(IDC_EDIT_FRAME_RATE, XMeshExporter::Get().GetActionSampleRate());
	
	m_listKeyFrame.InsertColumn(0,"������",LVCFMT_LEFT,180);
	m_listKeyFrame.InsertColumn(1,"��ʼ֡",LVCFMT_RIGHT,120);
	m_listKeyFrame.InsertColumn(2,"����֡",LVCFMT_LEFT,120);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDlgExporterPage2::OnHdnItemdblclickListAnimation(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}

void CDlgExporterPage2::OnEnChangeEditFrameRate()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	XMeshExporter::Get().SetActionSampleRate(GetDlgItemInt(IDC_EDIT_FRAME_RATE));

}
