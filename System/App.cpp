#include "App.h"

#include <Native\Resource.h>

#include <windows.h>
#include <shlobj.h>

String App::GetDataDir()
{
	wchar_t appDataDir[MAX_PATH];

	String appName = Resource::LoadString(L"ProductName");

	::SHGetFolderPathAndSubDir(
		null,
		CSIDL_APPDATA | CSIDL_FLAG_CREATE,
		null,
		SHGFP_TYPE_CURRENT,
		appName,
		appDataDir);

	return String(appDataDir);
}