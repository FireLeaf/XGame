#pragma once


// CDlgExportPage3 �Ի���

class CDlgExportPage3 : public CDialog
{
	DECLARE_DYNAMIC(CDlgExportPage3)

public:
	CDlgExportPage3(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgExportPage3();

// �Ի�������
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
