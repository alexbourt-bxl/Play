#ifndef GDI_H
#define GDI_H

#include <Types/Basic\String.h>
#include <Types/Geometric\Region.h>
#include <Threads\ThreadLock.h>

#include <windows.h>

class Gdi
{
public:
	static HRGN	ToHRGN(Region& rgn);
	
	static void	InitBitmapHeader(
		BITMAPINFOHEADER* bi, 
		int width, 
		int height);

	static void CopyToBuffer(int* buffer, HDC hdc, HBITMAP hbitmap);
	static void CopyFromBuffer(HDC hdc, HBITMAP hbitmap, int* buffer);

	static uint GetObjectCount();

public:
	class LeakTest;
};

//

class Gdi::LeakTest
{
public:
	explicit LeakTest(const String& funcName);
	~LeakTest();

private:
	uint m_objectCount;
	String m_funcName;

	/*	A static lock is used to avoid GDI 
		count pollution between threads. */
	static ThreadLock& GetLock();
};

#endif