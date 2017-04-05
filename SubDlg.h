// SubDlg.h: interface for the CSubDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SUBDLG_H__DC31CE00_4B4E_4437_AA5F_397016B42A36__INCLUDED_)
#define AFX_SUBDLG_H__DC31CE00_4B4E_4437_AA5F_397016B42A36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSubDlg : public CDialog
{
public:

	CSubDlg();
	CSubDlg(UINT nID, CWnd *pParent = NULL);
	virtual ~CSubDlg();

	UINT const GetID() const {return ID;}

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnOK();
	virtual void OnCancel();

	UINT ID;

};

#endif // !defined(AFX_SUBDLG_H__DC31CE00_4B4E_4437_AA5F_397016B42A36__INCLUDED_)
