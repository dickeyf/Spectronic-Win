// SpectronicDlg.cpp : implementation file
//

#include "stdafx.h"
#include "math.h"
#include "CError.h"
#include "ListData.h"
#include "Spectronic.h"
#include "SpectronicDlg.h"
#include "Configuration.h"
#include "MainDefaultDlg.h"
#include "Conf.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpectronicDlg dialog

CSpectronicDlg::CSpectronicDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSpectronicDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSpectronicDlg)
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	Spectronic = CSpectro::GetInstance();
	SetupDone = false;
}

CSpectronicDlg::~CSpectronicDlg()
{

}

void CSpectronicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpectronicDlg)
	DDX_Control(pDX, IDC_INITIALYSE, m_Init);
	DDX_Control(pDX, IDC_CANCELER, m_Cancel);
	DDX_Control(pDX, IDC_UVSCAN, m_Uv);
	DDX_Control(pDX, IDC_GRAPH, m_Graph);
	DDX_Control(pDX, IDC_WORKSPACE, m_WorkSpace);
	DDX_Control(pDX, IDC_VALUE, m_Value);
	DDX_Control(pDX, IDC_MENU, m_Tree);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSpectronicDlg, CDialog)
	//{{AFX_MSG_MAP(CSpectronicDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(SP_CHAR,OnInputChar)
	ON_COMMAND(ID_EDITION_OPTIONS, OnEditionOptions)
	ON_BN_CLICKED(IDC_INITIALYSE, OnInitialyse)
	ON_NOTIFY(TVN_SELCHANGED, IDC_MENU, OnSelchangedMenu)
	ON_COMMAND(ID_FICHIER_QUITTER, OnQuitter)
	ON_BN_CLICKED(IDC_UVSCAN, OnUvscan)
	ON_BN_CLICKED(IDC_CANCELER, OnCanceler)
	ON_COMMAND(ID_AIDE_PROPOS, OnAidePropos)
	ON_COMMAND(ID_FICHIER_SAVEETALON, OnFichierSaveEtalon)
	ON_COMMAND(ID_FICHIER_SAVEDATA_BIN, OnFichierSavedataBin)
	ON_COMMAND(ID_FICHIER_OPENDATA_BIN, OnFichierOpendataBin)
	ON_COMMAND(ID_FICHIER_OPENETALON, OnFichierOpenetalon)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_FICHIER_OPEN_TXT, OnFichierOpenTxt)
	ON_COMMAND(ID_FICHIER_SAVE_TXT, OnFichierSaveTxt)
	ON_WM_TIMER()
	ON_COMMAND(ID_FICHIER_PRINT, OnFichierPrint)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpectronicDlg message handlers

BOOL CSpectronicDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


// Tree
	HTREEITEM Spectro	= m_Tree.InsertItem("Spectronic 1001 Plus");
		HTREEITEM Manuel	= m_Tree.InsertItem("Commandes Manuelles", Spectro);
		HTREEITEM Precision	= m_Tree.InsertItem("Configuration du Spectre",Spectro);
		HTREEITEM Info		= m_Tree.InsertItem("Information sur le Scan", Spectro);
		HTREEITEM Etalon	= m_Tree.InsertItem("Fichier de l'étalon", Spectro);


	HTREEITEM Graph	= m_Tree.InsertItem("Graphique");

	// Les windows utilisés dans le menu principal du programme
	DefaultDlg	= new CMainDefaultDlg(this);
	CommandDlg	= new CMainCommand(this);
	BlankDlg	= new CMainBlank(this);
	ScanDlg		= new CMainScan(this);
	InfoDlg		= new CMainInfo(this);
	GraphDlg	= new CMainGraph(this);

	BigGraphDlg	= new CFullGraphDlg(this,&m_Value, GraphDlg);

	m_Value.GiveMaxDlgInstance(BigGraphDlg->MaxDlg);

	CRect m_Rect;
	m_WorkSpace.GetWindowRect(m_Rect);
	ScreenToClient(m_Rect);
	m_Rect.DeflateRect(4,4);
	m_Rect.top = m_Rect.top + 12;

	// Crée chacun des windows et les places à la bonne position

	DefaultDlg->Create(IDD_MAIN_DEFAULT, this);
	DefaultDlg->MoveWindow(m_Rect.left, m_Rect.top, m_Rect.Width(), m_Rect.Height());
	DefaultDlg->ShowWindow(SW_SHOW);
	DefaultDlg->SetFocus();

	CommandDlg->Create(IDD_MAIN_COMMANDS, this);
	CommandDlg->MoveWindow(m_Rect.left, m_Rect.top, m_Rect.Width(), m_Rect.Height());

	BlankDlg->Create(IDD_MAIN_BLANK, this);
	BlankDlg->MoveWindow(m_Rect.left, m_Rect.top, m_Rect.Width(), m_Rect.Height());

	ScanDlg->Create(IDD_MAIN_SCAN, this);
	ScanDlg->MoveWindow(m_Rect.left, m_Rect.top, m_Rect.Width(), m_Rect.Height());

	InfoDlg->Create(IDD_MAIN_INFO, this);
	InfoDlg->MoveWindow(m_Rect.left, m_Rect.top, m_Rect.Width(), m_Rect.Height());

	GraphDlg->Create(IDD_MAIN_GRAPH, this);
	GraphDlg->MoveWindow(m_Rect.left, m_Rect.top, m_Rect.Width(), m_Rect.Height());

	BigGraphDlg->Create(IDD_FULLGRAPHDLG,this);

	m_Value.SetExtendedStyle(LVS_EX_GRIDLINES);
	m_Value.EnableScrollBar(SB_HORZ, ESB_DISABLE_BOTH);

	m_Value.GetWindowRect(m_Rect);
	ScreenToClient(m_Rect);
	int width = ((m_Rect.Width()-30) / 2)-10;

	// Insère les colones dans la liste
	m_Value.InsertColumn(0, "",  LVCFMT_CENTER, 30);
	m_Value.InsertColumn(1, "X", LVCFMT_CENTER, width);
	m_Value.InsertColumn(2, "Y", LVCFMT_CENTER, width);
	
	// Permet de sélectionner une ligne complète à la place d'un seul élément
	ListView_SetExtendedListViewStyleEx (	m_Value.GetSafeHwnd(), 
											LVS_EX_FULLROWSELECT, 
											LVS_EX_FULLROWSELECT );

	GraphDlg->m_XUp = 2;
	GraphDlg->m_XUs = 20;

	// Ajoute quelques messages de bienvenue
	DefaultDlg->m_Log.AddToLog("Spectronic 1001 Plus");
	DefaultDlg->m_Log.AddToLog("¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯");
	DefaultDlg->m_Log.AddToLog("Veuillez patienter pendant que la machine s'initialise...");
	DefaultDlg->m_Log.AddToLog("Assurez vous que la machine est bien branché à l'ordinateur.");
	DefaultDlg->m_Log.AddToLog("Tous les messages de ce qui arrive dans le programme seront affichés ici.");
	DefaultDlg->m_Log.AddToLog("Veuillez insérer l'échantillon.");
	DefaultDlg->m_Log.AddToLog("");

	return TRUE;
}

void CSpectronicDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CSpectronicDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC DestDC(this);
		
		CRect m_Rect;

		m_Graph.GetWindowRect(m_Rect);
		ScreenToClient(m_Rect);

		GraphDlg->UpdateData(FALSE);

		m_Value.DrawGraph(&m_Rect,GraphDlg->m_XUp,GraphDlg->m_XUs,GraphDlg->m_YUp,GraphDlg->m_YUs,&DestDC,FALSE);

		CDialog::OnPaint();
		//PAINTSTRUCT ps;
		
	}
}

HCURSOR CSpectronicDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSpectronicDlg::OnOK()
{
	
}

LRESULT CSpectronicDlg::OnInputChar(WPARAM wParam,LPARAM lParam)
{
	if ((wParam==242256242)&&((char)lParam != ' '))
	{
		if ((char)lParam==',')
		{
			XValue=atof(InputBuffer.c_str());
			InputBuffer2=InputBuffer;
			InputBuffer="";
			return true;
		}
		//Si c'est la fin du string
		if (lParam==13)
		{
			if (InputBuffer=="----------------")
					{
						Spectronic->ScanDone();
						DefaultDlg->m_Log.AddNews("Scan test terminé.");
						InputBuffer="";
						KillTimer(WM_TIMEUP);
						return true;
					}
			//Verifier que le string a été bon
			if (GoodString)
			{
				//Si c'est pas un CR prematuré
				if ((XValue!=0)&&(XValue!=LastXValue)&&(XValue*.1>=190))
				{   //Ajouter le couple x,y à la liste
					InputBuffer2+=",";
					InputBuffer2+=InputBuffer;
					InputBuffer2+="\r\n";
					CSerialPort::WriteLogFile(InputBuffer2);
					XValue *= 0.1;
					if(BlankDlg->m_Sel == TRUE)
					{
						YValue = .000147*atof(InputBuffer.c_str())-*Position;
						Position++;
					}
					else
					{
						YValue = .000147*atof(InputBuffer.c_str());
					}
                    m_Value.AddData(XValue, YValue);
					UpdateData(FALSE);

					InputBuffer="";
					LastXValue=XValue;
					XValue=0;
					return true;
				}
			}
			else 
			{   //Sinon, reset GoodString
				//Pis mettons le dans la screen
				//de logging pour qu'on puisse lire
				//les machins interressants que la
				//machine nous send
				DefaultDlg->m_Log.AddToLog(InputBuffer.c_str());
				GoodString=true;
				InputBuffer="";
				XValue=0;
				return true;
			}
		}
		if (((lParam>47)&&(lParam<58))				
			||(lParam==46)||(lParam==45))
		{
			InputBuffer+=(char)lParam;
		}
		else
		{
			InputBuffer+=(char)lParam;
			GoodString=false;
		}
	}
	return true;
}

void CSpectronicDlg::OnEditionOptions() 
{
	CConfiguration dlg;
	int result = dlg.DoModal();
	if(result == IDOK)
	{
		
	}
	else if(result == IDCANCEL)
	{
		
	}
}

void CSpectronicDlg::OnCancel() 
{	
	CDialog::OnCancel();
}

void CSpectronicDlg::OnInitialyse() 
{
	UpdateData(TRUE);

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
		if (Spectronic->Init(m_hWnd))
		{
			SetupDone=true;
			CString temp;
			temp.Format("ComPort#:%d, BaudRate:%d, ByteSize:%d, Parity:None, StopBits:%d", 2, 4800, 8, 1);
			DefaultDlg->m_Log.AddNews(temp);
			DefaultDlg->m_Log.AddNews("Initialisation(défault) du port de communication réussie.");
		}
		else
		{
			SetupDone=false;
			ThrowError("Initialisation du port de communication échouée.");
			DefaultDlg->m_Log.AddError("Initialisation(défault) du port de communication échouée.");
			CloseHandle(file);
			return;
		}
	}
	else
	{
		if(ReadFile(file, &config, sizeof(ComConfig), &byteread, NULL))
		{
			if (Spectronic->Init(m_hWnd, config.ComPortNb, config.BaudRate, config.ByteSize, config.Parity, config.StopBits))
			{
				SetupDone=true;
				CString temp;
				temp.Format("ComPort#:%d, BaudRate:%d, ByteSize:%d, Parity:%d, StopBits:%d", config.ComPortNb, config.BaudRate, config.ByteSize, config.Parity, config.StopBits);
				DefaultDlg->m_Log.AddNews(temp);
				DefaultDlg->m_Log.AddNews("Initialisation du port de communication réussie.");
			}
			else
			{
				SetupDone=false;
				ThrowError("Initialisation du port de communication échouée.\r\nVeuillez vérifier votre configuration.");
				DefaultDlg->m_Log.AddError("Initialisation du port de communication échouée.");
				CloseHandle(file);
				return;
			}
		}
		else
		{
			ThrowError(	"Il y a un problème avec le fichier de configuration.\r\nAller dans Edition->Options pour reconfigurer votre port de communication.\r\nConfiguration par défaut utilisée.");
			if (Spectronic->Init(m_hWnd))
			{
				SetupDone=true;
				CString temp;
				temp.Format("ComPort#:%d, BaudRate:%d, ByteSize:%d, Parity:None, StopBits:%d", 2, 4800, 8, 1);
				DefaultDlg->m_Log.AddNews(temp);
				DefaultDlg->m_Log.AddNews("Initialisation(défault) du port de communication réussie.");
			}
			else
			{
				SetupDone=false;
				ThrowError("Initialisation du port de communication échouée.");
				DefaultDlg->m_Log.AddError("Initialisation(défault) du port de communication échouée.");
				CloseHandle(file);
				return;
			}
		}	// End if ReadFile
	}	// End if INVALID_HANDLE

	CloseHandle(file);

	m_Uv.EnableWindow(TRUE);
	m_Cancel.EnableWindow(TRUE);
	m_Init.EnableWindow(FALSE);

	UpdateData(FALSE);
}

void CSpectronicDlg::OnSelchangedMenu(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

// Change le texte dans la workspace
	int i=0;
	HTREEITEM selected = m_Tree.GetSelectedItem();
	HTREEITEM tempnode = selected;
	string tmptext[5];

	do
	{
		tmptext[i] = m_Tree.GetItemText(tempnode);
		tempnode = m_Tree.GetParentItem(tempnode);
		i++;
	}while(tempnode!=NULL);

	string finaltext;
	for(int j=(i-1); j>=0; j--)
	{
		finaltext += tmptext[j];
		if(j)
			finaltext += ": ";
	}

	m_WorkSpace.SetWindowText(finaltext.c_str());

// Change la fenêtre qui est présentement affichée.

	CString text = m_Tree.GetItemText(selected);
	
	if(text == "Commandes Manuelles")
	{
		GraphDlg->ShowWindow(SW_HIDE);
		DefaultDlg->ShowWindow(SW_HIDE);
		BlankDlg->ShowWindow(SW_HIDE);
		ScanDlg->ShowWindow(SW_HIDE);
		InfoDlg->ShowWindow(SW_HIDE);
		CommandDlg->ShowWindow(SW_SHOW);
	}
	else if(text == "Spectronic 1001 Plus")
	{
		GraphDlg->ShowWindow(SW_HIDE);
		CommandDlg->ShowWindow(SW_HIDE);
		BlankDlg->ShowWindow(SW_HIDE);
		ScanDlg->ShowWindow(SW_HIDE);
		InfoDlg->ShowWindow(SW_HIDE);
		DefaultDlg->ShowWindow(SW_SHOW);
	}
	else if(text == "Fichier de l'étalon")
	{
		GraphDlg->ShowWindow(SW_HIDE);
		CommandDlg->ShowWindow(SW_HIDE);
		DefaultDlg->ShowWindow(SW_HIDE);
		ScanDlg->ShowWindow(SW_HIDE);
		InfoDlg->ShowWindow(SW_HIDE);
		BlankDlg->ShowWindow(SW_SHOW);
	}
	else if(text == "Configuration du Spectre")
	{
		GraphDlg->ShowWindow(SW_HIDE);
		CommandDlg->ShowWindow(SW_HIDE);
		DefaultDlg->ShowWindow(SW_HIDE);
		BlankDlg->ShowWindow(SW_HIDE);
		InfoDlg->ShowWindow(SW_HIDE);
		ScanDlg->ShowWindow(SW_SHOW);
	}
	else if(text == "Information sur le Scan")
	{
		GraphDlg->ShowWindow(SW_HIDE);
		CommandDlg->ShowWindow(SW_HIDE);
		DefaultDlg->ShowWindow(SW_HIDE);
		BlankDlg->ShowWindow(SW_HIDE);
		ScanDlg->ShowWindow(SW_HIDE);
		InfoDlg->ShowWindow(SW_SHOW);
	}
	else if(text == "Graphique")
	{
		CommandDlg->ShowWindow(SW_HIDE);
		DefaultDlg->ShowWindow(SW_HIDE);
		BlankDlg->ShowWindow(SW_HIDE);
		ScanDlg->ShowWindow(SW_HIDE);
		InfoDlg->ShowWindow(SW_HIDE);
		GraphDlg->ShowWindow(SW_SHOW);
	}

	*pResult = 0;
}

void CSpectronicDlg::OnQuitter() 
{
	CDialog::OnOK();	
}

BOOL CSpectronicDlg::PreTranslateMessage(MSG* pMsg) 
{
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE))
		CDialog::OnOK();
	
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
	{
		if(Spectronic->IsReady())
		{
			CommandDlg->UpdateData(TRUE);
			string temp;
			temp=CommandDlg->m_Command;
			Spectronic->SendCommand(temp);
			CommandDlg->m_Command.Empty();
			CommandDlg->UpdateData(FALSE);
		}
		else
			ThrowError("Veuillez configurer le port de communication.");


		return FALSE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CSpectronicDlg::OnUvscan() 
{
	if (SetupDone)
	{
		blank.clear();
		m_Value.DeleteAllItems();
		BlankDlg->UpdateData(TRUE);
		ScanDlg->UpdateData(TRUE);

		if(BlankDlg->m_Sel == TRUE)
		{
			CString path = BlankDlg->m_List.GetItemText(BlankDlg->m_List.GetSelectionMark(),1);
			blank = m_Value.FileToList(path);
			double size = ((ScanDlg->m_StopWv - ScanDlg->m_StrtWv) / ScanDlg->m_WvInc);
			if( (int)size != (blank.size()-1))
			{
				ThrowError("Votre fichier étalon ne convient pas pour les paramètres de scan que vous utilisés.");
				DefaultDlg->m_Log.AddError("Votre fichier étalon ne convient pas pour les paramètres de scan que vous utilisés.");
				DefaultDlg->m_Log.AddError("Scan interrompu...");
				return;
			}
			else
			{
				CString message = "Fichier étalon utilisé: ";
				message += path;
				DefaultDlg->m_Log.AddNews(message);
				Position = blank.begin();
			}
		}
		SetTimer(WM_TIMEUP, 300, NULL);
		m_Value.SetDataConf(BlankDlg->m_Sel,ScanDlg->m_WvInc);
		Spectronic->ScanInterval(
			ScanDlg->m_StrtWv,
			ScanDlg->m_StopWv,
			ScanDlg->m_WvInc,
			ScanDlg->m_Speed);

		CString param;
		param.Format("Début: %g, Fin: %g, Incrémentation: %g, Vitesse: %d",
						ScanDlg->m_StrtWv, 
						ScanDlg->m_StopWv,
						ScanDlg->m_WvInc,
						ScanDlg->m_Speed);
		DefaultDlg->m_Log.AddNews(param);
	}
	else
		ThrowError("Veuillez initialiser le port de communication.");
}

void CSpectronicDlg::OnCanceler()
{
	if(SetupDone)
		Spectronic->Reset();
	else
		ThrowError("Veuillez initialiser le port de communication.");
}

void CSpectronicDlg::OnAidePropos() 
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();	
}

void CSpectronicDlg::OnFichierSavedataBin() 
{
	CFileDialog file(FALSE, "scn", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Ficher d'un spectre (*.scn)|*.scn|", this);
	int sucess = file.DoModal();
	if(sucess == IDOK)
	{
		CString path = file.GetPathName();
		CString m_blank = "";
		int sel = BlankDlg->m_List.GetSelectionMark();

		BlankDlg->UpdateData(TRUE);

		if(sel != -1 && BlankDlg->m_Sel == TRUE)
			m_blank = BlankDlg->m_List.GetItemText(sel, 0);

		ScanDlg->UpdateData(TRUE);
		InfoDlg->UpdateData(TRUE);

		FileInfo info;
		info.Etalon = m_blank;
		info.Title = InfoDlg->m_Title;
		info.Subst = InfoDlg->m_Substance;
		info.Name = InfoDlg->m_Name;
		info.Group = InfoDlg->m_Groupe;
		info.Date = InfoDlg->m_Date;
		info.WvInc = ScanDlg->m_WvInc;
		info.Speed = ScanDlg->m_Speed;
		info.StrtWv = ScanDlg->m_StrtWv;
		info.StopWv = ScanDlg->m_StopWv;

		m_Value.SaveDataBinToDisk(path, info);

		DefaultDlg->m_Log.AddNews("Fichier de donnés sauvegardé avec succès.");
	}
}

void CSpectronicDlg::OnFichierOpendataBin() 
{
	CFileDialog file(TRUE, "scn", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Ficher d'un spectre (*.scn)|*.scn|", this);
	int sucess = file.DoModal();
	if(sucess == IDOK)
	{
		CString path = file.GetPathName();

		FileInfo info = m_Value.LoadDataBinFromDisk(path);

		if(info.WvInc != 0)
		{
			ScanDlg->m_WvInc = info.WvInc;
			ScanDlg->m_Speed = info.Speed;
			ScanDlg->m_StrtWv = info.StrtWv;
			ScanDlg->m_StopWv = info.StopWv;
			InfoDlg->m_Title = info.Title;
			InfoDlg->m_Substance = info.Subst;
			InfoDlg->m_Name = info.Name;
			InfoDlg->m_Groupe = info.Group;
			InfoDlg->m_Date = info.Date;

			ScanDlg->UpdateData(FALSE);
			InfoDlg->UpdateData(FALSE);

			DefaultDlg->m_Log.AddNews("Fichier de donnés chargé avec succès.");
		}
		else
			DefaultDlg->m_Log.AddError("Fichier de donnés chargé sans succès.");
	}
	CRect m_Rect;
	m_Graph.GetWindowRect(m_Rect);
	ScreenToClient(m_Rect);
	InvalidateRect(m_Rect, TRUE);
	SendMessage(WM_PAINT,0,0);
}

void CSpectronicDlg::OnFichierSaveEtalon() 
{
	CFileDialog file(FALSE, "blk", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Fichier Étalon (*.blk)|*.blk|", this);
	int sucess = file.DoModal();
	if(sucess == IDOK)
	{
		CString path = file.GetPathName();

		BlankDlg->UpdateData(TRUE);

		if(BlankDlg->m_List.GetSelectionMark() != -1 && BlankDlg->m_Sel == TRUE)
		{
			DefaultDlg->m_Log.AddError("Sauvegarde échouée -> Veuillez désélectionner l'étalon choisi.");
			ThrowError("Veuillez désélectionner l'étalon choisi dans la liste.");
			return;
		}

		ScanDlg->UpdateData(TRUE);
		FileInfo info;
		info.Etalon = "";
		info.WvInc = ScanDlg->m_WvInc;
		info.Speed = ScanDlg->m_Speed;
		info.StrtWv = ScanDlg->m_StrtWv;
		info.StopWv = ScanDlg->m_StopWv;
		m_Value.SaveDataBinToDisk(path, info);
		DefaultDlg->m_Log.AddNews("Fichier étalon sauvegardé avec succès.");
	}
}

void CSpectronicDlg::OnFichierOpenetalon() 
{
	CFileDialog file(TRUE, "blk", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Fichier Étalon (*.blk)|*.blk|", this);
	int sucess = file.DoModal();
	if(sucess == IDOK)
	{
		CString path = file.GetPathName();
		FileInfo info = m_Value.LoadDataBinFromDisk(path);
		if(info.WvInc != 0)
		{
			ScanDlg->m_WvInc = info.WvInc;
			ScanDlg->m_Speed = info.Speed;
			ScanDlg->m_StrtWv = info.StrtWv;
			ScanDlg->m_StopWv = info.StopWv;
			ScanDlg->UpdateData(FALSE);
			DefaultDlg->m_Log.AddNews("Fichier étalon chargé avec succès.");
		}
		else
			DefaultDlg->m_Log.AddError("Fichier étalon chargé sans succès.");
	}
	CRect m_Rect;
	m_Graph.GetWindowRect(m_Rect);
	ScreenToClient(m_Rect);
	InvalidateRect(m_Rect, TRUE);
	SendMessage(WM_PAINT,0,0);
	BigGraphDlg->ShowWindow(SW_HIDE);
}

void CSpectronicDlg::OnFichierOpenTxt() 
{
	CFileDialog file(TRUE, "txt", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Fichier Texte (*.txt)|*.txt|", this);
	int sucess = file.DoModal();
	if(sucess == IDOK)
	{
		CString path = file.GetPathName();
		if(m_Value.LoadDataFromDisk(path))
			DefaultDlg->m_Log.AddNews("Fichier texte chargé avec succès.");
		else
			DefaultDlg->m_Log.AddError("Erreur dans le chargement du fichier texte.");
	}
	CRect m_Rect;
	m_Graph.GetWindowRect(m_Rect);
	ScreenToClient(m_Rect);
	InvalidateRect(m_Rect, TRUE);
	SendMessage(WM_PAINT,0,0);
	BigGraphDlg->ShowWindow(SW_HIDE);	
}

void CSpectronicDlg::OnFichierSaveTxt() 
{
	CFileDialog file(FALSE, "txt", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Fichier Texte (*.txt)|*.txt|", this);
	int sucess = file.DoModal();
	if(sucess == IDOK)
	{
		CString path = file.GetPathName();
		if(m_Value.SaveDataToDisk(path))
			DefaultDlg->m_Log.AddNews("Fichier texte sauvegardé avec succès.");
		else
			DefaultDlg->m_Log.AddError("Erreur dans la sauvegarde du fichier texte.");
	}	
}

void CSpectronicDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CRect m_Rect, m_WndRect, m_Final;
	WINDOWPLACEMENT max;

	m_Graph.GetWindowRect(m_Rect);
	ScreenToClient(m_Rect);

	if (m_Rect.PtInRect(point))
	{
		max.length = sizeof(WINDOWPLACEMENT);
		max.showCmd = SW_SHOWMAXIMIZED;
		max.flags = WPF_RESTORETOMAXIMIZED;
		max.ptMaxPosition.x=0;
		max.ptMaxPosition.y=0;
		max.rcNormalPosition = CRect(CPoint(0,0), CPoint(640,480));

		BigGraphDlg->SetWindowPlacement(&max);

		BigGraphDlg->GetWindowRect(m_Rect);
		BigGraphDlg->MaxDlg->GetWindowRect(m_WndRect);
		m_WndRect.NormalizeRect();

		m_Final = m_Rect;
		m_Final.left = m_Rect.right-m_WndRect.Width()-5;
		m_Final.top += 25;
		m_Final.bottom = m_WndRect.Height()+21;
		m_Final.right -= 5;

		BigGraphDlg->MaxDlg->MoveWindow(m_Final);
		BigGraphDlg->MaxDlg->ShowWindow(SW_SHOW);
	}

	CDialog::OnLButtonDblClk(nFlags, point);
}

void CSpectronicDlg::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == WM_TIMEUP)
	{
		CRect m_Rect;
		m_Graph.GetWindowRect(m_Rect);
		ScreenToClient(m_Rect);
		InvalidateRect(m_Rect, TRUE);
		PostMessage(WM_PAINT,0,0);
	}

	CDialog::OnTimer(nIDEvent);
}

void CSpectronicDlg::OnFichierPrint() 
{
	CPrintDialog print(FALSE);
	BOOL response = print.DoModal();
	if(response == IDOK)
	{
		CDC* printerDC = CDC::FromHandle(print.GetPrinterDC());

		CString strTitle;
		strTitle.LoadString(AFX_IDS_APP_TITLE);

		DOCINFO info;
		ZeroMemory(&info, sizeof(DOCINFO));
		info.cbSize = sizeof(DOCINFO);
		info.lpszDocName = strTitle;

		printerDC->StartDoc(&info);		// Commencement de l'impression

			CPrintInfo printinfo;
			printinfo.m_rectDraw.SetRect(0,0, 
									printerDC->GetDeviceCaps(HORZRES), 
									printerDC->GetDeviceCaps(VERTRES));
			CRect m_Rect;
			m_Rect.top = 100;
			m_Rect.bottom = printerDC->GetDeviceCaps(VERTRES) - 100;
			m_Rect.left = 100;
			m_Rect.right = printerDC->GetDeviceCaps(HORZRES) - 100;

			printerDC->StartPage();		// Début d'une page


				if (GraphDlg->m_XAuto) 

				{

					GraphDlg->m_XUs=2;

					GraphDlg->m_XUp=20;

				}

				if (GraphDlg->m_YAuto) 

				{

					GraphDlg->m_YUs=2;

					GraphDlg->m_YUp=.5;

				}
				m_Value.DrawGraph(&m_Rect,GraphDlg->m_XUp,GraphDlg->m_XUs,GraphDlg->m_YUp,GraphDlg->m_YUs,printerDC,TRUE);

				printerDC->SetTextAlign(TA_TOP|TA_CENTER);
				printerDC->TextOut(printerDC->GetDeviceCaps(HORZRES)/2,150,InfoDlg->GetName().c_str());
			printerDC->EndPage();		// Fin d'une page

		printerDC->EndDoc();			// Fin de l'impression

		printerDC->Detach();
		DefaultDlg->m_Log.AddNews("Impression du graphique");
	}
}


void CSpectronicDlg::OnDestroy() 
{
	if(DefaultDlg)
		delete DefaultDlg;
	if(CommandDlg)
		delete CommandDlg;
	if(BlankDlg)
		delete BlankDlg;
	if(ScanDlg)
		delete ScanDlg;
	if(InfoDlg)
		delete InfoDlg;
	if(BigGraphDlg)
		delete BigGraphDlg;
	if(GraphDlg)
		delete GraphDlg;
	Spectronic->DestroyInstance();

	CDialog::OnDestroy();
}
