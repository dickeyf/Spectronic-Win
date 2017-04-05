// SubDlg.cpp: implementation of the CSubDlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Spectronic.h"
#include "SubDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSubDlg::CSubDlg()
{
	ASSERT(0);	// Never use this constructor
}

CSubDlg::CSubDlg(UINT id, CWnd *pParent)
: CDialog(id)
{
	ID = id;
}

CSubDlg::~CSubDlg()	{}

BOOL CSubDlg::PreTranslateMessage(MSG* pMsg)
{
	// Don't let CDialog process the Escape key.
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE))
	{
		return TRUE;
	}
	
	// Don't let CDialog process the Return key, if a multi-line edit has focus
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
	{
		// Special case: if control with focus is an edit control with
		// ES_WANTRETURN style, let it handle the Return key.

		TCHAR szClass[10];
		CWnd* pWndFocus = GetFocus();
		if (((pWndFocus = GetFocus()) != NULL) &&
			IsChild(pWndFocus) &&
			(pWndFocus->GetStyle() & ES_WANTRETURN) &&
			GetClassName(pWndFocus->m_hWnd, szClass, 10) &&
			(lstrcmpi(szClass, _T("EDIT")) == 0))
		{
			pWndFocus->SendMessage(WM_CHAR, pMsg->wParam, pMsg->lParam);
			return TRUE;
		}

		return FALSE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CSubDlg::OnOK()
{
	EndDialog(IDOK);
}

void CSubDlg::OnCancel()
{
	EndDialog(IDCANCEL);
}