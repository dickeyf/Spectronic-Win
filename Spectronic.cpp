// Spectronic.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Spectronic.h"
#include "SpectronicDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpectronicApp

BEGIN_MESSAGE_MAP(CSpectronicApp, CWinApp)
	//{{AFX_MSG_MAP(CSpectronicApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpectronicApp construction

CSpectronicApp::CSpectronicApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSpectronicApp object

CSpectronicApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSpectronicApp initialization

BOOL CSpectronicApp::InitInstance()
{
	AfxEnableControlContainer();
	using namespace std;

	// Standard initialization

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CSpectronicDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
