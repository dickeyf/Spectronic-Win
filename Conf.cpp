// Conf.cpp: implementation of the CConf class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Spectronic.h"
#include "Conf.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConf::CConf():	ComPortNb(2),
				BaudRate(4800),
				ByteSize(8),
				Parity(NOPARITY),
				StopBits(ONESTOPBIT),
				InputBuffer(1024),
				OutputBuffer(512)
{
	
}

CConf::~CConf()
{
	
}
