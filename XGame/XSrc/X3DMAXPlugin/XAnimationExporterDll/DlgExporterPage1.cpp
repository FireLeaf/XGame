// DlgExporterPage1.cpp : 实现文件
//

#include "stdafx.h"
#include "XAnimationExporterDll.h"
#include "DlgExporterPage1.h"
#include "MeshExporterUtil.h"

// CDlgExporterPage1 对话框

IMPLEMENT_DYNAMIC(CDlgExporterPage1, CDialog)

CDlgExporterPage1::CDlgExporterPage1(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgExporterPage1::IDD, pParent)
{
	m_iBoneNum = 0;
}

CDlgExporterPage1::~CDlgExporterPage1()
{
}

void CDlgExporterPage1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_SKELTON, m_boneTreeCtrl);
	DDX_Control(pDX, IDC_CHECK_INCLUDE_DUMMY, m_ckIncludeDummy);
}


BEGIN_MESSAGE_MAP(CDlgExporterPage1, CDialog)
	ON_BN_CLICKED(IDC_CHECK_INCLUDE_DUMMY, &CDlgExporterPage1::OnBnClickedCheckIncludeDummy)
END_MESSAGE_MAP()


// CDlgExporterPage1 消息处理程序

BOOL CDlgExporterPage1::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	BuildBoneSkelton();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgExporterPage1::InsertBoneNode(INode* pBoneNode, HTREEITEM hParentItem)
{
	if (!pBoneNode)
	{
		return;
	}
	m_iBoneNum++;
	HTREEITEM hItem = m_boneTreeCtrl.InsertItem(pBoneNode->GetName(), hParentItem);
	for (int i = 0; i < pBoneNode->NumberOfChildren(); i++)
	{
		INode* pNode = pBoneNode->GetChildNode(i);
		if (pNode)
		{
			InsertBoneNode(pNode, hItem);
		}
	}
	if (TVI_ROOT == hParentItem)
	{
		m_boneTreeCtrl.Expand(hItem, TVE_EXPAND);
	}
}
void CDlgExporterPage1::OnBnClickedCheckIncludeDummy()
{
	// TODO: 在此添加控件通知处理程序代码
	BuildBoneSkelton();
}

void CDlgExporterPage1::BuildBoneSkelton()
{
	m_boneTreeCtrl.DeleteAllItems();
	m_iBoneNum = 0;
	INode* pNode = MeshExporterUtil::GetRootBoneNode(m_ckIncludeDummy.GetCheck() ? 1 : 0);
	InsertBoneNode(pNode, TVI_ROOT);
	SetDlgItemInt(IDC_EDIT_BONE_NUM, m_iBoneNum);
}