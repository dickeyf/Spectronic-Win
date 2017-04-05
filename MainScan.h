#if !defined(AFX_MAINSCAN_H__27450F4F_D391_4CB0_86D2_3BFE1F688267__INCLUDED_)
#define AFX_MAINSCAN_H__27450F4F_D391_4CB0_86D2_3BFE1F688267__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainScan.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainScan dialog

class CMainScan : public CDialog
{
// Construction
public:
	CMainScan(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMainScan)
	enum { IDD = IDD_MAIN_SCAN };
	double	m_WvInc;
	int		m_Speed;
	double	m_StopWv;
	double	m_StrtWv;
	CString	m_Time;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainScan)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMainScan)
		virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINSCAN_H__27450F4F_D391_4CB0_86D2_3BFE1F688267__INCLUDED_)
