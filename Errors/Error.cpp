#include "Error.h"

#include <Types/BaseTypes.h>
#include <System\App.h>

#include <windows.h>
#include <stdio.h>

void Error::ShowLast()
{
	LPVOID lpMsgBuf;

	if (!::FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		null,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0,
		null))
	{
		::MessageBox(null, L"Error", null, MB_OK | MB_ICONERROR); // Cannot display error
	}
	else
	{
		::MessageBox(null, (LPCTSTR)lpMsgBuf, null, MB_OK | MB_ICONERROR);
	}
	
	::LocalFree(lpMsgBuf);
}

String Error::GetCrashLogFile()
{
	return App::GetDataDir() + L"\\Crash.log";
}
