
// Alice.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CGalGameThesongofdestroyApp: 
// �йش����ʵ�֣������ Alice.cpp
//

class CGalGameThesongofdestroyApp : public CWinApp
{
public:
	CGalGameThesongofdestroyApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CGalGameThesongofdestroyApp theApp;
