// Test.h : main header file for the TEST application
//

#if !defined(AFX_TEST_H__09FA0A79_BC79_4226_9FEE_4F613E829E30__INCLUDED_)
#define AFX_TEST_H__09FA0A79_BC79_4226_9FEE_4F613E829E30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "SkinMagicLib.h"

// 注意，如果MFC是动态链接到工程中的，则应该选择SkinMagicLibMD6Trial.lib库

#pragma comment(lib, "SkinMagicTrial.lib")

/////////////////////////////////////////////////////////////////////////////
// CTestApp:
// See Test.cpp for the implementation of this class
//

class CTestApp : public CWinApp
{
public:
	CTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEST_H__09FA0A79_BC79_4226_9FEE_4F613E829E30__INCLUDED_)
