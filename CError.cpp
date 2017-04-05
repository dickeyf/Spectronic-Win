//////////////////////////////////////////////////////////////////////
// Auteur:		François Dickey and Etienne Tremblay
// Date:		29/01/2001
// Description:	Class to handle error messages.  Can send windows
//				messages or user defined messages.  Designed to be
//				used with try/throw/catch system.
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CError.h"

bool CError::LogOnOff = false;
HANDLE CError::LogFile = INVALID_HANDLE_VALUE;

void ThrowError()
{
	CError* error = new CError;
	string temp = error->GetErrorMsg();
	::MessageBox(NULL, temp.c_str(), "Erreur", MB_OK|MB_APPLMODAL|MB_ICONERROR);
	delete error;
}

void ThrowError(DWORD code)
{
	CError* error = new CError(code);
	string temp = error->GetErrorMsg();
	::MessageBox(NULL, temp.c_str(), "Erreur", MB_OK|MB_APPLMODAL|MB_ICONERROR);
	delete error;
}

void ThrowError(string msg)
{
	CError* error = new CError(msg);
	string temp = error->GetErrorMsg();
	::MessageBox(NULL, temp.c_str(), "Erreur", MB_OK|MB_APPLMODAL|MB_ICONERROR);
	delete error;
}

void ThrowError(const CError erreur)
{
	::MessageBox(NULL, erreur.GetErrorMsg().c_str(), "Erreur", MB_OK|MB_APPLMODAL|MB_ICONERROR);
}

CError::CError()
{
	Error = ::GetLastError();
	if(LogOnOff)
	{
		if(LogFile == INVALID_HANDLE_VALUE)
			CreateLogFile("log.txt");
		string temp = GetErrorMsg();
		WriteLogFile(temp);
	}
	strError.erase(strError.begin(), strError.end());
}

CError::CError(const DWORD error)
{
	Error = error;
	if(LogOnOff)
	{
		if(LogFile == INVALID_HANDLE_VALUE)
			CreateLogFile("log.txt");
		string temp = GetErrorMsg();
		WriteLogFile(temp);
	}
	strError.erase(strError.begin(), strError.end());
}

CError::CError(const string error)
{
	strError.erase(strError.begin(), strError.end());
	strError = error;
	if(LogOnOff)
	{
		if(LogFile == INVALID_HANDLE_VALUE)
			CreateLogFile("log.txt");
		string temp = error;
		temp += "\r\n";
		WriteLogFile(temp);
	}
}

CError::CError(const CError& copy)
{
	Error = copy.Error;
	strError = copy.strError;
	LogOnOff = copy.LogOnOff;
	LogFile = copy.LogFile;
}

CError::~CError()
{
	CloseHandle(LogFile);
	LogFile = INVALID_HANDLE_VALUE;
}

const string CError::GetErrorMsg() const
{
	if(strError.empty())
	{
		char* temp;
		BOOL sucess = FormatMessage(	FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
										NULL, Error, MAKELANGID(LANG_NEUTRAL, SUBLANG_SYS_DEFAULT),
										(LPTSTR)&temp, 0, NULL);
		if(sucess == FALSE)
		{
			string temp2;
			temp2 = '\0';
			return temp2;
		}
		else
		{
			string temp2;
			temp2.assign(temp);
			LocalFree(temp);
			return temp2;
		}
	}
	else
	{
		return strError;
	}
}

void CError::SetErrorMsg(const string msg)
{
	strError.erase(strError.begin(), strError.end());
	strError = msg;
	if(LogOnOff)
	{
		string temp = msg;
		temp += "\r\n";
		WriteLogFile(temp);
	}
}

void CError::SetErrorMsg()
{
	Error = GetLastError();
	if(LogOnOff)
	{
		string temp = GetErrorMsg();
		WriteLogFile(temp);
	}
	strError.erase(strError.begin(), strError.end());
}

void CError::SetErrorMsg(DWORD error)
{
	Error = error;
	if(LogOnOff)
	{
		string temp = GetErrorMsg();
		WriteLogFile(temp);
	}
	strError.erase(strError.begin(), strError.end());
}

CError& CError::operator=(const CError& copy)
{
	Error = copy.Error;
	strError = copy.strError;
	return(*this);
}

bool CError::SetLogFile(bool onoff)
{
	LogOnOff = onoff;
	return true;
}

bool CError::CreateLogFile(const string path)
{
	LogFile = CreateFile(	path.c_str(),
							GENERIC_WRITE,
							FILE_SHARE_READ,
							NULL,
							OPEN_ALWAYS,
							FILE_ATTRIBUTE_NORMAL,
							NULL);
	return true;
}

bool CError::WriteLogFile(const string output)
{
	DWORD bytewritten;
	if(LogFile != INVALID_HANDLE_VALUE)
	{
		SetFilePointer(LogFile, NULL, NULL, FILE_END);
		WriteFile(LogFile, output.c_str(), output.size(), &bytewritten, NULL);
		return true;
	}
	else
		return false;
}
