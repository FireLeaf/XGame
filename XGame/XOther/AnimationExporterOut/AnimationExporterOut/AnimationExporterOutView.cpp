// AnimationExporterOutView.cpp : CAnimationExporterOutView 类的实现
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CAnimationExporterOutView 构造/析构

CAnimationExporterOutView::CAnimationExporterOutView()
{
	// TODO: 在此处添加构造代码

}

CAnimationExporterOutView::~CAnimationExporterOutView()
{
}

BOOL CAnimationExporterOutView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CAnimationExporterOutView 绘制

void CAnimationExporterOutView::OnDraw(CDC* /*pDC*/)
{
	CAnimationExporterOutDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CAnimationExporterOutView 打印

BOOL CAnimationExporterOutView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CAnimationExporterOutView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CAnimationExporterOutView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CAnimationExporterOutView 诊断

#ifdef _DEBUG
void CAnimationExporterOutView::AssertValid() const
{
	CView::AssertValid();
}

void CAnimationExporterOutView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CAnimationExporterOutDoc* CAnimationExporterOutView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CAnimationExporterOutDoc)));
	return (CAnimationExporterOutDoc*)m_pDocument;
}
#endif //_DEBUG


// CAnimationExporterOutView 消息处理程序
