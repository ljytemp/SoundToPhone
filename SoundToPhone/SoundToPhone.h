
// SoundToPhone.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CSoundToPhoneApp:
// �йش����ʵ�֣������ SoundToPhone.cpp
//

class CSoundToPhoneApp : public CWinApp
{
public:
	CSoundToPhoneApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CSoundToPhoneApp theApp;