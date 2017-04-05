#if !defined(AFX_MAXDLG_H__0E46D300_3E9C_4917_AF98_A89E35936534__INCLUDED_)
#define AFX_MAXDLG_H__0E46D300_3E9C_4917_AF98_A89E35936534__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
// MaxDlg.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CMaxDlg dialog

class CMaxDlg : public CDialog
{
// Construction
public:
	CMaxDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMaxDlg)
	enum { IDD = IDD_MAXDLG };
	CListCtrl	m_List;
	CString	m_Xy;
	double	m_Begin;
	double	m_End;
	CString	m_Max;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMaxDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMaxDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAXDLG_H__0E46D300_3E9C_4917_AF98_A89E35936534__INCLUDED_)
