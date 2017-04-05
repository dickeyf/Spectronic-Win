// MainInfo.cpp : implementation file
//

#include "stdafx.h"
#include "Spectronic.h"
#include "MainInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainInfo dialog


CMainInfo::CMainInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CMainInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainInfo)
	m_Date = _T("");
	m_Groupe = _T("");
	m_Name = _T("");
	m_Substance = _T("");
	m_Title = _T("");
	//}}AFX_DATA_INIT
}


void CMainInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainInfo)
	DDX_Text(pDX, IDC_DATE, m_Date);
	DDX_Text(pDX, IDC_GROUP, m_Groupe);
	DDX_Text(pDX, IDC_STUDENT, m_Name);
	DDX_Text(pDX, IDC_SUBST, m_Substance);
	DDX_Text(pDX, IDC_TITLE, m_Title);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMainInfo, CDialog)
	//{{AFX_MSG_MAP(CMainInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainInfo message handlers

void CMainInfo::OnOK()
{
	
}

std::string CMainInfo::GetName()
{
	std::string Name;
	Name=m_Title;
	return Name;
}

BOOL CMainInfo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CTime time = CTime::GetCurrentTime();

	m_Date = time.Format("%d/%m/%Y");

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
