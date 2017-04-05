// MainGraph.cpp : implementation file
//

#include "stdafx.h"
#include "Spectronic.h"
#include "MainGraph.h"
#include "SpectronicDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainGraph dialog


CMainGraph::CMainGraph(CWnd* pParent /*=NULL*/)
	: CDialog(CMainGraph::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMainGraph)
	m_Color = FALSE;
	m_XAuto = FALSE;
	m_YAuto = FALSE;
	m_XMax = 0.0;
	m_XMin = 0.0;
	m_XUp = 0;
	m_XUs = 0;
	m_YMax = 0.0;
	m_YMin = 0.0;
	m_YUp = .1;
	m_YUs = 10;
	//}}AFX_DATA_INIT
	parent = pParent;
}


void CMainGraph::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainGraph)
	DDX_Control(pDX, IDC_YUS, c_YUs);
	DDX_Control(pDX, IDC_YUP, c_YUp);
	DDX_Control(pDX, IDC_YMIN, c_YMin);
	DDX_Control(pDX, IDC_YMAX, c_YMax);
	DDX_Control(pDX, IDC_XUS, c_XUs);
	DDX_Control(pDX, IDC_XUP, c_XUp);
	DDX_Control(pDX, IDC_XMIN, c_XMin);
	DDX_Control(pDX, IDC_XMAX, c_XMax);
	DDX_Control(pDX, IDC_GRADS, m_Grads);
	DDX_Control(pDX, IDC_DROITE, m_Droite);
	DDX_Control(pDX, IDC_BACK, m_Back);
	DDX_Check(pDX, IDC_DEFAULTCOLORS, m_Color);
	DDX_Check(pDX, IDC_XAUTOMATIC, m_XAuto);
	DDX_Check(pDX, IDC_YAUTOMATIC, m_YAuto);
	DDX_Text(pDX, IDC_XMAX, m_XMax);
	DDX_Text(pDX, IDC_XMIN, m_XMin);
	DDX_Text(pDX, IDC_XUP, m_XUp);
	DDX_Text(pDX, IDC_XUS, m_XUs);
	DDX_Text(pDX, IDC_YMAX, m_YMax);
	DDX_Text(pDX, IDC_YMIN, m_YMin);
	DDX_Text(pDX, IDC_YUP, m_YUp);
	DDX_Text(pDX, IDC_YUS, m_YUs);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMainGraph, CDialog)
	//{{AFX_MSG_MAP(CMainGraph)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_DEFAULTCOLORS, OnDefaultcolors)
	ON_BN_CLICKED(IDC_YAUTOMATIC, OnYAutomatic)
	ON_BN_CLICKED(IDC_XAUTOMATIC, OnXAutomatic)
	ON_EN_KILLFOCUS(IDC_XUP, OnKillfocusXup)
	ON_EN_KILLFOCUS(IDC_XUS, OnKillfocusXus)
	ON_EN_KILLFOCUS(IDC_YUP, OnKillfocusYup)
	ON_EN_KILLFOCUS(IDC_YUS, OnKillfocusYus)
	ON_EN_KILLFOCUS(IDC_YMIN, OnKillfocusYmin)
	ON_EN_KILLFOCUS(IDC_YMAX, OnKillfocusYmax)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainGraph message handlers

BOOL CMainGraph::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	clrBack = RGB(255, 255, 255);
	clrDroite = RGB(0, 0, 0);
	clrGrads = RGB(0, 0, 0);

	m_XAuto = TRUE;
	m_YAuto = TRUE;
	m_XUp = 2;
	m_XUs = 20;
	m_YUp = .1;
	m_YUs = 1;
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMainGraph::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	UpdateData(TRUE);
	if(m_Color == FALSE)
	{
		CRect m_Rect;
		CColorDialog color;

		m_Back.GetWindowRect(m_Rect);
		ScreenToClient(m_Rect);
		if(m_Rect.PtInRect(point))
		{
			if(color.DoModal() == IDOK)
			{
				clrBack = color.GetColor();
				InvalidateRect(m_Rect, FALSE);
				PostMessage(WM_PAINT,0,0);

				CSpectronicDlg* main = (CSpectronicDlg*)parent;

				CRect m_Rect;
				main->m_Graph.GetWindowRect(m_Rect);
				main->ScreenToClient(m_Rect);

				main->m_Value.SetBackColor(clrBack);

				main->InvalidateRect(m_Rect, FALSE);
				main->UpdateWindow();
			}
		}

		m_Droite.GetWindowRect(m_Rect);
		ScreenToClient(m_Rect);
		if(m_Rect.PtInRect(point))
		{
			if(color.DoModal() == IDOK)
			{
				clrDroite = color.GetColor();
				InvalidateRect(m_Rect, FALSE);
				PostMessage(WM_PAINT,0,0);

				CSpectronicDlg* main = (CSpectronicDlg*)parent;

				CRect m_Rect;
				main->m_Graph.GetWindowRect(m_Rect);
				main->ScreenToClient(m_Rect);

				main->m_Value.SetCourbeColor(clrDroite);

				main->InvalidateRect(m_Rect, FALSE);
				main->UpdateWindow();
			}
		}

		m_Grads.GetWindowRect(m_Rect);
		ScreenToClient(m_Rect);
		if(m_Rect.PtInRect(point))
		{
			if(color.DoModal() == IDOK)
			{
				clrGrads = color.GetColor();
				InvalidateRect(m_Rect, FALSE);
				PostMessage(WM_PAINT,0,0);

				CSpectronicDlg* main = (CSpectronicDlg*)parent;

				CRect m_Rect;
				main->m_Graph.GetWindowRect(m_Rect);
				main->ScreenToClient(m_Rect);

				main->m_Value.SetAxesColor(clrGrads);

				main->InvalidateRect(m_Rect, FALSE);
				main->UpdateWindow();
			}
		}
	}

	UpdateData(FALSE);

	CDialog::OnLButtonDblClk(nFlags, point);
}

void CMainGraph::OnPaint() 
{
	CRect m_Rect;

	CPaintDC dc(this);
	dc.SetROP2(R2_COPYPEN);
	dc.SetMapMode(MM_TEXT);
	
	m_Back.GetWindowRect(m_Rect);
	ScreenToClient(m_Rect);
	dc.FillSolidRect(m_Rect, clrBack);

	m_Droite.GetWindowRect(m_Rect);
	ScreenToClient(m_Rect);
	dc.FillSolidRect(m_Rect, clrDroite);

	m_Grads.GetWindowRect(m_Rect);
	ScreenToClient(m_Rect);
	dc.FillSolidRect(m_Rect, clrGrads);
}

void CMainGraph::OnDefaultcolors() 
{
	UpdateData(TRUE);
	if(m_Color == TRUE)
	{
		clrBack = RGB(255, 255, 255);
		clrDroite = RGB(0, 0, 0);
		clrGrads = RGB(0, 0, 0);

		int i;
		CRect m_Rect[3];

		m_Back.GetWindowRect(m_Rect[0]);
		ScreenToClient(m_Rect[0]);

		m_Droite.GetWindowRect(m_Rect[1]);
		ScreenToClient(m_Rect[1]);

		m_Grads.GetWindowRect(m_Rect[2]);
		ScreenToClient(m_Rect[2]);

		for(i=0; i<3; i++)
			InvalidateRect(m_Rect[i], FALSE);
		PostMessage(WM_PAINT,0,0);

		CSpectronicDlg* main = (CSpectronicDlg*)parent;

		CRect m_Rectmp;
		main->m_Graph.GetWindowRect(m_Rectmp);
		main->ScreenToClient(m_Rectmp);

		main->m_Value.SetAxesColor(clrGrads);
		main->m_Value.SetCourbeColor(clrDroite);
		main->m_Value.SetBackColor(clrBack);

		main->InvalidateRect(m_Rectmp, FALSE);
		main->UpdateWindow();
	}

	UpdateData(FALSE);
}

void CMainGraph::OnYAutomatic() 
{
	UpdateData(TRUE);

	if(m_YAuto == TRUE)
	{
		c_YMin.EnableWindow(FALSE);
		c_YMax.EnableWindow(FALSE);
		c_YUp.EnableWindow(FALSE);
		c_YUs.EnableWindow(FALSE);

		m_YUp = .1;
		m_YUs = 1;

		CSpectronicDlg* main = (CSpectronicDlg*)parent;

		CRect m_Rect;
		main->m_Graph.GetWindowRect(m_Rect);
		main->ScreenToClient(m_Rect);

		main->InvalidateRect(m_Rect, FALSE);
		main->UpdateWindow();

		MessageBox("L'échelle des Y du graphique s'ajustera en temps réel lors du scan.", "Information", MB_ICONINFORMATION);
	}
	else
	{
		c_YMin.EnableWindow();
		c_YMax.EnableWindow();
		c_YUp.EnableWindow();
		c_YUs.EnableWindow();
	}

	UpdateData(FALSE);
}

void CMainGraph::OnXAutomatic() 
{
	UpdateData(TRUE);

	if(m_XAuto == TRUE)
	{
		c_XMin.EnableWindow(FALSE);
		c_XMax.EnableWindow(FALSE);
		c_XUp.EnableWindow(FALSE);
		c_XUs.EnableWindow(FALSE);
		m_XUp = 2;
		m_XUs = 20;

		CSpectronicDlg* main = (CSpectronicDlg*)parent;

		CRect m_Rect;
		main->m_Graph.GetWindowRect(m_Rect);
		main->ScreenToClient(m_Rect);

		main->InvalidateRect(m_Rect, FALSE);
		main->UpdateWindow();

		MessageBox("L'échelle des X du graphique s'ajustera en temps réel lors du scan.", "Information", MB_ICONINFORMATION);
	}
	else
	{
		c_XMin.EnableWindow();
		c_XMax.EnableWindow();
		c_XUp.EnableWindow();
		c_XUs.EnableWindow();
	}

	UpdateData(FALSE);
}

void CMainGraph::OnKillfocusXup()
{
	UpdateData(TRUE);
	CSpectronicDlg* main = (CSpectronicDlg*)parent;

	CRect m_Rect;
	main->m_Graph.GetWindowRect(m_Rect);
	main->ScreenToClient(m_Rect);

	main->InvalidateRect(m_Rect, FALSE);
	main->UpdateWindow();
}

void CMainGraph::OnKillfocusXus() 
{
	UpdateData(TRUE);
	CSpectronicDlg* main = (CSpectronicDlg*)parent;

	CRect m_Rect;
	main->m_Graph.GetWindowRect(m_Rect);
	main->ScreenToClient(m_Rect);

	main->InvalidateRect(m_Rect, FALSE);
	main->UpdateWindow();
}

void CMainGraph::OnKillfocusYup() 
{
	UpdateData(TRUE);
	CSpectronicDlg* main = (CSpectronicDlg*)parent;

	CRect m_Rect;
	main->m_Graph.GetWindowRect(m_Rect);
	main->ScreenToClient(m_Rect);

	main->InvalidateRect(m_Rect, FALSE);
	main->UpdateWindow();
}

void CMainGraph::OnKillfocusYus() 
{
	UpdateData(TRUE);
	CSpectronicDlg* main = (CSpectronicDlg*)parent;

	CRect m_Rect;
	main->m_Graph.GetWindowRect(m_Rect);
	main->ScreenToClient(m_Rect);

	main->InvalidateRect(m_Rect, FALSE);
	main->UpdateWindow();
}

void CMainGraph::OnKillfocusYmin() 
{
	UpdateData(TRUE);
	CSpectronicDlg* main = (CSpectronicDlg*)parent;

	CRect m_Rect;
	main->m_Graph.GetWindowRect(m_Rect);
	main->ScreenToClient(m_Rect);

	main->InvalidateRect(m_Rect, FALSE);
	main->UpdateWindow();
}

void CMainGraph::OnKillfocusYmax() 
{
	UpdateData(TRUE);
	CSpectronicDlg* main = (CSpectronicDlg*)parent;

	CRect m_Rect;
	main->m_Graph.GetWindowRect(m_Rect);
	main->ScreenToClient(m_Rect);

	main->InvalidateRect(m_Rect, FALSE);
	main->UpdateWindow();
}
