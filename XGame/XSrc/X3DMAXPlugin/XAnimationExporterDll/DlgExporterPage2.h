#pragma once
#include "afxcmn.h"


// CDlgExporterPage2 对话框

class CDlgExporterPage2 : public CDialog
{
	DECLARE_DYNAMIC(CDlgExporterPage2)

public:
	CDlgExporterPage2(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgExporterPage2();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };
protected:
	void UpdateKeyFrameList();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonEdit();
	afx_msg void OnBnClickedButtonDel();
	CListCtrl m_listKeyFrame;
	afx_msg void OnLvnItemchangedListAnimation(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
};
