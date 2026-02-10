#ifndef FILE_FORMAT_JPEG_H
#define FILE_FORMAT_JPEG_H

#include <Files\Formats\FileFormat.h>

#include <stdio.h>

extern "C" {
#define XMD_H // this is for libjpeg
#include <jpeglib.h>
}

class FileFormat_Jpeg 
	: public FileFormat
{
public:
	struct Params
	{
		int quality;
	};

public:
	FileFormat_Jpeg();

	Type	GetObjectType() const;

	Object*	LoadFile(const String& filename, void* params);
	bool	SaveFile(Object* obj, const String& filename, void* params);
};

#endif