// ListLog.cpp : implementation file
//

#include "stdafx.h"
#include "Spectronic.h"
#include "ListLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CListLog::CListLog()
{
	count = 0;
}

CListLog::~CListLog()
{
}

int CListLog::InsertToLog(int nIndex, LPCTSTR lpszItem)
{
	CTime time = CTime::GetCurrentTime();
	CString strtime = time.Format("%H:%M:%S");

	string temp;
	vector<int>::iterator position = Message.begin();
	position += nIndex;
	Message.insert(position, LOG_NORMAL);

	temp = "> ";
	temp += lpszItem;
	count++;

	int result = CListCtrl::InsertItem(nIndex, strtime);
	CListCtrl::SetItem(nIndex, 1, LVIF_TEXT , temp.c_str(), NULL, NULL, NULL, NULL);

	return result;
}

int CListLog::AddToLog(LPCTSTR lpszItem)
{
	CTime time = CTime::GetCurrentTime();
	CString strtime = time.Format("%H:%M:%S");

	string temp;
	vector<int>::iterator position = Message.begin();
	position += count;
	Message.insert(position, LOG_NORMAL);

	temp = "> ";
	temp += lpszItem;

	int result = CListCtrl::InsertItem(count, strtime);
	CListCtrl::SetItem(count, 1, LVIF_TEXT , temp.c_str(), NULL, NULL, NULL, NULL);

	CListCtrl::EnsureVisible(count, FALSE);
	count++;

	return result;
}

BOOL CListLog::DeleteItem(UINT nIndex)
{
	vector<int>::iterator node = Message.begin();
	node += nIndex;
	Message.erase(node);

	count--;

	return CListCtrl::DeleteItem(nIndex);
}

BOOL CListLog::DeleteAllItems()
{
	Message.clear();
	return CListCtrl::DeleteAllItems();
}

int CListLog::AddError(LPCTSTR lpszItem)
{
	CTime time = CTime::GetCurrentTime();
	CString strtime = time.Format("%H:%M:%S");

	string temp;
	vector<int>::iterator position = Message.begin();
	position += count;
	Message.insert(position, LOG_ERROR);

	temp = "> ";
	temp += lpszItem;

	int result = CListCtrl::InsertItem(count, strtime);
	CListCtrl::SetItem(count, 1, LVIF_TEXT , temp.c_str(), NULL, NULL, NULL, NULL);

	CListCtrl::EnsureVisible(count, FALSE);
	count++;

	return result;
}

int CListLog::AddNews(LPCTSTR lpszItem)
{
	CTime time = CTime::GetCurrentTime();
	CString strtime = time.Format("%H:%M:%S");

	string temp;
	vector<int>::iterator position = Message.begin();
	position += count;
	Message.insert(position, LOG_VERT);

	temp = "> ";
	temp += lpszItem;

	int result = CListCtrl::InsertItem(count, strtime);
	CListCtrl::SetItem(count, 1, LVIF_TEXT , temp.c_str(), NULL, NULL, NULL, NULL);

	CListCtrl::EnsureVisible(count, FALSE);
	count++;

	return result;
}


BEGIN_MESSAGE_MAP(CListLog, CListCtrl)
	//{{AFX_MSG_MAP(CListLog)
		ON_NOTIFY_REFLECT ( NM_CUSTOMDRAW, OnCustomdrawLog )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListLog message handlers

void CListLog::OnCustomdrawLog(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

    // Take the default processing unless we set this to something else below.
    *pResult = 0;

    // First thing - check the draw stage. If it's the control's prepaint
    // stage, then tell Windows we want messages for every item.
    if (pLVCD->nmcd.dwDrawStage == CDDS_PREPAINT)
	{
        *pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (pLVCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
	{
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if (pLVCD->nmcd.dwDrawStage == (CDDS_ITEMPREPAINT | CDDS_SUBITEM))
	{
		COLORREF crText, crBk = RGB(255,255,255);

		if(pLVCD->iSubItem == 0)
		{
			crText = RGB(0,0,0);
			crBk = RGB(255,255,255);
		}
		else if(pLVCD->iSubItem == 1)
		{
			if (Message[pLVCD->nmcd.dwItemSpec] == LOG_ERROR)
			{
				crText = RGB(239,7,7);
				crBk = RGB(255,235,235);
			}
			else if (Message[pLVCD->nmcd.dwItemSpec] == LOG_VERT)
			{
				crText = RGB(17,123,17);
				crBk = RGB(235,255,235);
			}
			else
			{
				crText = RGB(0,0,0);
				crBk = RGB(235,235,255);
			}
		}

		// Store the color back in the NMLVCUSTOMDRAW struct.
		pLVCD->clrText = crText;
		pLVCD->clrTextBk = crBk;

		*pResult = CDRF_DODEFAULT;
	}
}