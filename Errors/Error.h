#ifndef ERROR_H
#define ERROR_H

#include <Types/Basic\String.h>

class Error
{
public:
	static void		ShowLast();
	static String GetCrashLogFile();
};

#endif