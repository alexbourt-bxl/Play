#include "FileFormat_Text.h"

#include <Types/Basic\String.h>

FileFormat_Text::FileFormat_Text()
{
	Name = L"Text";

	Extensions.Add(L"txt");
}

Type FileFormat_Text::GetObjectType() const
{
	return TYPEOF(String);
}

Object* FileFormat_Text::LoadFile(const String& filename, void* /*params*/)
{
	FILE* file;

	if ((_wfopen_s(&file, filename, L"r")) != 0)
		return null;

	fseek(file, 0, SEEK_END);
	int bufferSize = ftell(file);
	rewind(file);

	wchar_t* buffer = (wchar_t*)malloc(bufferSize);

	fgetws(
		buffer, 
		bufferSize / sizeof(wchar_t),
		file);

	fclose(file);

	String* text = new String(buffer);
	return null; // TODO: return text;
}

bool FileFormat_Text::SaveFile(Object* obj, const String& filename, void* params)
{
	assert(typeof(obj) == TYPEOF(String));
	String* text = (String*)obj;

	FILE* file;

	if ((_wfopen_s(&file, filename, L"w")) != 0)
		return false;

	fputws(
		(wchar_t*)text,
		file);

	fclose(file);
	
	return true;
}