// AnimationExporterOutView.cpp : CAnimationExporterOutView ���ʵ��
//

#include "stdafx.h"
#include "AnimationExporterOut.h"

#include "AnimationExporterOutDoc.h"
#include "AnimationExporterOutView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAnimationExporterOutView

IMPLEMENT_DYNCREATE(CAnimationExporterOutView, CView)

BEGIN_MESSAGE_MAP(CAnimationExporterOutView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CAnimationExporterOutView ����/����

CAnimationExporterOutView::CAnimationExporterOutView()
{
	// TODO: �ڴ˴���ӹ������

}

CAnimationExporterOutView::~CAnimationExporterOutView()
{
}

BOOL CAnimationExporterOutView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CAnimationExporterOutView ����

void CAnimationExporterOutView::OnDraw(CDC* /*pDC*/)
{
	CAnimationExporterOutDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}


// CAnimationExporterOutView ��ӡ

BOOL CAnimationExporterOutView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CAnimationExporterOutView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CAnimationExporterOutView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CAnimationExporterOutView ���

#ifdef _DEBUG
void CAnimationExporterOutView::AssertValid() const
{
	CView::AssertValid();
}

void CAnimationExporterOutView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAnimationExporterOutDoc* CAnimationExporterOutView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAnimationExporterOutDoc)));
	return (CAnimationExporterOutDoc*)m_pDocument;
}
#endif //_DEBUG


// CAnimationExporterOutView ��Ϣ�������
