#pragma once


// CDlgExportPage3 对话框

class CDlgExportPage3 : public CDialog
{
	DECLARE_DYNAMIC(CDlgExportPage3)

public:
	CDlgExportPage3(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgExportPage3();

// 对话框数据
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
