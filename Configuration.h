#if !defined(AFX_CONFIGURATION_H__E35BE86B_00F0_11D5_A1ED_0090000873F3__INCLUDED_)
#define AFX_CONFIGURATION_H__E35BE86B_00F0_11D5_A1ED_0090000873F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Configuration.h : header file
//
#include "resource.h"
#include "ConfigComPort.h"
/////////////////////////////////////////////////////////////////////////////
// CConfiguration dialog

class CConfiguration : public CDialog
{
// Construction

public:
	CConfiguration(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfiguration)
	enum { IDD = IDD_CONFIGURATION_DIALOG };
	CButton	m_WorkSpace;
	CTreeCtrl	m_Tree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfiguration)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	CConfigComPort *ComPortDlg;

	// Generated message map functions
	//{{AFX_MSG(CConfiguration)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangedTreeOptions(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGURATION_H__E35BE86B_00F0_11D5_A1ED_0090000873F3__INCLUDED_)
