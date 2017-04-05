// MaxDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Spectronic.h"
#include "MaxDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMaxDlg dialog


CMaxDlg::CMaxDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMaxDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMaxDlg)
	m_Xy = _T("");
	m_Begin = 0.0;
	m_End = 0.0;
	m_Max = _T("");
	//}}AFX_DATA_INIT
}


void CMaxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMaxDlg)
	DDX_Control(pDX, IDC_LIST, m_List);
	DDX_Text(pDX, IDC_XY, m_Xy);
	DDX_Text(pDX, IDC_BEGIN, m_Begin);
	DDX_Text(pDX, IDC_END, m_End);
	DDX_Text(pDX, IDC_MAXIMUM, m_Max);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMaxDlg, CDialog)
	//{{AFX_MSG_MAP(CMaxDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMaxDlg message handlers

BOOL CMaxDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CRect m_Rect;
	m_List.GetWindowRect(m_Rect);
	ScreenToClient(m_Rect);
	int width = m_Rect.Width() / 2;

	ListView_SetExtendedListViewStyleEx (	m_List.GetSafeHwnd(), 
											LVS_EX_FULLROWSELECT, 
											LVS_EX_FULLROWSELECT );

	m_List.InsertColumn(0, "X", LVCFMT_LEFT, width);
	m_List.InsertColumn(1, "Y", LVCFMT_CENTER, m_Rect.Width()-width-20);

	m_Xy = "0,0";

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CMaxDlg::PreTranslateMessage(MSG* pMsg) 
{
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_DELETE))
	{
		m_List.DeleteItem(m_List.GetSelectionMark());
		m_List.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	}

	UpdateData(FALSE);
	
	return CDialog::PreTranslateMessage(pMsg);
}