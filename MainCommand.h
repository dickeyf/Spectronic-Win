#if !defined(AFX_MAINCOMMAND_H__A0DB92F3_E005_4C5B_B59C_1D12DFCA0E8C__INCLUDED_)
#define AFX_MAINCOMMAND_H__A0DB92F3_E005_4C5B_B59C_1D12DFCA0E8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainCommand.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainCommand dialog

class CMainCommand : public CDialog
{
// Construction
public:
	CMainCommand(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMainCommand)
	enum { IDD = IDD_MAIN_COMMANDS };
	CButton	m_Delete;
	CButton	m_Send;
	CListCtrl	m_List;
	CButton	m_Add;
	CString	m_Command;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainCommand)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMainCommand)
	afx_msg void OnSendcommand();
	virtual void OnOK();
	afx_msg void OnChangeCommand();
	afx_msg void OnAdd();
	virtual BOOL OnInitDialog();
	afx_msg void OnDelete();
	afx_msg void OnClickList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINCOMMAND_H__A0DB92F3_E005_4C5B_B59C_1D12DFCA0E8C__INCLUDED_)
