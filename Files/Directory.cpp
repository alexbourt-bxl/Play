#include "Directory.h"

#include <windows.h>
#include <shlwapi.h>

Directory::Directory()
{
	ContentsChangeEvent.Sender = this;
	m_filesRetrieved = false;
}

Directory::Directory(const String& path)
{
	ContentsChangeEvent.Sender = this;
	m_filesRetrieved = false;
	m_path = path;
}

Directory::Directory(const Directory& dir)
{
	ContentsChangeEvent.Sender = this;
	m_filesRetrieved = false;
	m_path = dir.m_path;
}

String Directory::GetPath()
{
	return m_path;
}

void Directory::SetPath(const String& path)
{
	if (m_path == path)
		return;

	if (m_filesRetrieved)
	{
		m_files.Clear();
		m_filesRetrieved = false;
	}

	m_path = path;

	::Message msg;
	ContentsChangeEvent.Send(msg);
}

Array<File>& Directory::GetFiles(/*bool getDirs*/)
{
	if (!::PathFileExists(m_path))
	{
		if (m_filesRetrieved)
		{
			m_files.Clear();
			m_filesRetrieved = false;
		}

		return m_files;
	}

	if (m_filesRetrieved)
		return m_files;

	m_files.Clear();

	String findMask = m_path + L"\\*.*";

	HANDLE hf;
	WIN32_FIND_DATA fd;

	int nLSuccess = 1;
	hf = ::FindFirstFile(findMask, &fd);

	if (hf != INVALID_HANDLE_VALUE)
	{
		while (nLSuccess != 0)
		{
			//if (getDirs || (!getDirs && wcsrchr(fd.cFileName, L'.') != 0))
			//{
				File file(m_path + L"\\" + fd.cFileName);

				if ((fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) == FILE_ATTRIBUTE_HIDDEN)
				{
					nLSuccess = (int)::FindNextFile(hf, &fd);
					continue;
				}

				//if (getDirs)
				//{
					if (file.Name == L"." || file.Name == L"..")
					{
						nLSuccess = (int)::FindNextFile(hf, &fd);
						continue;
					}

					//if (!file.IsValidDirectory)
					//{
					//	nLSuccess = (int)::FindNextFile(hf, &fd);
					//	continue;
					//}

					if (!File::Handler.IsValidExtension(file.Extension))
					{
						nLSuccess = (int)::FindNextFile(hf, &fd);
						continue;
					}
				//}
				//else
				//{
					//if (!FileHandler::IsValidExtension(file.Extension))
					//	continue;
				//}

				m_files.Add(file);
			//}

			nLSuccess = (int)::FindNextFile(hf, &fd);
		}
	}

	::FindClose(hf);

	//FileHandler::StringQuickSort(dirFiles, fileCount);

	/*	TODO: 
		m_files.Sort(comparisonProc); */

	m_filesRetrieved = true;
	return m_files;
}

//void StartDirWatchThread()
//{
//	if (!wmsg.bAlive)
//	{
//		wmsg.bKill = false;
//		dirWatchThread = (HANDLE)_beginthreadex(0, 0, &WatchDirectory, 0, 0, &dirWatchThreadID);
//		SetThreadPriority(dirWatchThread, THREAD_PRIORITY_BELOW_NORMAL);
//
//		SYSTEM_INFO si;
//		GetSystemInfo(&si);
//
//		SetThreadAffinityMask(thumbThread, min(2, si.dwNumberOfProcessors));
//	}
//}
//
//unsigned __stdcall WatchDirectory(void*)
//{
//	while (!wmsg.bKill && nCurrentDirIcons)
//	{
//		wmsg.bAlive = true;
//
//		DWORD dwWaitStatus; 
//		HANDLE dwChangeHandle; 
//
//		dwChangeHandle = FindFirstChangeNotification(currentDir, false, FILE_NOTIFY_CHANGE_FILE_NAME  |
//																		FILE_NOTIFY_CHANGE_DIR_NAME   |
//																		FILE_NOTIFY_CHANGE_ATTRIBUTES |
//																		FILE_NOTIFY_CHANGE_SECURITY   |
//																		FILE_NOTIFY_CHANGE_LAST_WRITE |
//																		FILE_NOTIFY_CHANGE_SIZE); 
//	 
//		if (dwChangeHandle == INVALID_HANDLE_VALUE) 
//			ExitProcess(GetLastError()); 
//	 
//		// Change notification is set. Now wait on the 
//		// handle and refresh accordingly. 
//	 
//		bool b = true;
//
//		while (b) 
//		{ 
//			// Wait for notification.
//
//			dwWaitStatus = WaitForSingleObject(dwChangeHandle, INFINITE); 
//
//			switch (dwWaitStatus) 
//			{ 
//			case WAIT_OBJECT_0: 
//				{
//					HANDLE hDir = CreateFile(currentDir,
//											FILE_LIST_DIRECTORY,
//											FILE_SHARE_READ | FILE_SHARE_DELETE,
//											NULL,
//											OPEN_EXISTING,
//											FILE_FLAG_BACKUP_SEMANTICS,
//											NULL);
//
//					FILE_NOTIFY_INFORMATION fni[1024];
//					DWORD nBytesReturned = 0;
//
//					ReadDirectoryChangesW(hDir,
//										 &fni,
//										 sizeof(fni),
//										 false,
//											FILE_NOTIFY_CHANGE_FILE_NAME  |
//											FILE_NOTIFY_CHANGE_DIR_NAME   |
//											FILE_NOTIFY_CHANGE_ATTRIBUTES |
//											FILE_NOTIFY_CHANGE_SIZE		  |
//											FILE_NOTIFY_CHANGE_LAST_WRITE,
//										 &nBytesReturned,
//										 NULL,
//										 NULL);
//
//					wchar_t *fileName = (wchar_t*)calloc(fni->FileNameLength + 1, sizeof(wchar_t));
//					wcsncpy(fileName, fni->FileName, fni->FileNameLength / sizeof(wchar_t));
//
//					if (wcscmp(fileName, L"thumbs.1") != 0)
//						LoadCurrentDirFiles(currentDir);
//
//					free(fileName);
//
//					CloseHandle(hDir);
//
//					if (FindNextChangeNotification(dwChangeHandle) == FALSE)
//						ExitProcess(GetLastError()); 
//					
//					break;
//				}
//
//			default: 
//				ExitProcess(GetLastError()); 
//			}
//		}
//
//		//Sleep(1000);
//	}
//
//	wmsg.bAlive = false;
//	_endthreadex(0);
//	CloseHandle(dirWatchThread);
//
//	return 0;
//}