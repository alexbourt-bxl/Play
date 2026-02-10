#ifndef FILE_FORMAT_TEXT_H
#define FILE_FORMAT_TEXT_H

#include <Files\Formats\FileFormat.h>

#include <stdio.h>

class FileFormat_Text
	: public FileFormat
{
public:
	FileFormat_Text();

	Type	GetObjectType() const;

	Object*	LoadFile(const String& filename, void* params);
	bool	SaveFile(Object* obj, const String& filename, void* params);
};

#endif