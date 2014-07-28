#pragma once


// CDlgExportPage4 对话框

class CDlgExportPage4 : public CDialog
{
	DECLARE_DYNAMIC(CDlgExportPage4)

public:
	CDlgExportPage4(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgExportPage4();

// 对话框数据
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
