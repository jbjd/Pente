
// Poker.h : main header file for the Poker application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CPokerApp:
// See Poker.cpp for the implementation of this class
//

class CPokerApp : public CWinApp
{
private:
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken = 0;
public:
	CPokerApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CPokerApp theApp;
