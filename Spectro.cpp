//////////////////////////////////////////////////////////////////////
// Auteur:		François Dickey and Etienne Tremblay
// Date:		22/01/2001
// Description:	Class to communicate with the Spectronic 1201.
//				Basicly send commands and collect back some useful
//				data.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Spectronic.h"
#include "Spectro.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSpectro* CSpectro::OnlyInstance = NULL;

CSpectro::CSpectro()
{

}

CSpectro::~CSpectro()
{
	
}


CSpectro* CSpectro::GetInstance()
{
	if(OnlyInstance == NULL)
		OnlyInstance = new CSpectro;
	return OnlyInstance;
}

bool CSpectro::Init(HWND WinHandle,
					int ComPortNb,
					int baudrate /*= CBR_4800*/,
					int bytesize /*= 8*/,
					int parity /*= NOPARITY*/,
					int stopbits /*= ONESTOPBIT*/)
{
	if (!ComPort.Init(WinHandle, ComPortNb, baudrate, bytesize, parity, stopbits))
		return false;
	Reset();
	return true;
}

bool CSpectro::Reset()
{
	ComPort.SendAbort();
	return true;
}

bool CSpectro::SendCommand(const std::string command)
{
	std::string temp = command;
	temp += "\r";
	ComPort.SendString(temp.c_str(),strlen(temp.c_str()));
	return true;
}

bool CSpectro::ScanInterval(int StartWv, int StopWv, double Interval, int MCSpeed)
{
	ComDataOnOff(false);
	Test(8);
	Prt(false);
	Strtwv(StartWv);
	Stopwv(StopWv);
	Cyctm(0);
	Dlytm(0);
	Cycles(1);
	Wvinc(Interval);
	Nmmin(MCSpeed);
	Name("TST22");
	Lmpch(320);
	Redwv(620);
	Goto(StartWv);
	Abs();
	ComDataOnOff(true);
	Runs();
	return true;
}

void CSpectro::ScanDone()
{
	ComDataOnOff(false);
}

bool CSpectro::Abs()
{
	SendCommand("ABS");
	return true;
}

bool CSpectro::Abs(int absmode)
{
	char buffer[3];
	string temp;

	itoa(absmode, buffer, 10);

	temp += "ABS ";
	temp += buffer;

	SendCommand(temp);

	return true;
}

bool CSpectro::Trans()
{
	SendCommand("TRANS");
	return true;
}

bool CSpectro::Conc()
{
	SendCommand("CONC");
	return true;
}

bool CSpectro::Conc(int concmode)
{
	char buffer[3];
	string temp;

	itoa(concmode, buffer, 10);

	temp += "CONC ";
	temp += buffer;

	SendCommand(temp);

	return true;
}

	
// Data normalization commands
bool CSpectro::Autoz()
{
	SendCommand("AUTOZ");
	return true;
}

bool CSpectro::Factor()
{
	SendCommand("FACTOR");
	return true;
}

bool CSpectro::Factor(int factor)
{
	char buffer[3];
	string temp;

	itoa(factor, buffer, 10);

	temp += "FACTOR ";
	temp += buffer;

	SendCommand(temp);

	return true;
}

bool CSpectro::Cmult()
{
	SendCommand("CMULT");
	return true;
}

bool CSpectro::Cmult(int cmult)
{
	char buffer[3];
	string temp;

	itoa(cmult, buffer, 10);

	temp += "CMULT ";
	temp += buffer;

	SendCommand(temp);
	
	return true;
}

bool CSpectro::Czero()
{
	SendCommand("CZERO");
	return true;
}

bool CSpectro::Czero(int czero)
{
	char buffer[3];
	string temp;

	itoa(czero, buffer, 10);

	temp += "CZERO ";
	temp += buffer;

	SendCommand(temp);
	
	return true;
}

bool CSpectro::Lolim()
{
	SendCommand("LOLIM");
	return true;
}

bool CSpectro::Lolim(int lolim)
{
	char buffer[3];
	string temp;

	itoa(lolim, buffer, 10);

	temp += "LOLIM ";
	temp += buffer;

	SendCommand(temp);
	
	return true;
}

bool CSpectro::Hilim()
{
	SendCommand("HILIM");
	return true;
}

bool CSpectro::Hilim(int hilim)
{
	char buffer[3];
	string temp;

	itoa(hilim, buffer, 10);

	temp += "HILIM ";
	temp += buffer;

	SendCommand(temp);
	
	return true;
}

bool CSpectro::Sigav()
{
	SendCommand("SIGAV");
	return true;
}

bool CSpectro::Sigav(int sigav)
{
	char buffer[3];
	string temp;

	itoa(sigav, buffer, 10);

	temp += "SIGAV ";
	temp += buffer;

	SendCommand(temp);
	
	return true;
}


// Data output commands
bool CSpectro::ComDataOnOff(bool onoff)
{
	if(onoff)
		ComPort.DataOn();
	else
		ComPort.DataOff();
	
	return true;
}

bool CSpectro::Data()
{
	SendCommand("DATA");
	return true;
}

bool CSpectro::Format(int dataformat)
{
	char buffer[3];
	string temp;

	itoa(dataformat, buffer, 10);

	temp += "FORMAT ";
	temp += buffer;

	SendCommand(temp);
	
	return true;
}

// Monochromator control commands
bool CSpectro::Goto(int wavelength)
{
	char buffer[3];
	string temp;

	itoa(wavelength, buffer, 10);

	temp += "GOTO ";
	temp += buffer;

	SendCommand(temp);
	
	return true;
}

bool CSpectro::Comp()
{
	SendCommand("COMP");
	return true;
}

bool CSpectro::Redwv()
{
	SendCommand("REDWV");
	return true;
}

bool CSpectro::Redwv(int redwv)
{
	char buffer[3];
	string temp;

	itoa(redwv, buffer, 10);

	temp += "REDWV ";
	temp += buffer;

	SendCommand(temp);
	
	return true;
}

// Lamp Control commands
bool CSpectro::Lmpch()
{
	SendCommand("LMPCH");
	return true;
}

bool CSpectro::Lmpch(int lmpch)
{
	char buffer[3];
	string temp;

	itoa(lmpch, buffer, 10);

	temp += "LMPCH ";
	temp += buffer;

	SendCommand(temp);
	
	return true;
}


// Test parameter commands
bool CSpectro::Test(int testnb)
{
	char buffer[3];
	string temp;

	itoa(testnb, buffer, 10);

	temp += "TEST ";
	temp += buffer;

	SendCommand(temp);
	
	return true;
}

bool CSpectro::Seqid()
{
	SendCommand("SEQID");
	return true;
}

bool CSpectro::Seqid(int seqid)
{
	char buffer[3];
	string temp;

	itoa(seqid, buffer, 10);

	temp += "SEQID ";
	temp += buffer;

	SendCommand(temp);
	
	return true;
}

bool CSpectro::Name()
{
	SendCommand("NAME");
	return true;
}

bool CSpectro::Name(std::string name)
{
	string temp;

	temp += "NAME ";
	temp += name;

	SendCommand(temp);
	
	return true;
}

bool CSpectro::Dlytm()
{
	SendCommand("DLYTM");
	return true;
}

bool CSpectro::Dlytm(int delaytime)
{
	char buffer[3];
	string temp;

	itoa(delaytime, buffer, 10);

	temp += "DLYTM ";
	temp += buffer;

	SendCommand(temp);
	
	return true;
}

bool CSpectro::Cyctm()
{
	SendCommand("CYCTM");
	return true;
}

bool CSpectro::Cyctm(int cyctm)
{
	char buffer[3];
	string temp;

	itoa(cyctm, buffer, 10);

	temp += "CYCTM ";
	temp += buffer;

	SendCommand(temp);
	
	return true;
}

bool CSpectro::Cycles()
{
	SendCommand("CYCLES");
	return true;
}

bool CSpectro::Cycles(int cycles)
{
	char buffer[3];
	string temp;

	itoa(cycles, buffer, 10);

	temp += "CYCLES ";
	temp += buffer;

	SendCommand(temp);
	
	return true;
}

bool CSpectro::Interval()
{
	SendCommand("INTERVAL");
	return true;
}

bool CSpectro::Interval(int interval)
{
	char buffer[3];
	string temp;

	itoa(interval, buffer, 10);

	temp += "INTERVAL ";
	temp += buffer;

	SendCommand(temp);
	
	return true;
}

bool CSpectro::Lin()
{
	SendCommand("LIN");
	return true;
}

bool CSpectro::Lin(int lin)
{
	char buffer[3];
	string temp;

	itoa(lin, buffer, 10);

	temp += "LIN ";
	temp += buffer;

	SendCommand(temp);
	
	return true;
}

bool CSpectro::Strtwv()
{
	SendCommand("STRTWV");
	return true;
}

bool CSpectro::Strtwv(int strtwv)
{
	char buffer[3];
	string temp;

	itoa(strtwv, buffer, 10);

	temp += "STRTWV ";
	temp += buffer;

	SendCommand(temp);
	
	return true;
}

bool CSpectro::Stopwv()
{
	SendCommand("STOPWV");
	return true;
}

bool CSpectro::Stopwv(int stopwv)
{
	char buffer[3];
	string temp;

	itoa(stopwv, buffer, 10);

	temp += "STOPWV ";
	temp += buffer;

	SendCommand(temp);
	
	return true;
}

bool CSpectro::Speed()
{
	SendCommand("SPEED");
	return true;
}

bool CSpectro::Speed(int speed)
{
	char buffer[3];
	string temp;

	itoa(speed, buffer, 10);

	temp += "SPEED ";
	temp += buffer;

	SendCommand(temp);
	
	return true;
}

bool CSpectro::Wv(int number)
{
	char buffer[3];
	string temp;

	itoa(number, buffer, 10);

	temp += "WV";
	temp += buffer;

	SendCommand(temp);
	
	return true;
}

bool CSpectro::Wv(int number, int wv)
{
	char buffer[3];
	string temp;

	itoa(number, buffer, 10);

	temp += "WV";
	temp += buffer;
	temp += " ";

	itoa(wv, buffer, 10);

	temp += buffer;

	SendCommand(temp);
	
	return true;
}

bool CSpectro::Wvinc()
{
	SendCommand("WVINC");
	return true;
}

bool CSpectro::Wvinc(double wvinc)
{
	char buffer[3];
	string temp;

	_gcvt(wvinc, 10, buffer);

	temp += "WVINC ";
	temp += buffer;

	SendCommand(temp);
	return true;
}

bool CSpectro::Nmmin()
{
	SendCommand("NMMIN");
	return true;
}

bool CSpectro::Nmmin(int nmmin)
{
	char buffer[3];
	string temp;

	itoa(nmmin, buffer, 10);

	temp += "NMMIN ";
	temp += buffer;

	SendCommand(temp);
	return true;
}

// Utility Comands
bool CSpectro::Disp(std::string text)
{
	string temp;

	temp += "DISP ";
	temp += text;

	SendCommand(temp);
	return true;
}

bool CSpectro::Beep()
{
	SendCommand("BEEP");
	return true;
}

bool CSpectro::Beep(int nbbeep)
{
	char buffer[3];
	string temp;

	itoa(nbbeep, buffer, 10);

	temp += "BEEP ";
	temp += buffer;

	SendCommand(temp);
	
	return true;
}

bool CSpectro::Lock()
{
	SendCommand("LOCK");
	return true;
}

bool CSpectro::Unlock()
{
	SendCommand("UNLOCK");
	return true;
}

bool CSpectro::Run()
{
	SendCommand("RUN");
	return true;
}

bool CSpectro::Runs()
{
	SendCommand("RUN S");
	return true;
}

bool CSpectro::Timer(int time)
{
	char buffer[3];
	string temp;

	itoa(time, buffer, 10);

	temp += "TIMER ";
	temp += buffer;

	SendCommand(temp);
	
	return true;
}

bool CSpectro::Cat()
{
	SendCommand("CAT");
	return true;
}

bool CSpectro::List()
{
	SendCommand("LIST");
	return true;
}

bool CSpectro::Echo(std::string outputstr)
{
	string temp;

	temp += "ECHO ";
	temp += outputstr;

	SendCommand(temp);
	
	return true;
}


// Printer
bool CSpectro::Prt(bool onoff)
{
	if(onoff)
		SendCommand("PRT ON");
	else
		SendCommand("PRT OFF");
	return true;
}
