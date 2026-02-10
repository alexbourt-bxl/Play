#ifndef FILE_FORMAT_H
#define FILE_FORMAT_H

#include <Types/Object.h>
#include <Types/Basic\String.h>
#include <Containers/Array.h>

class FileFormat
{
public:
	FileFormat();
	virtual ~FileFormat();

	virtual Type	GetObjectType() const = 0;  // Indicates which object type the format supports

	virtual Object*	LoadFile(const String& filename, void* params) = 0;
	virtual bool	SaveFile(Object* obj, const String& filename, void* params) = 0;

	__declspec(property(get = GetName, put = SetName))
	String& Name;
	String&	GetName();
protected:
	void SetName(const String& name);

public:
	__declspec(property(get = GetExtensions))
	Array<String>& Extensions;
	Array<String>& GetExtensions();

private:
	String			m_name;
	Array<String>	m_extensions;
};

#endif