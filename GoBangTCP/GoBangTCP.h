
// GoBangTCP.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CGoBangTCPApp:
// �йش����ʵ�֣������ GoBangTCP.cpp
//

class CGoBangTCPApp : public CWinApp
{
public:
	CGoBangTCPApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CGoBangTCPApp theApp;