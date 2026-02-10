#include "File.h"
#include "Math.h"

#include <stdlib.h>
#include <wchar.h>
#include <shlwapi.h>
#include <shlobj.h>

File::File()
{
	m_hasNumber	= false;
	m_number	= 0;
	m_padding	= 0;
}

File::File(const File& file)
{
	m_directory = file.m_directory;
	m_prefix	= file.m_prefix;

	m_hasNumber = file.m_hasNumber;
	m_number	= file.m_number;
	m_padding	= file.m_padding;

	m_suffix	= file.m_suffix;
	m_extension = file.m_extension;
}

File::File(const String& filename)
{
	InitName(filename);
}

File::File(const String& directory, const String& prefix, uint number, int padding, const String& suffix, const String& extension)
{
	// A padding of -1 means there is no number
	// A padding of 0 means there is no padding as such

	m_directory = directory;

	if (m_directory[m_directory.Length - 1] == L'\\')
		m_directory[m_directory.Length - 1] = L'\0';

	m_prefix	= prefix;

	m_hasNumber = padding > -1;
	m_number	= m_hasNumber ? number : 0;
	m_padding	= m_hasNumber ? padding : 0;

	m_suffix	= suffix;
	m_extension = extension;
}

void File::InitName(const String& _filename)
{
	m_hasNumber	= false;
	m_number	= 0;
	m_padding	= 0;

	// The file name isn't turned into a Unicode file name
	// at this point because it's better to keep it as specified.

	String& filename = (String&)_filename;

	// Break down the file name

	// Check if the filename is a relative path

	if (::PathIsRelative(filename))
	{
		int fullPathLength = ::GetFullPathName(filename, 0, NULL, NULL);
		wchar_t* fullPath = (wchar_t*)calloc(fullPathLength + 1, sizeof(wchar_t));

		::GetFullPathName(filename, fullPathLength, fullPath, NULL); //&absoluteFileNameFilePart);
	
		filename = fullPath;
		free(fullPath);
	}

	// Check if the filename is a directory

	if (::PathIsDirectory(filename))
	{
		if (filename[filename.Length - 1] == L'\\')
			filename = filename.Left(filename.Length - 1);

		m_directory = filename;
		return;
	}

	// Otherwise break it into parts

	// Directory

	int endOfDir = filename.LastIndexOf(L'\\');

	if (endOfDir >= 0)
		m_directory = filename.Left(endOfDir);

	// Name

	Name = filename.Right(filename.Length - (endOfDir + 1));
}

File& File::operator = (const File& file)
{
	m_directory = file.m_directory;
	m_prefix	= file.m_prefix;

	m_hasNumber = file.m_hasNumber;
	m_number	= file.m_number;
	m_padding	= file.m_padding;

	m_suffix	= file.m_suffix;
	m_extension = file.m_extension;
	
	return *this;
}
File& File::operator = (const String& filename)
{
	InitName(filename);
	return *this;
}

String File::GetDirectory()
{
	return m_directory;
}

void File::SetDirectory(const String& directory)
{
	m_directory = directory;
}

String File::GetPrefix()
{
	return m_prefix;
}

void File::SetPrefix(const String& prefix)
{
	m_prefix = prefix;
}

bool File::GetHasNumber()
{
	return m_hasNumber;
}

uint File::GetNumber()
{
	return m_number;
}

void File::SetNumber(uint number)
{
	m_number = number;
}

int File::GetPadding()
{
	return m_padding;
}

void File::SetPadding(int padding)
{
	m_padding = padding;
}

String File::GetSuffix()
{
	return m_suffix;
}

void File::SetSuffix(const String& suffix)
{
	m_suffix = suffix;
}

String File::GetExtension()
{
	return m_extension;
}

void File::SetExtension(const String& extension)
{
	m_extension = extension;
}

String File::GetName()
{
	String name;

	name += m_prefix;
	name += GetNumberString();
	name += m_suffix;

	if (m_extension != L"")
	{
		name += L".";
		name += m_extension;
	}

	return name;
}

void File::SetName(const String& _name)
{
	m_prefix	= L"";

	m_hasNumber	= false;
	m_number	= 0;
	m_padding	= 0;

	m_suffix	= L"";
	m_extension = L"";

	String& name = (String&)_name;

	// Extension

	int nExt = name.LastIndexOf(L'.');

	if (nExt > -1)
	{
		nExt++;
		m_extension = name.Right(name.Length - nExt);
	}

	// Suffix

	// If the file has an extension, the suffix search starts from one character before;
	// if there is no extension, the suffix search starts from the last character.

	int suffixLength = 0;
	int nSuffix = m_extension != L""
		? nExt - 2
		: name.Length - 1;

	while (nSuffix >= 0
		&& !iswdigit(name[nSuffix]))
	{
		suffixLength++;
		nSuffix--;
	}

	if (suffixLength > 0)
	{ 
		nSuffix++;
		m_suffix = name.SubString(nSuffix, suffixLength);
	}

	// Number
	// 9-digit limit

	int numberLength = 0;
	int nNumber = m_suffix != L""
		? nSuffix - 1
		: nSuffix;

	while (nNumber >= 0
		&& numberLength < 9
		&& iswdigit(name[nNumber]))
	{
		numberLength++;
		nNumber--;
	}

	if (numberLength > 0)
	{
		nNumber++;
		m_hasNumber = true;

		m_number = wcstoul(name.SubString(nNumber, numberLength), null, 10);
		m_padding = numberLength;
	}

	// Prefix

	if (m_hasNumber)
	{
		m_prefix = name.Left(nNumber);
	}
	else
	{
		m_prefix = m_suffix;
		m_suffix = L"";
	}
}

String File::GetPath()
{
	String path;
	String name = Name;

	path += m_directory;
	if (m_directory != L"" && name != L"")
		path += L"\\";
	path += name;

	return path;
}

bool File::GetIsValidDirectory()
{
	return ::PathIsDirectory(Path) != FALSE;
}

bool File::GetExists()
{
	return ::PathFileExists(Path) != FALSE;
}

Object* File::Load()
{
	//Mouse::SetCursor(Cursor::Wait);
	Object* obj = Handler.LoadFile(Path);
	//Mouse::SetCursor(Cursor::Normal);

	return obj;
}

bool File::Save(Object* obj)
{
	//Mouse::SetCursor(Cursor::Wait);
	bool result = Handler.SaveFile(obj, Path);
	//Mouse::SetCursor(Cursor::Normal);

	return result;
}

//Object* File::Load()
//{
//	OPENFILENAME ofn;
//
//	wchar_t filename[32000]; 
//	filename[0] = NULL;
//
//	ofn.lStructSize			= sizeof(OPENFILENAME);
//	ofn.hwndOwner			= NULL;
//	ofn.hInstance			= NULL;
//	ofn.lpstrFilter			= File::s_fileHandler.GetFormatFilter();
//	ofn.lpstrCustomFilter	= NULL;
//	ofn.nMaxCustFilter		= 0;
//	ofn.nFilterIndex		= File::s_fileHandler.Formats.Count + 1;
//	ofn.lpstrFile			= filename;
//	ofn.nMaxFile			= sizeof(filename) / sizeof(wchar_t);
//	ofn.lpstrFileTitle		= NULL;
//	ofn.nMaxFileTitle		= NULL;
//	ofn.lpstrInitialDir		= NULL;
//	ofn.lpstrTitle			= L"Open file";
//	ofn.Flags				= OFN_ENABLESIZING | OFN_EXPLORER /*| OFN_ALLOWMULTISELECT*/ | OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;
//	ofn.nFileOffset			= 0;
//	ofn.nFileExtension		= NULL;
//	ofn.lpstrDefExt			= L"";
//	ofn.lCustData			= 0;
//	ofn.lpfnHook			= NULL;
//	ofn.lpTemplateName		= NULL;
//
//	if (::GetOpenFileName(&ofn) != 0)
//		return Load(filename);
//
//	return null;
//}

//bool File::Save(Object* obj)
//{
//	OPENFILENAME ofn;
//
//	wchar_t filename[32000]; 
//	filename[0] = NULL;
//
//	ofn.lStructSize			= sizeof(OPENFILENAME);
//	ofn.hwndOwner			= NULL;
//	ofn.hInstance			= NULL;
//	ofn.lpstrFilter			= File::s_fileHandler.GetFormatFilter();
//	ofn.lpstrCustomFilter	= NULL;
//	ofn.nMaxCustFilter		= 0;
//	ofn.nFilterIndex		= File::s_fileHandler.Formats.Count + 1;
//	ofn.lpstrFile			= filename;
//	ofn.nMaxFile			= sizeof(filename) / sizeof(wchar_t);
//	ofn.lpstrFileTitle		= NULL;
//	ofn.nMaxFileTitle		= NULL;
//	ofn.lpstrInitialDir		= NULL;
//	ofn.lpstrTitle			= L"Save file";
//	ofn.Flags				= OFN_ENABLESIZING | OFN_EXPLORER | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
//	ofn.nFileOffset			= 0;
//	ofn.nFileExtension		= NULL;
//	ofn.lpstrDefExt			= L"jpg"; // TODO: need a generic way to pick a default format for different object types
//	ofn.lCustData			= 0;
//	ofn.lpfnHook			= NULL;
//	ofn.lpTemplateName		= NULL;
//
//	if (GetSaveFileName(&ofn) != 0)
//		return Save(obj, filename);
//
//	return false;
//}

String File::Browse()
{
	OPENFILENAME ofn;

	wchar_t filename[32000]; 
	filename[0] = NULL;

	wchar_t* filter = GetHandler().CreateFormatFilter();

	ofn.lStructSize			= sizeof(OPENFILENAME);
	ofn.hwndOwner			= null;
	ofn.hInstance			= null;
	ofn.lpstrFilter			= filter;
	ofn.lpstrCustomFilter	= null;
	ofn.nMaxCustFilter		= 0;
	ofn.nFilterIndex		= GetHandler().Formats.Count + 1;
	ofn.lpstrFile			= filename;
	ofn.nMaxFile			= sizeof(filename) / sizeof(wchar_t);
	ofn.lpstrFileTitle		= null;
	ofn.nMaxFileTitle		= null;
	ofn.lpstrInitialDir		= null;
	ofn.lpstrTitle			= L"Select file";
	ofn.Flags				= OFN_ENABLESIZING | OFN_EXPLORER /*| OFN_ALLOWMULTISELECT*/ | OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;
	ofn.nFileOffset			= 0;
	ofn.nFileExtension		= null;
	ofn.lpstrDefExt			= L"";
	ofn.lCustData			= 0;
	ofn.lpfnHook			= null;
	ofn.lpTemplateName		= null;

	if (::GetOpenFileName(&ofn) != 0)
	{
		free(filter);
		return (String)filename;
	}

	free(filter);
	return String::Empty;
}

String File::GetUnicodeFilePath(const String& filename)
{
	// This function makes no assumptions about the validity of the file path;
	// it only returns a correctly formatted Unicode path.

	String resolvedFilePath = ResolveShortcut(filename);
	String unicodeFilePath;
	
	if (resolvedFilePath.Length > 4 && resolvedFilePath == L"\\\\?\\") // filePath is already a proper Unicode path
	{
		unicodeFilePath = resolvedFilePath;
	}
	else if (resolvedFilePath.Length > 0 && resolvedFilePath[0] == L'\\')
	{
		if (resolvedFilePath.Length > 1 && resolvedFilePath[1] == L'\\') // filePath begins with "\\" and is a network share
		{
			// Remove the first '\' and prepend "\\?\UNC"

			String tempFilePath;

			tempFilePath += L"\\\\?\\UNC";
			tempFilePath += resolvedFilePath.Right(resolvedFilePath.Length - 1);

			DWORD _unicodeFilePathSize = ::GetFullPathName(tempFilePath, 0, NULL, NULL);
			wchar_t* _unicodeFilePath = (wchar_t*)calloc(_unicodeFilePathSize, sizeof(wchar_t));

			::GetFullPathName(tempFilePath, _unicodeFilePathSize, _unicodeFilePath, NULL);
			unicodeFilePath = _unicodeFilePath;

			free(_unicodeFilePath);
		}
		else // filePath begins with "\" and is a relative path
		{
			DWORD _unicodeFilePathSize = ::GetFullPathName(resolvedFilePath, 0, NULL, NULL);
			wchar_t* _unicodeFilePath = (wchar_t*)calloc(_unicodeFilePathSize, sizeof(wchar_t));

			::GetFullPathName(filename, _unicodeFilePathSize, _unicodeFilePath, NULL);
			unicodeFilePath = _unicodeFilePath;

			free(_unicodeFilePath);
		}
	}
	else // filePath is a local path
	{
		// Prepend "\\?\"

		String tempFilePath;

		tempFilePath += L"\\\\?\\";
		tempFilePath += resolvedFilePath;

		DWORD _unicodeFilePathSize = ::GetFullPathName(tempFilePath, 0, NULL, NULL);
		wchar_t* _unicodeFilePath = (wchar_t*)calloc(_unicodeFilePathSize, sizeof(wchar_t));

		::GetFullPathName(tempFilePath, _unicodeFilePathSize, _unicodeFilePath, NULL);
		unicodeFilePath = _unicodeFilePath;

		free(_unicodeFilePath);
	}

	return unicodeFilePath;
}

String File::ResolveShortcut(const String& shortcut)
{
	if (File(shortcut).Extension != L"lnk")
		return shortcut;

	String resolvedFilePath;
	wchar_t wcPath[MAX_PATH]; 

	IShellLink* shellLink = NULL; 
	IPersistFile* persistFile = NULL; 

	HRESULT hresult; 
	WIN32_FIND_DATA wfd;

	hresult = CoInitialize(NULL);
	
	if (SUCCEEDED(hresult)) 
	{ 
		hresult = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*)&shellLink); 
		
		if (SUCCEEDED(hresult)) 
		{ 
			hresult = shellLink->QueryInterface(IID_IPersistFile, (void**)&persistFile); 

			if (SUCCEEDED(hresult))
			{ 
				hresult = persistFile->Load(shortcut, STGM_READ); 
			   
				if (SUCCEEDED(hresult))
				{ 
					hresult = shellLink->Resolve(NULL, 0); 

					if (SUCCEEDED(hresult))
					{ 
						hresult = shellLink->GetPath(
							wcPath, 
							sizeof(wcPath) / sizeof(wchar_t), 
							(WIN32_FIND_DATA*)&wfd, 
							SLGP_UNCPRIORITY);

						if (SUCCEEDED(hresult))
						{
							resolvedFilePath = wcPath;
						}
					} 
				} 

				persistFile->Release(); 
			} 

			shellLink->Release(); 
		}

		CoUninitialize();
	}

	return resolvedFilePath;
}

String File::GetNumberString()
{
	if (!m_hasNumber)
		return String::Empty;

	int actualPadding = max((signed)Math::GetDigitCount(m_number), m_padding);	
	int nPaddingDigits = Math::GetDigitCount(actualPadding);

	wchar_t* wcNumFormat = (wchar_t*)calloc(3 + nPaddingDigits + 4 + 1, sizeof(wchar_t));
	swprintf_s(wcNumFormat, 4 + nPaddingDigits + 3 + 1, L"%%0%iu", actualPadding);

	wchar_t* wcNum = (wchar_t*)calloc(actualPadding + 1, sizeof(wchar_t));
	swprintf_s(wcNum, actualPadding + 1, wcNumFormat, m_number);

	free(wcNumFormat);

	String strNum = wcNum;
	free(wcNum);

	return strNum;
}

FileHandler& File::GetHandler()
{
	static FileHandler handler;
	return handler;
}