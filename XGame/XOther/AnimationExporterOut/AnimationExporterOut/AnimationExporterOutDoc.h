// AnimationExporterOutDoc.h : CAnimationExporterOutDoc ��Ľӿ�
//


#pragma once


class CAnimationExporterOutDoc : public CDocument
{
protected: // �������л�����
	CAnimationExporterOutDoc();
	DECLARE_DYNCREATE(CAnimationExporterOutDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CAnimationExporterOutDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


