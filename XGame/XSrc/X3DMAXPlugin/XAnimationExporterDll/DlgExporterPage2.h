#pragma once
#include "afxcmn.h"


// CDlgExporterPage2 �Ի���

class CDlgExporterPage2 : public CDialog
{
	DECLARE_DYNAMIC(CDlgExporterPage2)

public:
	CDlgExporterPage2(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgExporterPage2();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };
protected:
	void UpdateKeyFrameList();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonEdit();
	afx_msg void OnBnClickedButtonDel();
	CListCtrl m_listKeyFrame;
	afx_msg void OnLvnItemchangedListAnimation(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
};
