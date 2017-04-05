#if !defined(AFX_MAINBLANK_H__76FCD546_65F2_43AF_BBE1_B6534158A771__INCLUDED_)
#define AFX_MAINBLANK_H__76FCD546_65F2_43AF_BBE1_B6534158A771__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainBlank.h : header file
//

#include "ListLog.h"
#include "CError.h"

/////////////////////////////////////////////////////////////////////////////
// CMainBlank dialog

class CMainBlank : public CDialog
{
// Construction
public:
	CMainBlank(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMainBlank)
	enum { IDD = IDD_MAIN_BLANK };
	CButton	m_Delete;
	CButton	m_SelCtrl;
	CListCtrl	m_List;
	CButton	m_Add;
	CButton	m_Browse;
	CEdit	m_PathCtrl;
	CEdit	m_NameCtrl;
	CString	m_Path;
	CString	m_Name;
	BOOL	m_Sel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainBlank)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMainBlank)
	afx_msg void OnBrowse();
	afx_msg void OnChangePath();
	afx_msg void OnChangeName();
	afx_msg void OnAjouter();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDelete();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINBLANK_H__76FCD546_65F2_43AF_BBE1_B6534158A771__INCLUDED_)
