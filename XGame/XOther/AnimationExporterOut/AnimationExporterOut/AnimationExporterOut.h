// AnimationExporterOut.h : AnimationExporterOut Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CAnimationExporterOutApp:
// �йش����ʵ�֣������ AnimationExporterOut.cpp
//

class CAnimationExporterOutApp : public CWinApp
{
public:
	CAnimationExporterOutApp();


// ��д
public:
	virtual BOOL InitInstance();

// ʵ��
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CAnimationExporterOutApp theApp;