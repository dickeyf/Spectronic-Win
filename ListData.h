#if !defined(AFX_LISTDATA_H__AF3FF055_0534_4596_9CB4_46250C24F933__INCLUDED_)
#define AFX_LISTDATA_H__AF3FF055_0534_4596_9CB4_46250C24F933__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListData.h : header file
//

#include <vector>
#include "conf.h"

using namespace std;

#include "MaxDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CListData window

class CListData : public CListCtrl
{
// Construction
public:
	CListData();

	int AddData(double x, double y);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListData)
	//}}AFX_VIRTUAL

// Implementation
public:

	void GiveMaxDlgInstance(CMaxDlg* MaxList);
	XY GetNearest(double value);
	XY GetExtrapolated(double value);
	XY FindMax(double beginx, double endx);
	FileInfo LoadDataBinFromDisk(const CString path);
	bool SaveDataBinToDisk(	const CString path, FileInfo info);

	bool LoadDataFromDisk(const CString path);
	bool SaveDataToDisk(const CString path);
	
	const map<double,double> ListToMap();
	const list<double> FileToList(const CString path);

	virtual BOOL DeleteItem( int nItem );
	virtual BOOL DeleteAllItems( );

							//Des que ListData est creer, Spectronic met dans Interval
											//la valeur de l'intervale, car la classe Graph a besoin de
											//connaitre l'intervale au moment que la saisie de donnee a
											//ete effectuer.
	BOOL Etalon;							//Pour savoir si les donnees ont deja ete etalonnees
	virtual ~CListData();
	int GetCount();
	double GetXMin();
	double GetXMax();
	double GetYMin();
	double GetYMax();
	double GetXData(int Pos);
	double GetYData(int Pos);
	double GetInterval();
	void SetDataConf(BOOL Etal,double WVInc);
	void DrawGraph(CRect *Area, float XGraduation, int XGraduationLabel, float YGraduation, 
					   int YGraduationLabel, CDC *DestDC, BOOL Print);

	void SetBackColor(COLORREF bkcolor) {Graphic->SetBackColor(bkcolor);}
	void SetCourbeColor(COLORREF courbecolor) {Graphic->SetCourbeColor(courbecolor);}
	void SetAxesColor(COLORREF axescolor) {Graphic->SetAxesColor(axescolor);}

private:	
	int count;
	double XMin;
	double XMax;
	double YMin;
	double YMax;
	double Interval;
	vector<double> XDatasCache;
	vector<double> YDatasCache;

	// Generated message map functions

	class Graph
	{
		public:

		Graph(CListData *List);
		~Graph();

		void SaveGraphBmp(CDC *Source,char *path);
		void GiveMaxDlgInstance(CMaxDlg* MaxList);
		void DisplayGraph(CRect *Area, float XGraduation, int XGraduationLabel, float YGraduation, 
					   int YGraduationLabel, CDC *DestDC, BOOL Print);

		void DrawMaxTable(CRect* Area, CDC *DestDC);
		void ZoomBox(float StartWV, float StopWV, float AbsMin, float AbsMax);
		void FullView();

		void SetBackColor(COLORREF bkcolor) {bk = bkcolor;}
		void SetCourbeColor(COLORREF courbecolor) {courbe = courbecolor;}
		void SetAxesColor(COLORREF axescolor) {axes = axescolor;}

		private:
		void DrawAxes(CRect *Area, double XMin, double XMax, double YMin, double YMax,
								double XGraduation, int XGraduationLabel, double YGraduation, 
								int YGraduationLabel, CDC *DestDC, BOOL Print);
		void SetGraphMode(CDC* DestDC);
		void DrawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, CDC *DestDC);
		CListData* DataList;
		float ZoomStartWV;
		float ZoomStopWV;
		float ZoomAbsMin;
		float ZoomAbsMax;
		bool Zoom;
		COLORREF bk;		// Couleur du background
		COLORREF courbe;	// Couleur de la courbe
		COLORREF axes;		// Couleur des axes

		CMaxDlg* MaxInst;



		class GraphBitmap
		{
			public:

			GraphBitmap(int Width, int Height, HDC DestDC);
			~GraphBitmap();
			bool DisplayBmp(int XPos, int YPos, HDC DestDC);

			void DrawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
			void PutPixel(unsigned int x, unsigned int y);
			void SaveBitmap(char* path);

			private:
			struct Bmp 
			{
				HDC			BmpDC;
				HBITMAP		BmpHandle;
				BITMAPINFO	BmpInfo;
				RGBQUAD		Color2;
				int         ScanSize;
				int			ScanNum;
				char*		DIB;
			};
			Bmp		Bitmap;
			CListData* DataList;
		};
	};

	Graph		*Graphic;
	

protected:
	
	//{{AFX_MSG(CListData)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnCustomdraw ( NMHDR* pNMHDR, LRESULT* pResult );
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTDATA_H__AF3FF055_0534_4596_9CB4_46250C24F933__INCLUDED_)
