
// GraduationProject.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CGraduationProjectApp: 
// �йش����ʵ�֣������ GraduationProject.cpp
//

class CGraduationProjectApp : public CWinApp
{
public:
	CGraduationProjectApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CGraduationProjectApp theApp;