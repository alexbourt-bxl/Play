#include "System.h"

#include <windows.h>

_locale_t System::Locale = _create_locale(LC_ALL, "");

uint System::GetTickCount()
{
	return (uint)::GetTickCount();
}

void System::Beep()
{
	::MessageBeep(MB_OK);
}

bool System::HasMMX()
{
	static int s_mmx = -1;

	if (s_mmx < 0)
	{
		s_mmx = 0;

		__asm
		{
			pushad

			mov		eax, 1
			cpuid

			shr		edx, 24 // test for bit 23
			jnc		_no_mmx
			mov		s_mmx, 1

_no_mmx:	popad
		}
	}

	return s_mmx > 0;
}

bool System::HasSSE()
{
	static int s_sse = -1;

	if (s_sse < 0)
	{
		s_sse = 0;

		__asm 
		{
			pushad

			mov		eax, 1
			cpuid

			shr		edx, 26	// test for bit 25
			jnc		_no_sse
			mov		s_sse, 1

_no_sse:	popad
		}
	}

	return s_sse > 0;
}

bool System::HasSSE2()
{
	static int s_sse2 = -1;

	if (s_sse2 < 0)
	{
		s_sse2 = 0;

		__asm 
		{
			pushad

			mov		eax, 1
			cpuid

			shr		edx, 27	// test for bit 26
			jnc		_no_sse2
			mov		s_sse2, 1

_no_sse2:	popad
		}
	}

	return s_sse2 > 0;
}