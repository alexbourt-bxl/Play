#include "Resource.h"

#include <Types/Graphics\GdiImage8.h>

#include <stdio.h>

Image* Resource::LoadImage(WORD id)
{
	return new GdiImage8(LoadBitmap(id));
}

HBITMAP Resource::LoadBitmap(WORD id)
{
	return (HBITMAP)::LoadImage(
		HINST_PLAY, //g_hInstance, 
		MAKEINTRESOURCE(id), 
		IMAGE_BITMAP, 
		0, 
		0, 
		LR_DEFAULTCOLOR);
}

HCURSOR Resource::LoadCursor(WORD id)
{
	return (HCURSOR)::LoadImage(
		HINST_PLAY,
		MAKEINTRESOURCE(id),
		IMAGE_CURSOR,
		0,
		0,
		LR_DEFAULTCOLOR);
}

HCURSOR Resource::LoadAnimatedCursor(WORD id)
{
	HRSRC hRes = ::FindResource(
		HINST_PLAY, 
		MAKEINTRESOURCE(id),
		L"ANICURSORS");

	DWORD size = ::SizeofResource(HINST_PLAY, hRes);

	HGLOBAL hGlob = ::LoadResource(HINST_PLAY, hRes);
	byte* bytes = (byte*)::LockResource(hGlob);

	return (HCURSOR)::CreateIconFromResource(bytes, size, FALSE, 0x00030000);
}

String Resource::LoadString(const String& resourceName)
{
	wchar_t appFile[512];
	
	if (!GetModuleFileName(null, appFile, sizeof(appFile) / sizeof(wchar_t)))
		return String();

	DWORD versionSize = GetFileVersionInfoSize(appFile, null);

	if (versionSize == 0)
		return String();

	void* versionData = malloc(versionSize);

	if (!GetFileVersionInfo(appFile, null, versionSize, versionData))
	{
		free(versionData);
		return String();
	}

	struct LANGANDCODEPAGE
	{
		WORD language;
		WORD codePage;
	} 
	*translate;

	UINT translateSize = 0;

	if (!VerQueryValue(versionData, L"\\VarFileInfo\\Translation", (void**)&translate, &translateSize))
	{
		free(versionData);
		return String();
	}

	wchar_t verStringName[128];
	swprintf_s(
		verStringName, sizeof(verStringName) / sizeof(wchar_t),
		L"\\StringFileInfo\\%04x%04x\\%s", 
		translate->language, 
		translate->codePage, 
		resourceName);

	wchar_t* resourceString = null;
	UINT resourceStringSize = 0;

	if (!VerQueryValue(versionData, verStringName, (void**)&resourceString, &resourceStringSize))
	{
		free(versionData);
		return String();
	}

	String result = resourceString;
	free(versionData);

	return result;
}

String Resource::CreateVersionString()
{
	wchar_t appFile[512];

	if (GetModuleFileName(null, appFile, sizeof(appFile) / sizeof(wchar_t)))
		return String();

	DWORD versionSize = GetFileVersionInfoSize(appFile, null);

	if (versionSize == 0)
		return String();

	void* versionData = malloc(versionSize);

	if (!GetFileVersionInfo(appFile, null, versionSize, versionData))
	{
		free(versionData);
		return String();
	}
	
	VS_FIXEDFILEINFO* vsffi;
	UINT nBytes = 0;

	if (!VerQueryValue(versionData, L"\\", (void**)&vsffi, &nBytes))
	{
		free(versionData);
		return String();
	}

	wchar_t appVersion[32];
	wchar_t versionPart[8];

	//swprintf_s(appVersion, sizeof(appVersion) / sizeof(wchar_t), L"%u.%u.%u", HIWORD(vsffi->dwFileVersionMS), LOWORD(vsffi->dwFileVersionMS), HIWORD(vsffi->dwFileVersionLS));
	
	swprintf_s(versionPart, sizeof(versionPart) / sizeof(wchar_t), L"%u", HIWORD(vsffi->dwFileVersionMS));
	wcscpy_s(appVersion, sizeof(appVersion) / sizeof(wchar_t), versionPart);

	//if (LOWORD(vsffi->dwFileVersionMS) != 0)
	//{
		swprintf_s(versionPart, sizeof(versionPart) / sizeof(wchar_t), L".%u", LOWORD(vsffi->dwFileVersionMS));
		wcscat_s(appVersion, sizeof(appVersion) / sizeof(wchar_t), versionPart);
	//}

	//if (HIWORD(vsffi->dwFileVersionLS) != 0)
	//{
		swprintf_s(versionPart, sizeof(versionPart) / sizeof(wchar_t), L".%u", HIWORD(vsffi->dwFileVersionLS));
		wcscat_s(appVersion, sizeof(appVersion) / sizeof(wchar_t), versionPart);
	//}

	if (LOWORD(vsffi->dwFileVersionLS) != 0)
	{
		swprintf_s(versionPart, sizeof(versionPart) / sizeof(wchar_t), L".%u", LOWORD(vsffi->dwFileVersionLS));
		wcscat_s(appVersion, sizeof(appVersion) / sizeof(wchar_t), versionPart);
	}

	String result = appVersion;
	free(versionData);

	return result;
}