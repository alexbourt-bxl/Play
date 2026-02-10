#ifndef CLIPBOARD_H
#define CLIPBOARD_H

#include <Types/Object.h>

class Window;

class Clipboard
{
private:
	class Data;

public:
	static Window*	GetOwnerWindow();
	static void		SetOwnerWindow(Window* window);

	static Object*	Get(Type objType);
	static bool		Set(Object* obj);

	static void		Update(); // updates the local clipboard to reflect the Windows clipboard

	static void		Clear();

private:
	static Data		s_object;
	static Window*	s_ownerWindow;
};

class Clipboard::Data
{
public:
	Data();
	~Data();

	operator bool();

	operator Object*();
	Data& operator = (Object* obj);

	void Clear();
	
private:
	Object* m_obj;
};

#endif