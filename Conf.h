// Conf.h: interface for the CConf class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONF_H__7AE5789A_7A66_49C3_B87A_E13EFBA348CD__INCLUDED_)
#define AFX_CONF_H__7AE5789A_7A66_49C3_B87A_E13EFBA348CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// The structures to save configuration on the disk

struct ComConfig
{
	int ComPortNb;
	int BaudRate;
	int ByteSize;
	int Parity;
	int StopBits;
};

struct SpectroConfig
{
	char BlankFile[255];
};

struct GraphConfig
{
	int MinWV;
	int MaxWV;
	int WVint;
	int MinAbs;
	int MaxAbs;
	int AbsInt;
};

struct ProgConfig
{
	GraphConfig Graph;
	ComConfig ComPort;
	SpectroConfig Spectro;
};

struct FileInfo
{
	CString Etalon;
	CString Title;
	CString Name;
	CString Subst;
	CString Group;
	CString Date;
	double WvInc;
	int Speed;
	double StrtWv;
	double StopWv;
};

struct XY
{
	double x;
	double y;
};

#endif // !defined(AFX_CONF_H__7AE5789A_7A66_49C3_B87A_E13EFBA348CD__INCLUDED_)
