// AnimationExporterOutView.h : CAnimationExporterOutView 类的接口
//


#pragma once


class CAnimationExporterOutView : public CView
{
protected: // 仅从序列化创建
	CAnimationExporterOutView();
	DECLARE_DYNCREATE(CAnimationExporterOutView)

// 属性
public:
	CAnimationExporterOutDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CAnimationExporterOutView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // AnimationExporterOutView.cpp 中的调试版本
inline CAnimationExporterOutDoc* CAnimationExporterOutView::GetDocument() const
   { return reinterpret_cast<CAnimationExporterOutDoc*>(m_pDocument); }
#endif

