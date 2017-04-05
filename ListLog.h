#if !defined(AFX_LISTLOG_H__0BFC3162_0C98_4A99_A21A_A70F907D9577__INCLUDED_)
#define AFX_LISTLOG_H__0BFC3162_0C98_4A99_A21A_A70F907D9577__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListLog.h : header file

/////////////////////////////////////////////////////////////////////////////
// CListLog window

class CListLog : public CListCtrl
{
// Construction
public:
	CListLog();

	int InsertToLog(int nIndex, LPCTSTR lpszItem);
	int AddToLog(LPCTSTR lpszItem);
	int AddError(LPCTSTR lpszItem);
	int AddNews(LPCTSTR lpszItem);
	BOOL DeleteItem( UINT nIndex );
	BOOL DeleteAllItems();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListLog)
	//}}AFX_VIRTUAL

	virtual ~CListLog();

	// Generated message map functions
protected:
	enum {LOG_ERROR, LOG_VERT, LOG_NORMAL};
	vector<int> Message;
	int count;

	//{{AFX_MSG(CListLog)
		afx_msg void OnCustomdrawLog ( NMHDR* pNMHDR, LRESULT* pResult );
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTLOG_H__0BFC3162_0C98_4A99_A21A_A70F907D9577__INCLUDED_)
