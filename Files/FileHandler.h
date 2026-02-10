#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <Files\Formats\FileFormat.h>

#include <Containers/Array.h>

class FileHandler
{
public:
	FileHandler();

	Object*			LoadFile(const String& filename);
	bool			SaveFile(Object*, const String& filename);

	bool			IsValidExtension(const String& extension);

	//Array<String>	GetDirectoryFiles(const String& directory, bool getDirs);
	//String		GetNextFileInDirectory(const String& currentFile);
	//String		GetPrevFileInDirectory(const String& currentFile);

	//wchar_t*		GetFormatFilter();
	wchar_t*		CreateFormatFilter();

	Array<FileFormat*> Formats;

private:
	void			StringQuickSort(wchar_t** sa, int count);
	void			sqs(wchar_t** sa, int left, int right);
	inline wchar_t	tlower(wchar_t b);
	inline wchar_t	isnum(wchar_t b);
	inline int		parsenum(wchar_t* &a);
	inline int		sc(wchar_t* a, wchar_t* b);

	//wchar_t*		m_formatFilter;
};

#endif