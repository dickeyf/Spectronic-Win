#if !defined(AFX_MAINDEFAULTDLG_H__2AB11ECC_7997_4366_A62F_E790FE8B54C6__INCLUDED_)
#define AFX_MAINDEFAULTDLG_H__2AB11ECC_7997_4366_A62F_E790FE8B54C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainDefaultDlg.h : header file
//
#include "ListLog.h"

/////////////////////////////////////////////////////////////////////////////
// CMainDefaultDlg dialog

class CMainDefaultDlg : public CDialog
{
// Construction
public:
	CMainDefaultDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMainDefaultDlg)
	enum { IDD = IDD_MAIN_DEFAULT };
	CListLog	m_Log;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainDefaultDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMainDefaultDlg)
		// NOTE: the ClassWizard will add member functions here
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDEFAULTDLG_H__2AB11ECC_7997_4366_A62F_E790FE8B54C6__INCLUDED_)
