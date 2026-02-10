#include "FileFormat.h"

#include <windows.h>
#include <stdio.h>

FileFormat::FileFormat()
{

}

FileFormat::~FileFormat()
{

}

String& FileFormat::GetName()
{
	return m_name;
}

void FileFormat::SetName(const String& name)
{
	m_name = name;
}

Array<String>& FileFormat::GetExtensions()
{
	return m_extensions;
}