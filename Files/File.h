#ifndef FILE_H
#define FILE_H

/*
	TODO: write and format description

	Files can have numbers
	frame names
	directory\prefix[number]suffix.extension

	Numbers and padding are limited to 9 digits (uint). If the number is made up of more 
	than 9 digits, then anything preceding the last 9 digits goes into the prefix.
*/

#include "FileHandler.h"

#define Handler GetHandler()

class File
{
public:
	File();
	File(const File& file);
	File(const String& filename);
	File(const String& directory, const String& prefix, uint number, int padding, const String& suffix, const String& extension); // Padding == 0 -> hasNumber = false
	
	void	InitName(const String& filename);

	File&	operator = (const File& file);
	File&	operator = (const String& filename);

	__declspec(property(get = GetDirectory, put = SetDirectory))
	String	Directory;
	String	GetDirectory();
	void	SetDirectory(const String& directory);

	__declspec(property(get = GetPrefix, put = SetPrefix))
	String	Prefix;
	String	GetPrefix();
	void	SetPrefix(const String& prefix);

	__declspec(property(get = GetHasNumber))
	bool	HasNumber;
	bool	GetHasNumber();

	__declspec(property(get = GetNumber, put = SetNumber))
	uint	Number;
	uint	GetNumber();
	void	SetNumber(uint number);

	__declspec(property(get = GetPadding, put = SetPadding))
	int		Padding;
	int		GetPadding();
	void	SetPadding(int padding);
	
	__declspec(property(get = GetSuffix, put = SetSuffix))
	String	Suffix;
	String	GetSuffix();
	void	SetSuffix(const String& suffix);
	
	__declspec(property(get = GetExtension, put = SetExtension))
	String	Extension;
	String	GetExtension();
	void	SetExtension(const String& extension);

	__declspec(property(get = GetName, put = SetName))
	String	Name;
	String	GetName();
	void	SetName(const String& name);

	__declspec(property(get = GetPath))
	String	Path;
	String	GetPath();

	__declspec(property(get = GetIsValidDirectory))
	bool	IsValidDirectory;
	bool	GetIsValidDirectory();

	__declspec(property(get = GetExists))
	bool	Exists;
	bool	GetExists();
	
	Object*	Load();
	bool	Save(Object* obj);

	//static Object* Load(); // opens a file dialog
	//static bool	   Save(Object* obj); // opens a file dialog

	static String Browse();

	static String GetUnicodeFilePath(const String& filename);
	static String ResolveShortcut(const String& shortcut);

	static FileHandler& GetHandler();

private:
	String	m_directory;
	String	m_prefix;

	bool	m_hasNumber;
	uint	m_number;
	int		m_padding;

	String	GetNumberString();

	String	m_suffix;
	String	m_extension;
};

#endif