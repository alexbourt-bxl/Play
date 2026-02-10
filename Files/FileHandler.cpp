#include "FileHandler.h"

#include <Files\File.h>
#include <Files\Formats\Image\FileFormat_Jpeg.h>
#include <Files\Formats\Image\FileFormat_Png.h>
#include <Files\Formats\Image\FileFormat_Tiff.h>
#include <Files\Formats\Data\FileFormat_Data.h>

#include <stdio.h>
#include <shlwapi.h>

FileHandler::FileHandler()
{
	// TODO: need dynamic extension adding mechanism,
	// maybe similar to type registration in Object.

	Formats.Add(new FileFormat_Jpeg());
	Formats.Add(new FileFormat_Png());
	Formats.Add(new FileFormat_Tiff());
	//Formats.Add(new FileFormat_Data());

	//m_formatFilter = CreateFormatFilter();
}

Object* FileHandler::LoadFile(const String& filename)
{
	if (!File(filename).Exists)
		return null;

	String ext = File(filename).Extension;
	ext = ext.ToLower();
	
	Object* obj = null;
	
	for (int i = 0; i < Formats.Count; i++)
	{
		/*	TODO: Possibly various formats will have to be loaded with .* extension. 
		This requires a way to forcibly link a file with a format during loading. */

		if ((Formats[i]->Extensions.Contains(ext)
			|| Formats[i]->Extensions.Contains(L"*"))
			&& (obj = Formats[i]->LoadFile(filename, null)) != null)
			break;

		/*	Continue here, because if two distinct file formats share the same 
		extension and the first one doesn't take, the next one might, and so on. */
	}

	return obj;
}

bool FileHandler::SaveFile(Object* obj, const String& filename)
{
	String ext = File(filename).Extension;

	FileFormat* format = null;

	for (int i = 0; i < Formats.Count; i++)
	{
		if ((Formats[i]->Extensions.Contains(ext) 
			|| Formats[i]->Extensions.Contains(L"*"))
			&& Formats[i]->GetObjectType() == typeof(obj))
		{
			format = Formats[i];
			break;
		}
	}

	return format
		? format->SaveFile(obj, filename, null)
		: null;
}

//wchar_t* FileHandler::GetFormatFilter()
//{
//	return m_formatFilter;
//}

wchar_t* FileHandler::CreateFormatFilter()
{
	if (Formats.Count == 0)
		return null;

	// Create the OpenFileName filter string

	wchar_t *wcResult = 0;
	int filterLength = 0;
	
	String strAllFormats = L"All Formats";

	// Calculate the filter length

	for (int i = 0; i < Formats.Count; i++)
	{
		filterLength += Formats[i]->Name.Length + 2; // "FormatName (

		for (int j = 0; j < Formats[i]->Extensions.Count; j++)
		{
			filterLength += 2 + Formats[i]->Extensions[j].Length; // *.extension

			if (j < Formats[i]->Extensions.Count - 1)
				filterLength++; // ;
		}

		filterLength += 2; // )\0

		for (int j = 0; j < Formats[i]->Extensions.Count; j++)
		{
			filterLength += 2 + Formats[i]->Extensions[j].Length; // *.extension

			if (j < Formats[i]->Extensions.Count - 1)
				filterLength++; // ;
		}

		filterLength++; // \0
	}

	filterLength += strAllFormats.Length; // All Formats
	filterLength++; // \0

	for (int i = 0; i < Formats.Count; i++)
	{
		for (int j = 0; j < Formats[i]->Extensions.Count; j++)
		{
			filterLength += 2 + Formats[i]->Extensions[j].Length; // *.extension;

			if (i < Formats.Count - 1 || j < Formats[i]->Extensions.Count - 1)
				filterLength++; // ;
		}
	}

	filterLength += 2; // \0\0

	// Create the filter string

	#define RESULT_AVAILABLE (filterLength - (pwcResult - wcResult))

	wcResult = (wchar_t*)calloc(filterLength, sizeof(wchar_t));

	wchar_t* pwcResult = wcResult;

	for (int i = 0; i < Formats.Count; i++)
	{
		wcsncpy_s(
			pwcResult, 
			RESULT_AVAILABLE, 
			Formats[i]->Name, 
			Formats[i]->Name.Length);
		pwcResult += Formats[i]->Name.Length;
		
		wcsncpy_s(
			pwcResult, 
			RESULT_AVAILABLE, 
			L" (", 
			2);
		pwcResult += 2;

		for (int j = 0; j < Formats[i]->Extensions.Count; j++)
		{
			wcsncpy_s(
				pwcResult, 
				RESULT_AVAILABLE, 
				L"*.", 
				2);
			pwcResult += 2;
			
			wcsncpy_s(
				pwcResult, 
				RESULT_AVAILABLE, 
				Formats[i]->Extensions[j], 
				Formats[i]->Extensions[j].Length);
			pwcResult += Formats[i]->Extensions[j].Length;

			if (j < Formats[i]->Extensions.Count - 1)
				*pwcResult++ = L';';
		}

		*pwcResult++ = L')';
		*pwcResult++ = L'\0';

		for (int j = 0; j < Formats[i]->Extensions.Count; j++)
		{
			wcsncpy_s(
				pwcResult, 
				RESULT_AVAILABLE, 
				L"*.", 
				2);
			pwcResult += 2;

			wcsncpy_s(
				pwcResult, 
				RESULT_AVAILABLE, 
				Formats[i]->Extensions[j], 
				Formats[i]->Extensions[j].Length);
			pwcResult += Formats[i]->Extensions[j].Length;

			if (j < Formats[i]->Extensions.Count - 1)
				*pwcResult++ = L';';
		}

		*pwcResult++ = L'\0';
	}

	// All Formats

	wcsncpy_s(
		pwcResult, 
		RESULT_AVAILABLE, 
		strAllFormats, 
		strAllFormats.Length);
	pwcResult += strAllFormats.Length;

	*pwcResult++ = L'\0';

	for (int i = 0; i < Formats.Count; i++)
	{
		for (int j = 0; j < Formats[i]->Extensions.Count; j++)
		{
			wcsncpy_s(
				pwcResult, 
				RESULT_AVAILABLE, 
				L"*.", 
				2);
			pwcResult += 2;

			wcsncpy_s(
				pwcResult, 
				RESULT_AVAILABLE, 
				Formats[i]->Extensions[j], 
				Formats[i]->Extensions[j].Length);
			pwcResult += Formats[i]->Extensions[j].Length;

			if (i < Formats.Count - 1 || j < Formats[i]->Extensions.Count - 1)
				*pwcResult++ = L';';
		}
	}
	
	*pwcResult++ = L'\0';
	*pwcResult++ = L'\0';

	#undef RESULT_AVAILABLE

	return wcResult;
}

bool FileHandler::IsValidExtension(const String& extension)
{
	for (int i = 0; i < Formats.Count; i++)
	{
		for (int j = 0; j < Formats[i]->Extensions.Count; j++)
		{
			String lowerExt = ((String&)extension).ToLower();
			String lowerFormatExt = Formats[i]->Extensions[j].ToLower();

			if (lowerExt == lowerFormatExt)
				return true;
		}
	}
	
	return false;
}

//Array<String> FileHandler::GetDirectoryFiles(const String& directory, bool getDirs)
//{
//	if (!::PathIsDirectory(directory))
//		return 0;
//
//	Array<String> files;
//
//	String findMask = directory + L"\\*.*";
//
//	HANDLE hf;
//	WIN32_FIND_DATA fd;
//
//	int fileCount = 0;
//
//	int nLSuccess = 1;
//	hf = ::FindFirstFile(findMask, &fd);
//
//	if (hf != INVALID_HANDLE_VALUE)
//	{
//		while (nLSuccess != 0)
//		{
//			if (getDirs || (!getDirs && wcsrchr(fd.cFileName, L'.') != 0))
//			{
//				wchar_t* szNoNameExtension = 0;
//				
//				if (wcsrchr(fd.cFileName, L'.'))
//					szNoNameExtension = wcsrchr(fd.cFileName, L'.') + 1;
//
//				int callocSize = (int)wcslen(directory) + (int)wcslen(fd.cFileName) + 2;
//				wchar_t* fullName = (wchar_t*)calloc(callocSize, sizeof(wchar_t));
//				wcscat_s(fullName, callocSize, directory);
//				wcscat_s(fullName, callocSize, L"\\");
//				wcscat_s(fullName, callocSize, fd.cFileName);
//
//				if ((fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) == 0 &&
//					(getDirs &&
//					 wcscmp(fd.cFileName, L".") != 0 &&
//					 wcscmp(fd.cFileName, L"..") != 0 &&
//					 ((szNoNameExtension == NULL &&
//					   PathIsDirectory(fullName) != FALSE) ||
//					 (szNoNameExtension != NULL &&
//					  wcslen(szNoNameExtension) > 0 &&
//					  IsValidExtension(szNoNameExtension))) ||
//                     (!getDirs &&
//					  szNoNameExtension != NULL &&
//					  wcslen(szNoNameExtension) > 0 &&
//					  IsValidExtension(szNoNameExtension)))
//					)
//				{
//					if (dirFiles == 0)
//					{
//						dirFiles = (wchar_t**)malloc(++fileCount * sizeof(wchar_t*));
//					}
//					else
//					{
//						dirFiles = (wchar_t**)realloc(dirFiles, ++fileCount * sizeof(wchar_t*));
//					}
//
//					dirFiles[fileCount - 1] = _wcsdup(fullName);
//				}
//
//				free(fullName);
//			}
//
//			nLSuccess = (int)FindNextFile(hf, &fd);
//		}
//	}
//
//	FindClose(hf);
//
//	free(findMask);
//
//	StringQuickSort(dirFiles, fileCount);
//
//	*nFiles = fileCount;
//
//	return dirFiles;
//}
//
//String FileHandler::GetNextFileInDirectory(wchar_t* currentFile)
//{
//	wchar_t** dirFiles = NULL;
//	int fileCount = 0;
//
//	const wchar_t* noPathFilename = wcsrchr(currentFile, L'\\') + 1;
//	int pathLength = (int)wcslen(currentFile) - (int)wcslen(noPathFilename) - 1;
//	wchar_t* noNamePath = (wchar_t*)calloc(pathLength + 1, sizeof(wchar_t));
//	wcsncpy_s(noNamePath, pathLength + 1, currentFile, pathLength);
//
//	dirFiles = GetDirectoryFiles(noNamePath, &fileCount, false);
//
//	free(noNamePath);
//
//	int currentFileIndex = -1;
//
//	for (int i = 0; i < fileCount; i++)
//	{
//		if (wcscmp(dirFiles[i], currentFile) == 0)
//		{
//			currentFileIndex = i;
//			break;
//		}
//	}
//
//	if (currentFileIndex == -1)
//		return NULL;
//
//	int nextFileIndex = currentFileIndex + 1;
//
//	if (nextFileIndex >= fileCount)
//		nextFileIndex = 0;
//
//	wchar_t* returnFile = _wcsdup(dirFiles[nextFileIndex]);
//
//	for (int i = 0; i < fileCount; i++)
//		free(dirFiles[i]);
//
//	free(dirFiles);
//
//	return returnFile;
//}
//
//String FileHandler::GetPrevFileInDirectory(const String& currentFile)
//{
//	wchar_t** dirFiles = NULL;
//	int fileCount = 0;
//
//	wchar_t* noPathFilename = wcsrchr(currentFile, L'\\') + 1;
//	int pathLength = (int)wcslen(currentFile) - (int)wcslen(noPathFilename) - 1;
//	wchar_t* noNamePath = (wchar_t*)calloc(pathLength + 1, sizeof(wchar_t));
//	wcsncpy_s(noNamePath, pathLength + 1, currentFile, pathLength);
//
//	dirFiles = GetDirectoryFiles(noNamePath, &fileCount, false);
//
//	free(noNamePath);
//
//	int currentFileIndex = -1;
//
//	for (int i = 0; i < fileCount; i++)
//	{
//		if (wcscmp(dirFiles[i], currentFile) == 0)
//		{
//			currentFileIndex = i;
//			break;
//		}
//	}
//
//	if (currentFileIndex == -1)
//		return NULL;
//
//	int prevFileIndex = currentFileIndex - 1;
//
//	if (prevFileIndex < 0)
//		prevFileIndex = fileCount - 1;
//
//	wchar_t* returnFile = _wcsdup(dirFiles[prevFileIndex]);
//
//	for (int i = 0; i < fileCount; i++)
//		free(dirFiles[i]);
//
//	free(dirFiles);
//	
//	return returnFile;
//}

void FileHandler::StringQuickSort(wchar_t** sa, int count)
{
	if (count > 0)
		sqs(sa, 0, count - 1);	
}

void FileHandler::sqs(wchar_t** sa, int left, int right)
{
	register int i = left; 
	register int j = right;
	wchar_t* x = sa[(left + right) / 2];
	wchar_t* y;

	do
	{
		while ((sc(sa[i], x) == -1) && (i < right)) i++;
		while ((sc(x, sa[j]) == -1) && (j > left)) j--;

		if (i <= j)
		{
			y = sa[i];
			sa[i] = sa[j];
			sa[j] = y;
			i++; j--;
		}
	}
	while (i < j);

	if (left < j)	sqs(sa, left, j);
	if (i < right)	sqs(sa, i, right);
}

inline wchar_t FileHandler::tlower(wchar_t b)
{
	if ((b >= 'A') && (b <= 'Z')) return b - 'A' + 'a';

	return b;
}

inline wchar_t FileHandler::isnum(wchar_t b)
{
	if ((b >= '0') && (b <= '9'))
		return 1;

	return 0;
}

inline int FileHandler::parsenum(wchar_t* &a)
{
	int result = *a++ - '0';

	while (isnum(*a))
	{
		result *= 10;
		result += *a++ - '0';
	}

	a--;

	return result;
}

inline int FileHandler::sc(wchar_t* a, wchar_t* b)
{
	if (a == b) return 0;

	if (a == NULL) return -1;
	if (b == NULL) return 1;

	while (*a && *b)
	{
		int a0, b0;	// Will contain either a number or a letter

		if (isnum(*a))	a0 = parsenum(a) + 256;
		else			a0 = tlower(*a);

		if (isnum(*b))	b0 = parsenum(b) + 256;
		else			b0 = tlower(*b);

		if (a0 < b0)	return -1;
		if (a0 > b0)	return 1;

		a++;
		b++;
	}

	if (*a) return 1;
	if (*b) return -1;

	return 0;
}
