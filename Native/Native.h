#ifndef NATIVE_H
#define NATIVE_H

#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);

void ProcessCommandLineArgs(wchar_t** argvw, int argsw, wchar_t** filename);
int ProcessMessages();

void LoadLibraries();
void UnloadLibraries();

LONG UnhandledExceptionHandler(LPEXCEPTION_POINTERS ep);

#endif