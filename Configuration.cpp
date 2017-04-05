// Configuration.cpp : implementation file
//

#include "stdafx.h"
#include "Spectronic.h"
#include "Configuration.h"
#include "Conf.h"
#include "CError.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfiguration dialog


CConfiguration::CConfiguration(CWnd* pParent /*=NULL*/)
	: CDialog(CConfiguration::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfiguration)
	//}}AFX_DATA_INIT
}


void CConfiguration::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfiguration)
	DDX_Control(pDX, IDC_WORKSPACE, m_WorkSpace);
	DDX_Control(pDX, IDC_TREE_OPTIONS, m_Tree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfiguration, CDialog)
	//{{AFX_MSG_MAP(CConfiguration)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_OPTIONS, OnSelchangedTreeOptions)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfiguration message handlers

BOOL CConfiguration::OnInitDialog() 
{
	CDialog::OnInitDialog();

	HTREEITEM Hardware	= m_Tree.InsertItem("Matériel");
		HTREEITEM ComPort	= m_Tree.InsertItem("Port de communication", Hardware);
	
	m_Tree.Expand(Hardware, TVE_EXPAND);
	m_Tree.Expand(ComPort, TVE_EXPAND);

	m_Tree.SelectItem(ComPort);

	ComPortDlg   = new CConfigComPort;

	CRect m_Rect;
	m_WorkSpace.GetWindowRect(m_Rect);
	ScreenToClient(m_Rect);
	m_Rect.DeflateRect(10,4);
	m_Rect.top = m_Rect.top + 12;

	ComPortDlg->Create(IDD_CONFIG_COMPORT, this);
	ComPortDlg->MoveWindow(m_Rect.left, m_Rect.top, m_Rect.Width(), m_Rect.Height());
	ComPortDlg->ShowWindow(SW_SHOW);
	ComPortDlg->SetFocus();

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfiguration::OnSelchangedTreeOptions(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	int i=0;
	HTREEITEM tempnode = m_Tree.GetSelectedItem();
	CString tmptext[5];

	do
	{
		tmptext[i] = m_Tree.GetItemText(tempnode);
		tempnode = m_Tree.GetParentItem(tempnode);
		i++;
	}while(tempnode!=NULL);

	CString finaltext;
	for(int j=(i-1); j>=0; j--)
	{
		finaltext += tmptext[j];
		if(j)
			finaltext += ": ";
	}

	m_WorkSpace.SetWindowText(finaltext);

	*pResult = 0;
}

void CConfiguration::OnOK() 
{
	ComPortDlg->UpdateData(TRUE);

	ComConfig config;
	CString temp;

	config.ComPortNb = atoi(ComPortDlg->m_Nb.GetBuffer(0));

	temp = ComPortDlg->m_BaudRate;
	if(temp == "300")
		config.BaudRate = CBR_300;
	else if(temp == "1200")
		config.BaudRate = CBR_600;
	else if(temp == "2400")
		config.BaudRate = CBR_2400;
	else if(temp == "4800")
		config.BaudRate = CBR_4800;
	else if(temp == "9600")
		config.BaudRate = CBR_9600;
	else if(temp == "14400")
		config.BaudRate = CBR_14400;

	config.ByteSize = atoi(ComPortDlg->m_ByteSize.GetBuffer(0));
	
	temp = ComPortDlg->m_Parity;
	if(temp == "Even")
		config.Parity = EVENPARITY;
	else if(temp == "Mark")
		config.Parity = MARKPARITY;
	else if(temp == "No Parity")
		config.Parity = NOPARITY;
	else if(temp == "Odd")
		config.Parity = ODDPARITY;
	else if(temp == "Space")
		config.Parity = SPACEPARITY;

	temp = ComPortDlg->m_StopBits.GetBuffer(0);
	if(temp == "1")
		config.StopBits = ONESTOPBIT;
	else if(temp == "1.5")
		config.StopBits = ONE5STOPBITS;
	else if(temp == "2")
		config.StopBits = TWOSTOPBITS;

	HANDLE file = CreateFile("configuration.dat",
		                     GENERIC_WRITE,
							 NULL,
                             NULL,
							 CREATE_ALWAYS,
							 FILE_ATTRIBUTE_NORMAL,
                             NULL);

	if(file == INVALID_HANDLE_VALUE)
	{
		ThrowError();
		CDialog::OnOK();
		return;
	}

	unsigned long bytewritten;
	WriteFile(file, &config, sizeof(config), &bytewritten, NULL);

	CloseHandle(file);
	
	CDialog::OnOK();
}
