#pragma once
#include "afxcmn.h"


// DlgAnimationExporterMain 对话框

class DlgAnimationExporterMain : public CDialog
{
	DECLARE_DYNAMIC(DlgAnimationExporterMain)

public:
	DlgAnimationExporterMain(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgAnimationExporterMain();

// 对话框数据
	enum { IDD = IDD_DIALOG_MAIN };
	
	enum
	{
		MAX_PAGE = 4,
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
protected:
	CDialog* m_pDlgPages[MAX_PAGE];
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	CTabCtrl m_tabExporter;
	virtual BOOL DestroyWindow();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedButtonExport();
};
