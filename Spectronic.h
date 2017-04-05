// Spectronic.h : main header file for the SPECTRONIC application
//

#if !defined(AFX_SPECTRONIC_H__485DCB6C_F0AA_11D4_A1ED_0090000873F3__INCLUDED_)
#define AFX_SPECTRONIC_H__485DCB6C_F0AA_11D4_A1ED_0090000873F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSpectronicApp:
// See Spectronic.cpp for the implementation of this class
//

class CSpectronicApp : public CWinApp
{
public:
	CSpectronicApp();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpectronicApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSpectronicApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPECTRONIC_H__485DCB6C_F0AA_11D4_A1ED_0090000873F3__INCLUDED_)
