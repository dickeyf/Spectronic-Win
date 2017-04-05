// ConfigComPort.cpp : implementation file
//

#include "stdafx.h"
#include "Spectronic.h"
#include "ConfigComPort.h"
#include "Conf.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfigComPort dialog


CConfigComPort::CConfigComPort(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigComPort::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfigComPort)
	m_BaudRate = _T("");
	m_ByteSize = _T("");
	m_Nb = _T("");
	m_Parity = _T("");
	m_StopBits = _T("");
	//}}AFX_DATA_INIT
}


void CConfigComPort::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigComPort)
	DDX_CBString(pDX, IDC_BAUDRATE, m_BaudRate);
	DDX_Text(pDX, IDC_BYTESIZE, m_ByteSize);
	DDX_Text(pDX, IDC_NB, m_Nb);
	DDX_CBString(pDX, IDC_PARITY, m_Parity);
	DDX_CBString(pDX, IDC_STOPBITS, m_StopBits);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigComPort, CDialog)
	//{{AFX_MSG_MAP(CConfigComPort)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigComPort message handlers

BOOL CConfigComPort::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	ComConfig config;
	unsigned long byteread;

	HANDLE file = CreateFile("configuration.dat",
		                     GENERIC_READ,
							 NULL,
                             NULL,
							 OPEN_EXISTING,
							 FILE_ATTRIBUTE_NORMAL,
                             NULL);

	if(file == INVALID_HANDLE_VALUE)
	{
		m_Nb = "2";
		m_BaudRate = "4800";
		m_ByteSize = "8";
		m_Parity = "No Parity";
		m_StopBits = "1";
		UpdateData(FALSE);
	}
	else
	{
		ReadFile(file, &config, sizeof(ComConfig), &byteread, NULL);
		m_Nb.Format("%d", config.ComPortNb);
		switch(config.BaudRate)
		{
		case CBR_300:
			m_BaudRate = "300";
			break;
		case CBR_1200:
			m_BaudRate = "1200";
			break;
		case CBR_2400:
			m_BaudRate = "2400";
			break;
		case CBR_4800:
			m_BaudRate = "4800";
			break;
		case CBR_9600:
			m_BaudRate = "9600";
			break;
		case CBR_14400:
			m_BaudRate = "14400";
			break;
		}

		m_ByteSize.Format("%d", config.ByteSize);
		
		switch(config.Parity)
		{
		case EVENPARITY:
			m_Parity = "Even";
			break;
		case MARKPARITY:
			m_Parity = "Mark";
			break;
		case NOPARITY:
			m_Parity = "No Parity";
			break;
		case ODDPARITY:
			m_Parity = "Odd";
			break;
		case SPACEPARITY:
			m_Parity = "Space";
			break;
		}

		switch(config.StopBits)
		{
		case ONESTOPBIT:
			m_StopBits = "1";
			break;
		case ONE5STOPBITS:
			m_StopBits = "1.5";
			break;
		case TWOSTOPBITS:
			m_StopBits = "2";
			break;
		}

		UpdateData(FALSE);
	}

	CloseHandle(file);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
