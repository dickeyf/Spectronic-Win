#if !defined(AFX_MAINGRAPH_H__13F0F0ED_762C_4A95_A45C_42A79F85006D__INCLUDED_)
#define AFX_MAINGRAPH_H__13F0F0ED_762C_4A95_A45C_42A79F85006D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainGraph.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainGraph dialog

class CMainGraph : public CDialog
{
// Construction
public:
	CMainGraph(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMainGraph)
	enum { IDD = IDD_MAIN_GRAPH };
	CEdit	c_YUs;
	CEdit	c_YUp;
	CEdit	c_YMin;
	CEdit	c_YMax;
	CEdit	c_XUs;
	CEdit	c_XUp;
	CEdit	c_XMin;
	CEdit	c_XMax;
	CStatic	m_Grads;
	CStatic	m_Droite;
	CStatic	m_Back;
	BOOL	m_Color;
	BOOL	m_XAuto;
	BOOL	m_YAuto;
	double	m_XMax;
	double	m_XMin;
	double	m_XUp;
	double	m_XUs;
	double	m_YMax;
	double	m_YMin;
	double	m_YUp;
	double	m_YUs;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainGraph)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	COLORREF clrBack;
	COLORREF clrDroite;
	COLORREF clrGrads;
	CWnd* parent;

	// Generated message map functions
	//{{AFX_MSG(CMainGraph)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnDefaultcolors();
	afx_msg void OnYAutomatic();
	afx_msg void OnXAutomatic();
	afx_msg void OnKillfocusXup();
	afx_msg void OnKillfocusXus();
	afx_msg void OnKillfocusYup();
	afx_msg void OnKillfocusYus();
	afx_msg void OnKillfocusYmin();
	afx_msg void OnKillfocusYmax();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINGRAPH_H__13F0F0ED_762C_4A95_A45C_42A79F85006D__INCLUDED_)
