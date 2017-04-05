//////////////////////////////////////////////////////////////////////
// Auteur:		François Dickey and Etienne Tremblay
// Date:		29/01/2001
// Description:	Class to handle error messages.  Can send windows
//				messages or user defined messages.  Designed to be
//				used with try/throw/catch system.
//////////////////////////////////////////////////////////////////////


#ifndef __CERROR__
#define __CERROR__

#include <string>
#include <vector>

using namespace std;

class CError
{
protected:
// Error code and error string
	DWORD Error;
	std::string strError;
	static bool First;

// Logfile variables
	static bool LogOnOff;
	static HANDLE LogFile;

private:
// LogFile write to file
	static bool WriteLogFile(string output);
	static bool CreateLogFile(const string path);

public:
// Constructor / Destructor
	CError();
	CError(const DWORD error);
	CError(const string error);
	CError(const CError& copy);
	virtual ~CError();

// Operator Overloading
	CError& operator=(const CError& copy);

// Errors Functions
	inline const string GetErrorMsg() const;
	void SetErrorMsg(const string msg);
	void SetErrorMsg();
	void SetErrorMsg(DWORD error);

// Set options for logfiles
	static bool SetLogFile(bool onoff);
};

void ThrowError();
void ThrowError(DWORD code);
void ThrowError(string msg);
void ThrowError(const CError erreur);

#endif