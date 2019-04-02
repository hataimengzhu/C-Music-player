// Test.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Test.h"
#include "TestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestApp

BEGIN_MESSAGE_MAP(CTestApp, CWinApp)
	//{{AFX_MSG_MAP(CTestApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestApp construction

CTestApp::CTestApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTestApp object

CTestApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTestApp initialization

BOOL CTestApp::InitInstance()
{

	//===SkinMagic===


       // 初始化SkinMagic库

       VERIFY(InitSkinMagicLib(AfxGetInstanceHandle(), "MonitorDir", NULL, NULL));

       // 从资源中加载皮肤文件。也可以用代码“LoadSkinFile("corona.smf")”直接从文件中加载

       if(LoadSkinFromResource(AfxGetInstanceHandle(), (LPCTSTR)IDR_SKINMAGIC1, "SKINMAGIC"))

       {

              // 设置对话框默认皮肤

              SetDialogSkin("Dialog"); 


       }


	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CTestDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CTestApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	//===SkinMagic===


       // 释放SkinMagic库申请的内存

       ExitSkinMagicLib();


       //===SkinMagic===


	return CWinApp::ExitInstance();
}
