// MainScan.cpp : implementation file
//

#include "stdafx.h"
#include "Spectronic.h"
#include "MainScan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainScan dialog


CMainScan::CMainScan(CWnd* pParent /*=NULL*/)
	: CDialog(CMainScan::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainScan)
	m_WvInc = 0.2;
	m_Speed = 200.0;
	m_StopWv = 400.0;
	m_StrtWv = 190.0;
	m_Time = _T("0:0:0");
	//}}AFX_DATA_INIT
}


void CMainScan::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainScan)
	DDX_Text(pDX, IDC_INC, m_WvInc);
	DDV_MinMaxDouble(pDX, m_WvInc, 0.1, 999.9);
	DDX_Text(pDX, IDC_SPEED, m_Speed);
	DDV_MinMaxInt(pDX, m_Speed, 5, 3000);
	DDX_Text(pDX, IDC_STOPWV, m_StopWv);
	DDV_MinMaxDouble(pDX, m_StopWv, 190., 950.);
	DDX_Text(pDX, IDC_STRTWV, m_StrtWv);
	DDV_MinMaxDouble(pDX, m_StrtWv, 190., 950.);
	DDX_Text(pDX, IDC_TIME, m_Time);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMainScan, CDialog)
	//{{AFX_MSG_MAP(CMainScan)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainScan message handlers

void CMainScan::OnOK()
{
	
}
