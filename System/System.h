#ifndef SYSTEM_H
#define SYSTEM_H

#include <Types/BaseTypes.h>

#include <locale.h>

class System
{
public:
	static uint	GetTickCount();
	static void	Beep();

	static _locale_t Locale;

	static bool	HasMMX();
	static bool	HasSSE();
	static bool	HasSSE2();
};

#endif