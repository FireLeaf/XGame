#pragma once


// CDlgExportPage4 �Ի���

class CDlgExportPage4 : public CDialog
{
	DECLARE_DYNAMIC(CDlgExportPage4)

public:
	CDlgExportPage4(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgExportPage4();

// �Ի�������
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
