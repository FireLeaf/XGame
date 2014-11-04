#pragma once


// CDlgInsertAC �Ի���

class CDlgInsertAC : public CDialog
{
	DECLARE_DYNAMIC(CDlgInsertAC)
public:
	enum
	{
		NONE_ACTION,
		NEW_ACTION,
		MODIFY_ACTION,
	};
public:
	CDlgInsertAC(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgInsertAC();

// �Ի�������
	enum { IDD = IDD_DIALOG_INSERT_AC };
public:
	void SetAction(int iAction, std::string name = "", int iStarFrame = 0, int iEndFrame = 0);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
protected:
	int m_iAction;
	std::string m_name;
	int m_iStartFrame;
	int m_iEndFrame;
public:
	virtual BOOL OnInitDialog();
};
