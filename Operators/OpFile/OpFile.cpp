#include "OpFile.h"

#include <Files\File.h>
#include <Types/Graphics\Image8.h>

DEFINE_TYPE(OpFile, Operator);
SET_TYPE_DISPLAY_NAME(OpFile, L"File");

OpFile::OpFile() 
{
	Init();
}

OpFile::OpFile(const OpFile& op) 
{
	m_file = op.m_file;

	Init();
}

OpFile::~OpFile()
{
	if (m_outObject)
		delete m_outObject;
}

String OpFile::GetDisplayName()
{
	return m_file.Name == L""
		? Name
		: m_file.Name;
}

void OpFile::Init()
{
	In.AcceptedTypes.Add(TYPEOF(Image8));
	In.Add(&m_in);

	Out.Add(&m_out);
	m_outObject = null;

	File.Name = L"File";
	File.ChangeEvent += EventHandler(this, &OpFile::File_OnChange);
	Properties.Add(&File);

	File = File::Browse();
}

Object* OpFile::Copy()
{
	return new OpFile(*this);
}

void OpFile::CopyWorkingValues()
{
	m_file = File;
}

void OpFile::Process()
{
	if (m_file.Name == L"")
		return;

	/*	If there is an input save the object. */

	Object* in = m_in.Data[Tag::Image];

	if (in && m_file.Save(in))
	{
		m_out.Data = m_in.Data; // works as cache
	}

	/*	Otherwise load the file. */

	else if (m_file.Exists)
	{
		if (m_outObject)
		{
			delete m_outObject;
			m_outObject = null;
		}

		m_outObject = m_file.Load();
		m_out.Data.Set(Tag::Image, m_outObject);
	}
}