// MainBlank.cpp : implementation file
//

#include "stdafx.h"
#include "Spectronic.h"
#include "MainBlank.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainBlank dialog


CMainBlank::CMainBlank(CWnd* pParent /*=NULL*/)
	: CDialog(CMainBlank::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainBlank)
	m_Path = _T("");
	m_Name = _T("");
	m_Sel = FALSE;
	//}}AFX_DATA_INIT
}


void CMainBlank::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainBlank)
	DDX_Control(pDX, IDC_DELETE, m_Delete);
	DDX_Control(pDX, IDC_SEL, m_SelCtrl);
	DDX_Control(pDX, IDC_LIST, m_List);
	DDX_Control(pDX, IDC_AJOUTER, m_Add);
	DDX_Control(pDX, IDC_BROWSE, m_Browse);
	DDX_Control(pDX, IDC_PATH, m_PathCtrl);
	DDX_Control(pDX, IDC_NOM, m_NameCtrl);
	DDX_Text(pDX, IDC_PATH, m_Path);
	DDX_Text(pDX, IDC_NOM, m_Name);
	DDX_Check(pDX, IDC_SEL, m_Sel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMainBlank, CDialog)
	//{{AFX_MSG_MAP(CMainBlank)
	ON_BN_CLICKED(IDC_BROWSE, OnBrowse)
	ON_EN_CHANGE(IDC_PATH, OnChangePath)
	ON_EN_CHANGE(IDC_NOM, OnChangeName)
	ON_BN_CLICKED(IDC_AJOUTER, OnAjouter)
	ON_NOTIFY(NM_CLICK, IDC_LIST, OnClickList)
	ON_BN_CLICKED(IDC_DELETE, OnDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainBlank message handlers

BOOL CMainBlank::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect m_Rect;
	m_List.GetWindowRect(m_Rect);
	ScreenToClient(m_Rect);
	int width = m_Rect.Width() / 4;

	ListView_SetExtendedListViewStyleEx (	m_List.GetSafeHwnd(), 
											LVS_EX_FULLROWSELECT, 
											LVS_EX_FULLROWSELECT );

	m_List.InsertColumn(0, "Nom", LVCFMT_LEFT, width);
	m_List.InsertColumn(1, "Fichier", LVCFMT_CENTER, m_Rect.Width()-width-20);

	// Cherche dans le répertoir ou nous sommes pour trouver des fichiers étalons

	vector< string > hFile;
	LPWIN32_FIND_DATA finddata = new WIN32_FIND_DATA;
	
	string path;
	HANDLE search;

	path = "c:\\spectronic\\debug\\*.blk";

	search = FindFirstFile(path.c_str(), finddata);

	if(search != INVALID_HANDLE_VALUE)
		hFile.push_back(finddata->cFileName);

	while(search != INVALID_HANDLE_VALUE)
	{
		if(FindNextFile(search, finddata))
			hFile.push_back(finddata->cFileName);
		else
			break;
	}

	delete finddata;
	FindClose(search);

	int i;
	int size = hFile.size();
	string name;
	string::const_iterator begin;
	string::const_iterator end;
	string::const_iterator pos;

	for(i=0; i<size; i++)
	{
		begin = hFile[i].begin();
		end = hFile[i].end()-4;
		for(pos = begin; pos<end; pos++)
			name += *pos;
		m_List.InsertItem(0,name.c_str());
		m_List.SetItem(0, 1, LVIF_TEXT , hFile[i].c_str(), NULL, NULL, NULL, NULL);
	}

	if(size > 0)
		m_List.EnableWindow(TRUE);

	UpdateData(FALSE);

	return TRUE;
}

void CMainBlank::OnBrowse() 
{
	CFileDialog file(TRUE, "blk", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, "Fichier Étalon (*.blk)|*.blk|", this);
	int result = file.DoModal();
	if(result == IDOK)
	{
		CString path = file.GetPathName();
		m_Path = path;

		m_NameCtrl.EnableWindow(TRUE);
		m_Name = file.GetFileTitle();
		m_Add.EnableWindow(TRUE);

		UpdateData(FALSE);
	}
	else if(result == IDCANCEL)
	{
		
	}
}

void CMainBlank::OnChangePath() 
{
	UpdateData(TRUE);

	if(m_Path != "")
		m_NameCtrl.EnableWindow(TRUE);
	else
		m_NameCtrl.EnableWindow(FALSE);

	UpdateData(FALSE);
}

void CMainBlank::OnChangeName() 
{
	UpdateData(TRUE);

	if(m_Name != "")
		m_Add.EnableWindow(TRUE);
	else
		m_Add.EnableWindow(FALSE);

	UpdateData(FALSE);
}

void CMainBlank::OnAjouter() 
{
	UpdateData(TRUE);

	m_List.InsertItem(0,m_Name);
	m_List.SetItem(0, 1, LVIF_TEXT , m_Path, NULL, NULL, NULL, NULL);

	m_Path.Empty();
	m_Name.Empty();

	m_NameCtrl.EnableWindow(FALSE);
	m_Add.EnableWindow(FALSE);

	m_List.EnableWindow(TRUE);

	UpdateData(FALSE);
}

void CMainBlank::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);

	if(m_List.GetItemCount() != 0 && m_List.GetSelectionMark() != -1)
	{
		m_Delete.EnableWindow(TRUE);
		m_SelCtrl.EnableWindow(TRUE);
	}

	UpdateData(FALSE);
	
	*pResult = 0;
}

void CMainBlank::OnDelete() 
{
	UpdateData(TRUE);

	m_List.DeleteItem(m_List.GetSelectionMark());
	m_Delete.EnableWindow(FALSE);

	if(m_List.GetItemCount() == 0)
		m_List.EnableWindow(FALSE);

	m_Delete.EnableWindow(FALSE);
	m_Sel = FALSE;
	m_SelCtrl.EnableWindow(FALSE);

	UpdateData(FALSE);
}

void CMainBlank::OnOK()
{
	
}
