#ifndef FILE_FORMAT_TIFF_H
#define FILE_FORMAT_TIFF_H

#include <Files\Formats\FileFormat.h>

extern "C" 
{
#include <tiffio.h>
}

class FileFormat_Tiff 
	: public FileFormat
{
public:
	FileFormat_Tiff();

	Type	GetObjectType() const;

	Object*	LoadFile(const String& filename, void* params);
	bool	SaveFile(Object* obj, const String& filename, void* params);
};

#endif