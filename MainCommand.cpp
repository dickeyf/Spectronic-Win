// MainCommand.cpp : implementation file
//

#include "stdafx.h"
#include "CError.h"
#include "Spectronic.h"
#include "MainCommand.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainCommand dialog


CMainCommand::CMainCommand(CWnd* pParent /*=NULL*/)
	: CDialog(CMainCommand::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainCommand)
	m_Command = _T("");
	//}}AFX_DATA_INIT
}


void CMainCommand::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainCommand)
	DDX_Control(pDX, IDC_DELETE, m_Delete);
	DDX_Control(pDX, IDC_SENDCOMMAND, m_Send);
	DDX_Control(pDX, IDC_LIST, m_List);
	DDX_Control(pDX, IDC_ADD, m_Add);
	DDX_Text(pDX, IDC_COMMAND, m_Command);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMainCommand, CDialog)
	//{{AFX_MSG_MAP(CMainCommand)
	ON_BN_CLICKED(IDC_SENDCOMMAND, OnSendcommand)
	ON_EN_CHANGE(IDC_COMMAND, OnChangeCommand)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	ON_NOTIFY(NM_CLICK, IDC_LIST, OnClickList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainCommand message handlers

BOOL CMainCommand::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CRect m_Rect;
	m_List.GetClientRect(m_Rect);
	int width = m_Rect.Width() - m_Rect.Width()/6 + 20;

	ListView_SetExtendedListViewStyleEx (	m_List.GetSafeHwnd(), 
											LVS_EX_FULLROWSELECT, 
											LVS_EX_FULLROWSELECT );

	m_List.InsertColumn(0, "", LVCFMT_CENTER, (m_Rect.Width()/6)-30);
	m_List.InsertColumn(1, "Commandes", LVCFMT_LEFT, width);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CMainCommand::OnOK()
{
	
}

void CMainCommand::OnSendcommand() 
{
	CSpectro* Spectronic = CSpectro::GetInstance();

	if(Spectronic->IsReady())
	{
		UpdateData(TRUE);

		int i;
		int count = m_List.GetItemCount();
		CString temp;

		for(i=0; i<count; i++)
		{
			temp = m_List.GetItemText(i, 1);
			Spectronic->SendCommand(temp.GetBuffer(0));
		}

		m_Command.Empty();

		MessageBox("La liste de commande a été envoyée avec succès. \r\n Si vous attendez une réponse de la machine, allez voir les logs.");

		UpdateData(FALSE);

	}
	else
		ThrowError("Veuillez configurer le ComPort...");
}

void CMainCommand::OnChangeCommand() 
{
	UpdateData(TRUE);
	if(m_Command != "")
		m_Add.EnableWindow(TRUE);
	else
		m_Add.EnableWindow(FALSE);
	UpdateData(FALSE);
}

void CMainCommand::OnAdd()
{
	int count = m_List.GetItemCount();
	CString temp;

	temp.Format("%d", count);
	m_List.InsertItem(count, temp, NULL);
	m_List.SetItem(count, 1, LVIF_TEXT , m_Command, NULL, NULL, NULL, NULL);

	m_Command.Empty();
	m_Add.EnableWindow(FALSE);
	m_List.EnableWindow(TRUE);
	m_Delete.EnableWindow(TRUE);
	m_Send.EnableWindow(TRUE);

	UpdateData(FALSE);
}

void CMainCommand::OnDelete() 
{
	if(m_List.DeleteAllItems() != TRUE)
		ThrowError("Problème avec l'effacement de la liste");
	m_Send.EnableWindow(FALSE);
	m_Delete.EnableWindow(FALSE);
}

void CMainCommand::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);

	if(m_List.GetItemCount() != 0 && m_List.GetSelectionMark() != -1)
	{
		m_Delete.EnableWindow(TRUE);
		m_Send.EnableWindow(TRUE);
	}

	UpdateData(FALSE);

	*pResult = 0;
}
