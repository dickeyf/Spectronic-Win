#if !defined(AFX_FULLGRAPHDLG_H__93A2CBFA_9A68_454D_B24F_7C57B4085752__INCLUDED_)
#define AFX_FULLGRAPHDLG_H__93A2CBFA_9A68_454D_B24F_7C57B4085752__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FullGraphDlg.h : header file
//

#include "ListData.h"
#include "MaxDlg.h"
#include "Conf.h"
#include "MainGraph.h"

/////////////////////////////////////////////////////////////////////////////
// CFullGraphDlg dialog

class CFullGraphDlg : public CDialog
{
// Construction
public:
	CFullGraphDlg(CWnd* pParent = NULL,CListData* Data=NULL,CMainGraph* setting=NULL);   // standard constructor

	CMaxDlg*		MaxDlg;

// Dialog Data
	//{{AFX_DATA(CFullGraphDlg)
	enum { IDD = IDD_FULLGRAPHDLG };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFullGraphDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	CListData*		DataList;
	CMainGraph*		Settings;
	double BeginX;
	double EndX;
	CRect m_Graph;				// Rectangle du graphique
	CRect m_Cursor;
	CRect m_LastCur;
	CRect m_IntervalLimit1;
	CRect m_IntervalLimit2;
	CRect m_MaxCur;
	bool CursorPainting;		// true-> le curseur a besoin d'un paint
	bool LimitCur1;
	bool LimitCur2;
	bool MaxCur;
	bool DragDrop;

	// Generated message map functions
	//{{AFX_MSG(CFullGraphDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FULLGRAPHDLG_H__93A2CBFA_9A68_454D_B24F_7C57B4085752__INCLUDED_)
