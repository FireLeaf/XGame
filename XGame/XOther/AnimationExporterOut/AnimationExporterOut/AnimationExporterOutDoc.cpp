// AnimationExporterOutDoc.cpp : CAnimationExporterOutDoc ���ʵ��
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


// CAnimationExporterOutDoc ����/����

CAnimationExporterOutDoc::CAnimationExporterOutDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CAnimationExporterOutDoc::~CAnimationExporterOutDoc()
{
}

BOOL CAnimationExporterOutDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CAnimationExporterOutDoc ���л�

void CAnimationExporterOutDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CAnimationExporterOutDoc ���

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


// CAnimationExporterOutDoc ����
