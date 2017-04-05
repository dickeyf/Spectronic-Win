// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__485DCB70_F0AA_11D4_A1ED_0090000873F3__INCLUDED_)
#define AFX_STDAFX_H__485DCB70_F0AA_11D4_A1ED_0090000873F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#define SP_CHAR WM_USER

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls

#endif // _AFX_NO_AFXCMN_SUPPORT

//Spectronic 1201 stuff
#include "Spectro.h"

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <list>
#include <map>
#include <string>
#include <math.h>

using namespace std;

#pragma warning(disable:4786)		// Warning de nom trop long


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__485DCB70_F0AA_11D4_A1ED_0090000873F3__INCLUDED_)
