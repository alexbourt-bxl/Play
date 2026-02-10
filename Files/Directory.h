#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <Types/Basic\String.h>
#include <Containers/Array.h>
#include <Files\File.h>
#include <Events/Event.h>

class Directory
	: public EventSender
{
public:
	Directory();
	Directory(const String& path);
	Directory(const Directory& dir);

	__declspec(property(get = GetPath, put = SetPath))
	String	Path;
	String	GetPath();
	void	SetPath(const String& path);

	__declspec(property(get = GetFiles))
	Array<File>& Files;
	Array<File>& GetFiles(/*bool getDirs = false*/);

	Event<::Message&> ContentsChangeEvent;

private:
	String m_path;

	/*	If the directory files are requested,
		the Directory class retrieves the files
		and stores it in an array for access. 
		If the directory contents change, the list 
		is automatically updated (if it exists). */
	Array<File> m_files;
	bool m_filesRetrieved;
};

#endif