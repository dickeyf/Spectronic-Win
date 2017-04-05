// MainDefaultDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Spectronic.h"
#include "MainDefaultDlg.h"
#include "ListLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainDefaultDlg dialog


CMainDefaultDlg::CMainDefaultDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDefaultDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainDefaultDlg)
	//}}AFX_DATA_INIT
}


void CMainDefaultDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainDefaultDlg)
	DDX_Control(pDX, IDC_LISTLOG, m_Log);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMainDefaultDlg, CDialog)
	//{{AFX_MSG_MAP(CMainDefaultDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CMainDefaultDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect m_Rect;
	m_Log.GetWindowRect(m_Rect);
	int width = m_Rect.Width();
	width = (width/10);
	m_Log.InsertColumn(0, "Temps", LVCFMT_CENTER, width);
	m_Log.InsertColumn(1, "Messages", LVCFMT_LEFT, m_Rect.Width()-width-20);

	ListView_SetExtendedListViewStyleEx (	m_Log.GetSafeHwnd(), 
											LVS_EX_FULLROWSELECT, 
											LVS_EX_FULLROWSELECT );

	return TRUE;
}

BOOL CMainDefaultDlg::PreTranslateMessage(MSG* pMsg)
{
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE))
	{
		CWnd* parent = GetParent();
		parent->PreTranslateMessage(pMsg);

		return TRUE;
	}
	
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
	{
		CWnd* parent = GetParent();
		parent->PreTranslateMessage(pMsg);

		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CMainDefaultDlg::OnOK()
{
	
}
