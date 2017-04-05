#if !defined(AFX_CONFIGCOMPORT_H__B0AE7B53_7FC8_4412_A541_F14ED3AA0A21__INCLUDED_)
#define AFX_CONFIGCOMPORT_H__B0AE7B53_7FC8_4412_A541_F14ED3AA0A21__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigComPort.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigComPort dialog

class CConfigComPort : public CDialog
{
// Construction
public:
	CConfigComPort(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfigComPort)
	enum { IDD = IDD_CONFIG_COMPORT };
	CString	m_BaudRate;
	CString	m_ByteSize;
	CString	m_Nb;
	CString	m_Parity;
	CString	m_StopBits;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigComPort)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigComPort)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGCOMPORT_H__B0AE7B53_7FC8_4412_A541_F14ED3AA0A21__INCLUDED_)
