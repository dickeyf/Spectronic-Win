#if !defined(AFX_MAININFO_H__A5765030_4674_4CF6_8A60_808705214F13__INCLUDED_)
#define AFX_MAININFO_H__A5765030_4674_4CF6_8A60_808705214F13__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainInfo dialog

class CMainInfo : public CDialog
{
// Construction
public:
	std::string GetName();
	CMainInfo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMainInfo)
	enum { IDD = IDD_MAIN_INFO };
	CString	m_Date;
	CString	m_Groupe;
	CString	m_Name;
	CString	m_Substance;
	CString	m_Title;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CMainInfo)
		virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAININFO_H__A5765030_4674_4CF6_8A60_808705214F13__INCLUDED_)
