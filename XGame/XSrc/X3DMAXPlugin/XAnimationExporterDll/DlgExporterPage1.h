#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDlgExporterPage1 �Ի���

class CDlgExporterPage1 : public CDialog
{
	DECLARE_DYNAMIC(CDlgExporterPage1)

public:
	CDlgExporterPage1(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgExporterPage1();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
protected:
	void InsertBoneNode(INode* pBoneNode, HTREEITEM hParentItem);
	void BuildBoneSkelton();
public:
	virtual BOOL OnInitDialog();
	CTreeCtrl m_boneTreeCtrl;
	CButton m_ckIncludeDummy;
	int		m_iBoneNum;
	afx_msg void OnBnClickedCheckIncludeDummy();
};
