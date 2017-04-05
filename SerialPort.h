// Auteur:		François Dickey and Etienne Tremblay
// Date:		22/01/2001
// Description:	Communication classe to communicate with ComPorts
//				Use the windows API.
//////////////////////////////////////////////////////////////////////

#ifndef __SERIALPORT_H__
#define __SERIALPORT_H__

#pragma warning(disable:4786)
#include <list>
#include <string>
#include <vector>

class CSerialPort
{
private:

	static UINT ReadThread(LPVOID pParam);


	//Comm Functions
	bool Terminate();                 //The destructor use it to destroy everything
    bool Open(int ComPortNb);         //Init use it to open the ComPort
	bool Close();                     //Terminate use it to kill the handle
	bool ReadChar(char* InputData);	  //Only ReadString will use this one to improve synchronisation's stability

	//Configuration functions
    bool GetConfig(COMMCONFIG& config);
	bool SetConfig(COMMCONFIG& Config);
    bool GetCommState(DCB& dcb);
	bool SetCommState(DCB& dcb);
	bool Setup(DWORD inqueue, DWORD outqueue);

	//Logging functions
    static bool CreateLogFile(const std::string path);
	static bool WriteByteLogFile(const char InData);
    static bool CreateByteLogFile(const std::string path);
	


protected:
	
    DCB PortDcb;								// Used to setup our communication port
	DCB PortOldDcb;								// Used to restore initial settings	

	//The handle is initially invalid so the destructor don't twart the ComPort with an invalid DCB
	static HANDLE ComPortHandle;	// HANDLE of the ComPort
	//Pour logger ce que a faite CSerialPort et ses functions
	static bool LogOnOff;
	static HANDLE LogFile;
	//Pour logger les valeurs que recoit CSerialPort
	//via ReadFile
	static bool ByteLevelLogging;
	static HANDLE LogByteFile;
	OVERLAPPED o;
	static HWND WinHandler;
	HANDLE ThreadHandle;
	CWinThread pReadThread;
	long ThreadId;
	static bool Destruction;
	static CRITICAL_SECTION cs;
	static bool ModeDataOn;
	static std::string InputBuffer;
	static std::list< std::string > DataQueue;
	static CRITICAL_SECTION cs2;
	static bool synching;

public:
// Destructor / Constructor
	CSerialPort();
	virtual ~CSerialPort();

// Comm Functions
	bool Init(HWND WinHandle,
				int ComPortNb = 2,
				int baudrate = CBR_4800, 
				int bytesize = 8,
				int parity = EVENPARITY,
				int stopbits = 0);

	bool SendString(const char* OutputData, int DataSize);    
	bool ReadString(char* InputString,int* Size);   //Get a whole CR terminated string from COM port
	bool SendAbort();
	std::string GetData();
	bool DataOn();
	bool DataOff();

// Queue buffer functions
	bool Flush();				                    // Flush the buffer
	bool ClearInBuffer();
	bool ClearOutBuffer();

// Event Methods
	bool SetMask(DWORD mask);
	bool GetMask(DWORD* mask);
	bool WaitEvent(DWORD* mask);
	static bool WriteLogFile(const std::string output);

	BOOL const IsOpen() const {return ComPortHandle != INVALID_HANDLE_VALUE;}	// Return true if opened
};

#endif	// __SERIALPORT_H__