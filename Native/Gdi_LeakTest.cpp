#include "Gdi.h"

#include <stdio.h>

Gdi::LeakTest::LeakTest(const String& /*funcName*/)
{
	//GetLock().Lock();

	//m_objectCount = Gdi::GetObjectCount();
	//m_funcName = funcName;
}

Gdi::LeakTest::~LeakTest()
{
	//uint leaks = Gdi::GetObjectCount() - m_objectCount;
	//
	//if (leaks > 0)
	//{
	//	wchar_t wc[512];
	//	swprintf_s(wc, 512, 
	//		L"%i GDI leak%s in %s\n", 
	//		leaks, 
	//		leaks == 1 ? L"" : L"s", 
	//		(wchar_t*)m_funcName);
	//	OutputDebugString(wc);

	//	DebugBreak();
	//}

	//GetLock().Unlock();
}

ThreadLock& Gdi::LeakTest::GetLock()
{
	static ThreadLock lock;
	return lock;
}