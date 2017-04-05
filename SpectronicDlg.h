// SpectronicDlg.h : header file
//

#if !defined(AFX_SPECTRONICDLG_H__485DCB6E_F0AA_11D4_A1ED_0090000873F3__INCLUDED_)
#define AFX_SPECTRONICDLG_H__485DCB6E_F0AA_11D4_A1ED_0090000873F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ListData.h"
#include "MainDefaultDlg.h"
#include "MainCommand.h"
#include "MainBlank.h"
#include "MainScan.h"
#include "MainInfo.h"
#include "FullGraphDlg.h"
#include "MaxDlg.h"
#include "MainGraph.h"

#include <list>

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CSpectronicDlg dialog

#define WM_TIMEUP 256

class CSpectronicDlg : public CDialog
{
// Construction
public:
	CSpectronicDlg(CWnd* pParent = NULL);	// standard constructor
	~CSpectronicDlg();

// Dialog Data
	//{{AFX_DATA(CSpectronicDlg)
	enum { IDD = IDD_SPECTRONIC_DIALOG };
	CButton	m_Init;
	CButton	m_Cancel;
	CButton	m_Uv;
	CStatic	m_Graph;
	CButton	m_WorkSpace;
	CListData	m_Value;
	CTreeCtrl	m_Tree;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpectronicDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
//Windows Stuff
	HICON m_hIcon;

// Spectronic 1201 stuff
	bool SetupDone;

	double XValue;
	double YValue;



	list<double> blank;
	list<double>::iterator Position;

	double LastXValue;
	bool GoodString;
	string InputBuffer;
	string InputBuffer2;
	CSpectro* Spectronic;
	CWnd* WinHandler;

// Les dialogues internes
	CMainDefaultDlg*	DefaultDlg;
	CMainCommand*		CommandDlg;
	CMainBlank*			BlankDlg;
	CMainScan*			ScanDlg;
	CMainInfo*			InfoDlg;
	CMainGraph*			GraphDlg;

// Édition du graphique
	CFullGraphDlg*		BigGraphDlg;

	// Generated message map functions
	//{{AFX_MSG(CSpectronicDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg LRESULT OnInputChar(WPARAM,LPARAM);
	afx_msg void OnEditionOptions();
	virtual void OnCancel();
	afx_msg void OnInitialyse();
	afx_msg void OnSelchangedMenu(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnQuitter();
	afx_msg void OnUvscan();
	afx_msg void OnCanceler();
	afx_msg void OnAidePropos();
	afx_msg void OnFichierSaveEtalon();
	afx_msg void OnFichierSavedataBin();
	afx_msg void OnFichierOpendataBin();
	afx_msg void OnFichierOpenetalon();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnFichierOpenTxt();
	afx_msg void OnFichierSaveTxt();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnFichierPrint();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPECTRONICDLG_H__485DCB6E_F0AA_11D4_A1ED_0090000873F3__INCLUDED_)
