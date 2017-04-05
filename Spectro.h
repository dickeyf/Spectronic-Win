// Spectro.h: interface for the CSpectro class.
//
//////////////////////////////////////////////////////////////////////

#include "SerialPort.h"

#if !defined(AFX_SPECTRO_H__E24C4588_F55D_11D4_A1ED_0090000873F3__INCLUDED_)
#define AFX_SPECTRO_H__E24C4588_F55D_11D4_A1ED_0090000873F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSpectro  
{
private:
	
// Constructor
	CSpectro();

// The communication device
	CSerialPort ComPort;
	static CSpectro* OnlyInstance;

// All the configuration of the Spectronic 1201
	int Mode;		// Absorbance Mode
	int DataFormat;	// Data Format
	bool Printer;	// If the printer is on/off
	int StrtWv;		// The start wavelength
	int StopWv;		// The stop wavelength
	int IncWv;		// The incrementation of the wavelength
	int NmSpeed;	// Monochromator speed
	

protected:
// Commands sent to the machine

	// Data mode commands
	bool Abs();					// Set absorbance mode
	bool Abs(int absmode);		// Set absorbance mode absmode(-3 to 3)
	bool Trans();				// Set transmitance mode
	bool Conc();				// Set concentration mode
	bool Conc(int concmode);	// Set concentration mode and calculate factor to
								// make concentration value = concmode
	
	// Data normalization commands
	bool Autoz();				// Set the absolute zero
	bool Factor();
	bool Factor(int factor);
	bool Cmult();
	bool Cmult(int cmult);
	bool Czero();
	bool Czero(int czero);
	bool Lolim();
	bool Lolim(int lolim);
	bool Hilim();
	bool Hilim(int hilim);
	bool Sigav();
	bool Sigav(int sigav);

	// Data output commands
	bool ComDataOnOff(bool onoff);
	bool Data();
	bool Format(int dataformat);		// Set unlabeled data format

	// Monochromator control commands
	bool Goto(int wavelength);			// Set wavelength
	bool Comp();
	bool Redwv();
	bool Redwv(int redwv);

	// Lamp Control commands
	bool Lmpch();
	bool Lmpch(int lmpch);

	// Test parameter commands
	bool Test(int testnb);
	bool Seqid();
	bool Seqid(int seqid);
	bool Name();
	bool Name(std::string name);
	bool Dlytm();
	bool Dlytm(int delaytime);	// Set Initial delay time
	bool Cyctm();
	bool Cyctm(int cyctm);
	bool Cycles();
	bool Cycles(int cycles);
	bool Interval();
	bool Interval(int interval);
	bool Lin();
	bool Lin(int lin);
	bool Strtwv();
	bool Strtwv(int strtwv);	// Set the start wavelength
	bool Stopwv();
	bool Stopwv(int stopwv);	// Set the stop wavelength
	bool Speed();
	bool Speed(int speed);
	bool Wv(int number);
	bool Wv(int number, int wv);
	bool Wvinc();
	bool Wvinc(double wvinc);
	bool Nmmin();
	bool Nmmin(int nmmin);

	// Utility Comands
	bool Disp(std::string text);
	bool Beep();
	bool Beep(int nbbeep);
	bool Lock();
	bool Unlock();
	bool Run();
	bool Runs();
	bool Timer(int time);
	bool Cat();
	bool List();
	bool Echo(std::string outputstr);	// Send a string to the screen of the Spectro

	// Printer
	bool Prt(bool onoff);					// Turn on/off the printer

// CSpectro utility functions	

public:
// Destructor
	virtual ~CSpectro();
	bool Reset();               // Reset the Spectronic and make sure it's ready to process datas

// Interface functions
	bool Init(HWND WinHandle,
				int ComPortNb = 2,
				int baudrate = CBR_4800, 
				int bytesize = 8,
				int parity = NOPARITY,
				int stopbits = ONESTOPBIT);// Initialyse the Spectronic 1201
	int GetWvAbs(int wavelength) const;	// Return the absorbance at the specified wavelength
	bool SendCommand(const std::string command);
	BOOL const IsReady(void) {return ComPort.IsOpen();}	// Return true if the machine is ready
	bool ScanInterval(int StartWv,int StopWv, double Interval, int MCSpeed);
	void ScanDone();

	static CSpectro* GetInstance();
	static void DestroyInstance() {delete OnlyInstance;}
};

#endif // !defined(AFX_SPECTRO_H__E24C4588_F55D_11D4_A1ED_0090000873F3__INCLUDED_)
