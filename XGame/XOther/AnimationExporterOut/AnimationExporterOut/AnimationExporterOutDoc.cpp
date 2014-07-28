// AnimationExporterOutDoc.cpp : CAnimationExporterOutDoc 类的实现
//

#include "stdafx.h"
#include "AnimationExporterOut.h"

#include "AnimationExporterOutDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAnimationExporterOutDoc

IMPLEMENT_DYNCREATE(CAnimationExporterOutDoc, CDocument)

BEGIN_MESSAGE_MAP(CAnimationExporterOutDoc, CDocument)
END_MESSAGE_MAP()


// CAnimationExporterOutDoc 构造/析构

CAnimationExporterOutDoc::CAnimationExporterOutDoc()
{
	// TODO: 在此添加一次性构造代码

}

CAnimationExporterOutDoc::~CAnimationExporterOutDoc()
{
}

BOOL CAnimationExporterOutDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CAnimationExporterOutDoc 序列化

void CAnimationExporterOutDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CAnimationExporterOutDoc 诊断

#ifdef _DEBUG
void CAnimationExporterOutDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CAnimationExporterOutDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CAnimationExporterOutDoc 命令
