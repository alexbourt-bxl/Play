#include "Tablet.h"

HMODULE	Tablet::s_hWintab = null;

WTINFO			Tablet::dll_WTInfo			= null;
WTOPEN			Tablet::dll_WTOpen			= null;
WTCLOSE			Tablet::dll_WTClose			= null;
WTPACKETSGET	Tablet::dll_WTPacketsGet	= null;
WTPACKET		Tablet::dll_WTPacket		= null;
WTENABLE		Tablet::dll_WTEnable		= null;
WTOVERLAP		Tablet::dll_WTOverlap		= null;
WTGET			Tablet::dll_WTGet			= null;
WTSET			Tablet::dll_WTSet			= null;
WTQUEUESIZESET	Tablet::dll_WTQueueSizeSet	= null;
WTEXTGET		Tablet::dll_WTExtGet		= null;

bool Tablet::LoadWintab()
{
	s_hWintab = ::LoadLibrary(L"Wintab32.dll");

	if (!s_hWintab)
		return false;

	dll_WTInfo			= (WTINFO)::GetProcAddress(s_hWintab, (LPCSTR)1020);
	dll_WTOpen			= (WTOPEN)::GetProcAddress(s_hWintab, (LPCSTR)1021);
	dll_WTClose			= (WTCLOSE)::GetProcAddress(s_hWintab, (LPCSTR)22);
	dll_WTPacketsGet	= (WTPACKETSGET)::GetProcAddress(s_hWintab, (LPCSTR)23);
	dll_WTPacket		= (WTPACKET)::GetProcAddress(s_hWintab, (LPCSTR)24);
	dll_WTEnable		= (WTENABLE)::GetProcAddress(s_hWintab, (LPCSTR)40);
	dll_WTOverlap		= (WTOVERLAP)::GetProcAddress(s_hWintab, (LPCSTR)41);
	dll_WTGet			= (WTGET)::GetProcAddress(s_hWintab, (LPCSTR)1061);
	dll_WTSet			= (WTSET)::GetProcAddress(s_hWintab, (LPCSTR)1062);
	dll_WTQueueSizeSet	= (WTQUEUESIZESET)::GetProcAddress(s_hWintab, (LPCSTR)85);
	dll_WTExtGet		= (WTEXTGET)::GetProcAddress(s_hWintab, (LPCSTR)63);

	return true;
}

void Tablet::UnloadWintab()
{
	if (s_hWintab)
		::FreeLibrary(s_hWintab);
}