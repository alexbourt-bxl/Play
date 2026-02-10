#ifndef FILE_FORMAT_PNG_H
#define FILE_FORMAT_PNG_H

#include <Files\Formats\FileFormat.h>

extern "C" {
#include <png.h>
}

class FileFormat_Png 
	: public FileFormat
{
public:
	FileFormat_Png();

	Type	GetObjectType() const;

	Object*	LoadFile(const String& filename, void* params);
	bool	SaveFile(Object* obj, const String& filename, void* params);
};

#endif