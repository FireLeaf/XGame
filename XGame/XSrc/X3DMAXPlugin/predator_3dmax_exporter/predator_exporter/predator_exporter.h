// predator_exporter.h : predator_exporter DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cpredator_exporterApp
// �йش���ʵ�ֵ���Ϣ������� predator_exporter.cpp
//

class Cpredator_exporterApp : public CWinApp
{
public:
	Cpredator_exporterApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
