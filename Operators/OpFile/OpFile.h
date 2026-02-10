#ifndef OP_FILE_H
#define OP_FILE_H

#include <Operators\Operator.h>
#include <Types/Basic\String.h>
#include <Properties\TextProperty\TextProperty.h>
#include <Files\File.h>

class OpFile
	: public Operator
{
	DECLARE_TYPE;

	typedef File _File;

public:
	OpFile();
	OpFile(const OpFile& opFile);
	~OpFile();

	String	GetDisplayName();

	void	Init();
	Object*	Copy();

	void	CopyWorkingValues();
	void	Process();

	/*	TODO: If a different file is selected and the output is connected to
		an operator who doesn't support the new file type, disconnect them.	*/

	TextProperty File;

	/*	TODO:

		Create a file watch thread that marks the op as dirty 
		and forces in to update any time the file is changed. */

	/*	TODO: 
	
		For loading, optionally check here if the file has been modified
		since being loaded, or if the requested type has changed. 
		
		There should be an option to auto-reload changed files
		(by marking the op as dirty and pushing an update).
		For saving, the inputs should speak for themselves. */

private:
	Input	m_in;

	Output	m_out;
	Object*	m_outObject;

	/*	Working values. */
	_File	m_file;

	//

	void	File_OnChange(Property::Message& msg);
};

#endif