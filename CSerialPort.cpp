//////////////////////////////////////////////////////////////////////
// Auteur:		François Dickey and Etienne Tremblay
// Date:		22/01/2001
// Description:	Communication class to communicate with ComPorts
//				Use the windows API.
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "CError.h"

	//Le handle est invalide au début pour pas que le destructeur n'envoie un invalide DCB.
	//Pour logger ce que a faite CSerialPort et ses functions
	bool CSerialPort::LogOnOff=true;
	HANDLE CSerialPort::LogFile=INVALID_HANDLE_VALUE;
	HANDLE CSerialPort::LogByteFile=INVALID_HANDLE_VALUE;
	//Pour logger les valeurs que recoit CSerialPort
	//via ReadFile
	bool CSerialPort::ByteLevelLogging=true;
	HWND CSerialPort::WinHandler=0;
	HANDLE CSerialPort::ComPortHandle=INVALID_HANDLE_VALUE;
	bool CSerialPort::Destruction=false;
	CRITICAL_SECTION CSerialPort::cs;
	CRITICAL_SECTION CSerialPort::cs2;
	bool CSerialPort::synching=false;
	bool CSerialPort::ModeDataOn;
	std::string CSerialPort::InputBuffer;
	std::list< std::string > CSerialPort::DataQueue;


// Constructeur
CSerialPort::CSerialPort()
{
    if (LogOnOff)
		CreateLogFile("ComPortLog.txt");

	if (ByteLevelLogging)
		CreateByteLogFile("ComPortByteLog.txt");
	ComPortHandle=INVALID_HANDLE_VALUE;
	
}

// Destructeur
CSerialPort::~CSerialPort()
{
	if (ComPortHandle!=INVALID_HANDLE_VALUE) 
		Terminate(); //Termine, seulement si init a ete executé

	if (ByteLevelLogging)
		CloseHandle(LogFile); //Détruire le handle
    if (LogOnOff)
		CloseHandle(LogByteFile); //Détruire le handle

	InitializeCriticalSection(&cs);
	EnterCriticalSection(&cs);
	Destruction=true;
	EnterCriticalSection(&cs);
	DeleteCriticalSection(&cs);
}

// Fonction représentant la thread qui reçoit les messages du Spectronic 1001 Plus
// Il envoit ensuite des message à SpectronicDlg pour que celui-ci mette à jour
// le graphique, les logs et les valeurs numériques dans la liste.
UINT CSerialPort::ReadThread(LPVOID pParam)
{
	char InputData;
	unsigned long wParam;
    unsigned long lParam;
	unsigned long ByteRead;

    while (!Destruction)
	{			
	  if (ReadFile(ComPortHandle,&InputData,1,&ByteRead,NULL))
	  {
	    if ((ByteRead==1)&&(InputData!=10))
		{
			if (ModeDataOn)
			{
				wParam=242256242;
				lParam=InputData;
				WriteByteLogFile(InputData);
				SendMessage(WinHandler,SP_CHAR,wParam,lParam);
			}
			else
			{
				if (InputData !=13)
					InputBuffer+=InputData;
				else
				{
					//DataQueue.push_back(InputBuffer);
					//Le buffer doit être vidé
					//à present
					InputBuffer="";
				}
			}
		}
	  }
	}
    LeaveCriticalSection(&cs);
	AfxEndThread(0);
	return 0;
}

//////////////////////////////////////////////////
////////////////Public Functions//////////////////
//////////////////////////////////////////////////

// Cette fonction initialise le com port pour être prêt à utilisation
bool CSerialPort::Init(HWND WinHandle,
						int ComPortNb,
						int baudrate /*= CBR_4800*/,
						int bytesize /*= 8*/,
						int parity /*= NOPARITY*/,
						int stopbits /*= ONESTOPBIT*/)
{		
	if(!Open(ComPortNb))
		return false;

	LPCOMMTIMEOUTS TimeOut = new COMMTIMEOUTS;

	WinHandler=WinHandle;

	GetCommState(PortDcb);

	PortOldDcb=PortDcb;
	PortDcb.BaudRate = baudrate;
	PortDcb.ByteSize = bytesize;
	PortDcb.Parity = parity;
	PortDcb.StopBits = stopbits;

	bool sucess=SetCommState(PortDcb);

    Setup(1024,512); //valeurs aproximatives

    TimeOut->ReadIntervalTimeout=0;
	TimeOut->ReadTotalTimeoutMultiplier=1;
	TimeOut->ReadTotalTimeoutConstant=1000; //TimeOut après 1000 ms
    TimeOut->WriteTotalTimeoutMultiplier=1;
    TimeOut->WriteTotalTimeoutConstant=1000;
	SetCommTimeouts(ComPortHandle,TimeOut);

	delete TimeOut;

	AfxBeginThread(ReadThread, pReadThread);

	return true;
}

bool CSerialPort::SendAbort()
{
	unsigned long ByteWritten;
	char OutputData=3; //Code ascii 3 est la valeur du control c

	if (!WriteFile(ComPortHandle,&OutputData,1,&ByteWritten,NULL))
		return false;
	
	if (ByteWritten!=1)
		return false;

	return true;
}

//SendString envoit une chaine de texte à travers le port serie
bool CSerialPort::SendString(const char* OutputData, int DataSize)
{
	unsigned long ByteWritten;
	std::string Logging="SendString:";

	if (!WriteFile(ComPortHandle,OutputData,DataSize,&ByteWritten,NULL))        
		return false;
	
	if (ByteWritten<DataSize)
		return false;

	Logging +=OutputData;
	Logging +=" Sent to comport\r\n";
    WriteLogFile(Logging);
	return true;
}

//ReadString attend qu'un string complet soit 
//transferé vers l'ordinateur pour ensuite
//le mettre dans une instance de la classe string
bool CSerialPort::ReadString(char* InputString,int* Size)
{
	char InputChar;
	std::string WorkString ="";	

	*Size=0;
   do
   {   
	   //In case somebody trip in the serial link...
	   if (!ReadChar(&InputChar))
		   return false;

	   //Si ce n'est pas un CR ou un LF, alors il doit faire partie du string
       if ((InputChar != '\r')&(InputChar != '\n'))
	   {
		   WorkString += InputChar;
		   MessageBox(NULL,(const char *)InputChar,"Spectronic 1201:Char Received",MB_OK);
		   *Size++;
	   }

   } while (InputChar!='\r');
   strcpy(InputString, WorkString.c_str());

 return true;
}

string CSerialPort::GetData()
{
	string temp;
	while (DataQueue.empty())
	{
		//Waste CPU
	}
	temp=DataQueue.front();
	DataQueue.pop_front();
	return temp;
}

bool CSerialPort::DataOff()
{
	if (!SendString("DATA OFF\r",sizeof("DATA OFF\r")))
		return false;
	InputBuffer="";
	DataQueue.erase(DataQueue.begin(),DataQueue.end());
	ModeDataOn=false;
	return true;
}

bool CSerialPort::DataOn()
{
	if (!SendString("DATA ON\r",sizeof("DATA ON\r")))
		return false;
	ModeDataOn=true;
	return true;
}

//Queue buffers functions
bool CSerialPort::Flush()
{
	if (!PurgeComm(ComPortHandle,PURGE_TXABORT|PURGE_RXABORT))
		return false;
	return true;
}

bool CSerialPort::ClearInBuffer()
{
	if (!PurgeComm(ComPortHandle,PURGE_RXABORT))
		return false;
	return true;
}

bool CSerialPort::ClearOutBuffer()
{
	if (!PurgeComm(ComPortHandle,PURGE_TXABORT))
		return false;
	return true;
}

//Event functions
bool CSerialPort::SetMask(DWORD mask)
{
	if (!SetCommMask(ComPortHandle,mask))
		return false;
	return true;
}

bool CSerialPort::GetMask(DWORD* mask)
{
	if (!GetCommMask(ComPortHandle,mask))
		return false;
	return true;
}

bool CSerialPort::WaitEvent(DWORD* mask)
{
	if (!WaitCommEvent(ComPortHandle,mask,&o))
		return false;
	return true;
}

//////////////////////////////////////////////////
////////////////Private Functions/////////////////
//////////////////////////////////////////////////

//Fait tout ce qui est nécésaire à l'arrêt de l'utilisation du comport
bool CSerialPort::Terminate()
{
	SetCommState(PortOldDcb);
	if(!Close())
		return false;
    return true;
}

// Ouvre un comport
bool CSerialPort::Open(int ComPortNb)
{
	std::string DeviceName = "COM";
	char number[3];
	itoa(ComPortNb, number, 10);
	DeviceName += number;
	ComPortHandle=CreateFile(DeviceName.c_str(),
		                     GENERIC_READ|GENERIC_WRITE,
							 NULL,
                             NULL,
							 OPEN_EXISTING,
							 FILE_ATTRIBUTE_NORMAL,
                             NULL);
    if (ComPortHandle==INVALID_HANDLE_VALUE)
	{
		ThrowError("Failed to open the Com Port: Invalid Handle Value");
		return false;
	}

    DeviceName += " was successfully open\r\n";
	WriteLogFile(DeviceName);
	return true;
}

bool CSerialPort::Close()
{	
	if (!CloseHandle(ComPortHandle))
	{
        WriteLogFile("Error while closing com port\n");
		return false;		
	}
	WriteLogFile("Com port successfully closed\r\n");
	return true;
}

//ReadChar sert à lire le prochain caractère qui attend sur la file de réception
bool CSerialPort::ReadChar(char* InputData)
{
	unsigned long ByteRead;
	if (!ReadFile(ComPortHandle,InputData,1,&ByteRead,NULL))
		return false;

	if (ByteRead!=1)
		return false;

    WriteByteLogFile(*InputData);

	return true;
}

//////////////////////////////////////////////////////////////////////
// Configuration functions
//////////////////////////////////////////////////////////////////////

// Retourne la configuration actuelle du comport
bool CSerialPort::GetConfig(COMMCONFIG& Config)
{
	DWORD configSize = sizeof(Config);
	if (!GetCommConfig(ComPortHandle,&Config,&configSize))
		return false;
	return true;
}

// Change la configuration actuelle du comport
bool CSerialPort::SetConfig(COMMCONFIG& Config)
{
	if (!SetCommConfig(ComPortHandle,&Config,sizeof(Config)))
		return false;
	return true;
}

bool CSerialPort::GetCommState(DCB& dcb)
{
	if (!::GetCommState(ComPortHandle,&dcb))
		return false;
	return true;
}

bool CSerialPort::SetCommState(DCB& dcb)
{
	if (!::SetCommState(ComPortHandle,&dcb))
		return false;
	return true;
}

// Change la grosseur des valeurs temporaires
bool CSerialPort::Setup(DWORD inqueue, DWORD outqueue)
{
	if (!SetupComm(ComPortHandle,inqueue,outqueue))
		return false;
	return true;
}

//Meme functions que dans CError.
//CSerialPort va tout noter d'interessant
//sur le port serie dans un fichier texte.
bool CSerialPort::CreateLogFile(const std::string path)
{
	LogFile = CreateFile(	path.c_str(),
							GENERIC_WRITE,
							FILE_SHARE_READ,
							NULL,
							OPEN_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							NULL);
	if (LogFile == INVALID_HANDLE_VALUE)
		return false;
	SetFilePointer(LogFile, NULL, NULL, FILE_END);
	return true;
}

bool CSerialPort::WriteLogFile(const std::string output)
{
	DWORD bytewritten;
	if (!WriteFile(LogFile, output.c_str(), output.size(), &bytewritten, NULL))
		return false;
	return true;
}

//CSerialPort note tout ce qui arrive vers l'ordi
//byte par byte dans une file separée pour que l'on
//soit capable d'analyser les resultats que va 
//fournir le spectrophotomètre durant nos tests.
bool CSerialPort::CreateByteLogFile(const std::string path)
{
	LogByteFile = CreateFile(	path.c_str(),
		     					GENERIC_WRITE,
								FILE_SHARE_READ,
								NULL,
								OPEN_ALWAYS,
								FILE_ATTRIBUTE_NORMAL,
								NULL);
	if (LogByteFile == INVALID_HANDLE_VALUE)
		return false;
	SetFilePointer(LogByteFile, NULL, NULL, FILE_END);
	return true;
}

bool CSerialPort::WriteByteLogFile(const char InData)
{
	DWORD bytewritten;
	std::string output="0x";
	char number[4];

    itoa(InData,number,16); //Log en hexadecimal
	output +=number;
	if ((InData !=0x0A)&(InData !=0x0D))
	{
	  output +='(';
	  output +=InData;
	  output +=")";
	}
    else
	{
		if (InData == 0x0A)
			output +="(Line Feed)";
		if (InData == 0x0D)
			output +="(Cariage return)";
	}
    output+="\r\n";
	
	if (!WriteFile(LogByteFile, output.c_str(), output.size(), &bytewritten, NULL))
		return false;

	return true;
}