// FullGraphDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Spectronic.h"
#include "FullGraphDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFullGraphDlg dialog


CFullGraphDlg::CFullGraphDlg(CWnd* pParent /*=NULL*/
							 ,CListData* Data

							 ,CMainGraph* setting)
	: CDialog(CFullGraphDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFullGraphDlg)
	//}}AFX_DATA_INIT
	DataList=Data;

	Settings=setting;



	MaxDlg = new CMaxDlg;



	MaxDlg->Create(IDD_MAXDLG,this);
}

void CFullGraphDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFullGraphDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFullGraphDlg, CDialog)
	//{{AFX_MSG_MAP(CFullGraphDlg)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFullGraphDlg message handlers

BOOL CFullGraphDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CursorPainting = false;
	LimitCur1 = false;
	LimitCur2 = false;
	MaxCur = false;
	DragDrop = false;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFullGraphDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CRect m_Rect;

	GetWindowRect(m_Rect);
	ScreenToClient(m_Rect);

	m_Rect.SetRect(m_Rect.left+10,m_Rect.top+30,m_Rect.right-10,m_Rect.bottom-20);

	static bool first=false;

	if(!first)
		first = true;
	else
	{
		CBrush brush;
		brush.CreateSolidBrush(RGB(255, 255, 255));
		dc.FillRect(m_LastCur, &brush);
	}


	Settings->UpdateData(false);

	if (Settings->m_XAuto) 

	{

		Settings->m_XUs=2;

		Settings->m_XUp=20;

	}

	if (Settings->m_YAuto) 

	{

		Settings->m_YUs=2;

		Settings->m_YUp=.5;

	}
	DataList->DrawGraph(&m_Rect,Settings->m_XUp,Settings->m_XUs,Settings->m_YUp,Settings->m_YUs,&dc,FALSE);
	
	if(CursorPainting)
	{
		InvalidateRect(m_Cursor, TRUE);

		CPaintDC dc(this); // device context for painting
		dc.SetROP2(R2_COPYPEN);
		dc.SetMapMode(MM_TEXT);

		CBrush brush;
		brush.CreateSolidBrush(RGB(0, 0, 0));
		
		dc.SetBkMode(TRANSPARENT);

		dc.FillRect(m_Cursor, &brush);

		dc.SetPixel(m_LastCur.left,m_LastCur.top, RGB(255, 255, 255));
		dc.SetPixel(m_LastCur.left,m_LastCur.bottom-1, RGB(255, 255, 255));
		dc.SetPixel(m_LastCur.right-1,m_LastCur.top, RGB(255, 255, 255));
		dc.SetPixel(m_LastCur.right-1,m_LastCur.bottom-1, RGB(255, 255, 255));

		m_LastCur = m_Cursor;
		CursorPainting = false;
	}

	if (LimitCur1)
	{
		InvalidateRect(m_IntervalLimit1, TRUE);
		CPaintDC dc(this); // device context for painting
		dc.SetROP2(R2_COPYPEN);
		dc.SetMapMode(MM_TEXT);

		CBrush brush;
		brush.CreateSolidBrush(RGB(255, 0, 0));
		
		dc.SetBkMode(TRANSPARENT);

		dc.FillRect(m_IntervalLimit1, &brush);

		dc.SetPixel(m_IntervalLimit1.left,m_IntervalLimit1.top, RGB(255, 255, 255));
		dc.SetPixel(m_IntervalLimit1.left,m_IntervalLimit1.bottom-1, RGB(255, 255, 255));
		dc.SetPixel(m_IntervalLimit1.right-1,m_IntervalLimit1.top, RGB(255, 255, 255));
		dc.SetPixel(m_IntervalLimit1.right-1,m_IntervalLimit1.bottom-1, RGB(255, 255, 255));
	}
	
	if (LimitCur2)
	{
		InvalidateRect(m_IntervalLimit2, TRUE);
		CPaintDC dc(this); // device context for painting
		dc.SetROP2(R2_COPYPEN);
		dc.SetMapMode(MM_TEXT);

		CBrush brush;
		brush.CreateSolidBrush(RGB(255, 0, 0));
		
		dc.SetBkMode(TRANSPARENT);

		dc.FillRect(m_IntervalLimit2, &brush);

		dc.SetPixel(m_IntervalLimit2.left,m_IntervalLimit2.top, RGB(255, 255, 255));
		dc.SetPixel(m_IntervalLimit2.left,m_IntervalLimit2.bottom-1, RGB(255, 255, 255));
		dc.SetPixel(m_IntervalLimit2.right-1,m_IntervalLimit2.top, RGB(255, 255, 255));
		dc.SetPixel(m_IntervalLimit2.right-1,m_IntervalLimit2.bottom-1, RGB(255, 255, 255));
	}

	if(MaxCur)
	{
		InvalidateRect(m_MaxCur, TRUE);
		CPaintDC dc(this); // device context for painting
		dc.SetROP2(R2_COPYPEN);
		dc.SetMapMode(MM_TEXT);

		CBrush brush;
		brush.CreateSolidBrush(RGB(0, 0, 196));
		
		dc.SetBkMode(TRANSPARENT);

		dc.FillRect(m_MaxCur, &brush);

		dc.SetPixel(m_MaxCur.left,m_MaxCur.top, RGB(255, 255, 255));
		dc.SetPixel(m_MaxCur.left,m_MaxCur.bottom-1, RGB(255, 255, 255));
		dc.SetPixel(m_MaxCur.right-1,m_MaxCur.top, RGB(255, 255, 255));
		dc.SetPixel(m_MaxCur.right-1,m_MaxCur.bottom-1, RGB(255, 255, 255));
	}

	CDialog::OnPaint();
}

void CFullGraphDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	LimitCur1 = false;
	LimitCur2 = false;
	MaxCur = false;

	Invalidate();
	SendMessage(WM_PAINT,0,0);

	GetWindowRect(m_Graph);
	ScreenToClient(m_Graph);
	m_Graph.SetRect(m_Graph.left+10,m_Graph.top+30,m_Graph.right-10,m_Graph.bottom-20);
}

void CFullGraphDlg::OnClose() 
{
	MaxDlg->ShowWindow(SW_HIDE);

	LimitCur1 = false;
	LimitCur2 = false;
	MaxCur = false;
	DragDrop = false;

	MaxDlg->m_Xy = "0,0";
	MaxDlg->m_Begin = 0;
	MaxDlg->m_End = 0;
	MaxDlg->m_Max = "0,0";

	CDialog::OnClose();
}

void CFullGraphDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	delete MaxDlg;
}

void CFullGraphDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	if((nFlags & MK_SHIFT) || DragDrop)
	{
		if(m_Graph.PtInRect(point))
		{
			CRect m_Rect;
			CPoint coord = point;		// Le point par rapport au graphique
			coord.x -= m_Graph.left;
			coord.y -= m_Graph.top;
			int size = DataList->GetCount();	// Le nombre de données
			

			double YMax=DataList->GetYMax();

			double XMax=DataList->GetXMax();

			double YMin=DataList->GetYMin();

			double XMin=DataList->GetXMin();



			double Height=(YMax-YMin);

			double YScale=Height/m_Graph.Height();



			if (YMin > -15*YScale)

			{

				YMin=-15*YScale;

				Height=(YMax-YMin);

				YScale=Height/m_Graph.Height();

			}

			

			double Yfactor = (YMax - YMin) / m_Graph.Height(); // Facteur de convertion en Y


			double Xfactor = (XMax - XMin) / m_Graph.Width();	// Facteur de convertion en X
				
			double Xdlambda = coord.x * Xfactor;
			double Xminlambda = XMin;
			double Xposlambda = Xdlambda + Xminlambda;	// La position des X que nous sommes réellement

			XY pos = DataList->GetExtrapolated(Xposlambda);
			XY realpos = pos;
			pos.x -= XMin;
			pos.y -= YMin;

			pos.x /= Xfactor;
			pos.y /= Yfactor;

			pos.y -= m_Graph.Height();
			pos.y *= -1;

			pos.y += m_Graph.top;
			pos.x += m_Graph.left;

			m_Cursor.top = pos.y - 3;
			m_Cursor.bottom = m_Cursor.top + 6;
			m_Cursor.left = pos.x - 3;
			m_Cursor.right = m_Cursor.left + 6;

			CursorPainting = true;
			InvalidateRect(m_Cursor, FALSE);
			InvalidateRect(m_LastCur, FALSE);
			SendMessage(WM_PAINT,0,0);

			MaxDlg->m_Xy.Format("%g , %g", realpos.x, realpos.y);

			MaxDlg->UpdateData(FALSE);
		}
	}
	
	CDialog::OnMouseMove(nFlags, point);
}

void CFullGraphDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	MaxDlg->SetFocus();
	MaxDlg->ShowWindow(SW_SHOW);
	if(nFlags & MK_SHIFT)
	{
		InvalidateRect(m_IntervalLimit2, TRUE);
		InvalidateRect(m_IntervalLimit1, TRUE);
		InvalidateRect(m_MaxCur, TRUE);

		LimitCur1=true;
		LimitCur2=false;
		MaxCur = false;
		CRect m_Rect;
		CPoint coord = point;		// Le point par rapport au graphique
		coord.x -= m_Graph.left;
		coord.y -= m_Graph.top;
		int size = DataList->GetCount();	// Le nombre de données

		double YMax=DataList->GetYMax();
		double YMin=DataList->GetYMin();

		double Height=(YMax-YMin);
		double YScale=Height/m_Graph.Height();
		if (YMin > -15*YScale)
		{
			YMin=-15*YScale;
			Height=(YMax-YMin);
			YScale=Height/m_Graph.Height();
		}

		double Xfactor = (DataList->GetXMax() - DataList->GetXMin()) / m_Graph.Width();	// Facteur de convertion en X
		double Yfactor = (YMax - YMin) / m_Graph.Height(); // Facteur de convertion en Y
				
		double Xdlambda = coord.x * Xfactor;
		double Xminlambda = DataList->GetXMin();
		double Xposlambda = Xdlambda + Xminlambda;	// La position des X que nous sommes réellement

		XY pos = DataList->GetExtrapolated(Xposlambda);
		BeginX=Xposlambda;
		MaxDlg->m_Begin = BeginX;
		MaxDlg->UpdateData(FALSE);

		XY realpos = pos;
		pos.x -= DataList->GetXMin();
		pos.y -= YMin;

		pos.x /= Xfactor;
		pos.y /= Yfactor;

		pos.y -= m_Graph.Height();
		pos.y *= -1;

		pos.y += m_Graph.top;
		pos.x += m_Graph.left;

		m_IntervalLimit1.top = pos.y - 3;
		m_IntervalLimit1.bottom = m_IntervalLimit1.top + 6;
		m_IntervalLimit1.left = pos.x - 3;
		m_IntervalLimit1.right = m_IntervalLimit1.left + 6;

		CursorPainting = true;
		DragDrop = true;
		InvalidateRect(m_IntervalLimit1, FALSE);
		InvalidateRect(m_MaxCur, FALSE);
		SendMessage(WM_PAINT,0,0);

		MaxDlg->m_Xy.Format("%g , %g", realpos.x, realpos.y);

		MaxDlg->UpdateData(FALSE);
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CFullGraphDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(nFlags & MK_SHIFT)
	{
		InvalidateRect(m_IntervalLimit2, TRUE);
		InvalidateRect(m_IntervalLimit1, TRUE);
		LimitCur2=true;
		m_IntervalLimit2 = m_IntervalLimit1;
		CRect m_Rect;
		CPoint coord = point;		// Le point par rapport au graphique
		coord.x -= m_Graph.left;
		coord.y -= m_Graph.top;
		int size = DataList->GetCount();	// Le nombre de données

		double YMax=DataList->GetYMax();
		double YMin=DataList->GetYMin();

		double Height=(YMax-YMin);
		double YScale=Height/m_Graph.Height();
		if (YMin > -15*YScale)
		{
			YMin=-15*YScale;
			Height=(YMax-YMin);
			YScale=Height/m_Graph.Height();
		}

		double Xfactor = (DataList->GetXMax() - DataList->GetXMin()) / m_Graph.Width();	// Facteur de convertion en X
		double Yfactor = (YMax - YMin) / m_Graph.Height(); // Facteur de convertion en Y
				
		double Xdlambda = coord.x * Xfactor;
		double Xminlambda = DataList->GetXMin();
		double Xposlambda = Xdlambda + Xminlambda;	// La position des X que nous sommes réellement

		XY pos = DataList->GetExtrapolated(Xposlambda);
		EndX=Xposlambda;
		MaxDlg->m_End = EndX;

		pos.x -= DataList->GetXMin();
		pos.y -= YMin;

		pos.x /= Xfactor;
		pos.y /= Yfactor;

		pos.y -= m_Graph.Height();
		pos.y *= -1;

		pos.y += m_Graph.top;
		pos.x += m_Graph.left;

		m_IntervalLimit1.top = pos.y - 3;
		m_IntervalLimit1.bottom = m_IntervalLimit1.top + 6;
		m_IntervalLimit1.left = pos.x - 3;
		m_IntervalLimit1.right = m_IntervalLimit1.left + 6;

		XY Maximum = DataList->FindMax(BeginX, EndX);

		m_MaxCur.top = ((((Maximum.y-YMin)/Yfactor)-m_Graph.Height())*-1)+m_Graph.top - 3;
		m_MaxCur.bottom =  m_MaxCur.top + 6;
		m_MaxCur.left = ((Maximum.x-DataList->GetXMin())/Xfactor)+m_Graph.left - 3;
		m_MaxCur.right = m_MaxCur.left + 6;

		MaxCur = true;
		DragDrop = false;
		InvalidateRect(m_IntervalLimit2, FALSE);
		InvalidateRect(m_MaxCur);
		SendMessage(WM_PAINT,0,0);

		MaxDlg->m_Max.Format("%g , %g", Maximum.x, Maximum.y);

		CString x,y;
		x.Format("%g", Maximum.x);
		y.Format("%g", Maximum.y);

		MaxDlg->m_List.InsertItem(0,x);
		MaxDlg->m_List.SetItem(0, 1, LVIF_TEXT , y, NULL, NULL, NULL, NULL);

		MaxDlg->UpdateData(FALSE);
	}

	CDialog::OnLButtonUp(nFlags, point);
}

BOOL CFullGraphDlg::PreTranslateMessage(MSG* pMsg) 
{
	if((pMsg->message == WM_KEYDOWN) && ((pMsg->wParam == VK_LEFT) || (pMsg->wParam == VK_RIGHT)) )
	{
		if(pMsg->wParam == VK_LEFT)
		{
			
		}
		else if(pMsg->wParam == VK_RIGHT)
		{
			
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}
