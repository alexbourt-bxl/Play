#ifndef FILE_CONTROL_H
#define FILE_CONTROL_H

#include <Controls\Layout\LineLayout\LineLayout.h>

#include <Files\File.h>
#include <Controls\ImageControl\ImageControl.h>
#include <Controls\Label\Label.h>

class FileControl
	: public LineLayout
{
	DECLARE_TYPE;

	typedef File _File;

public:
	FileControl();

	__declspec(property(get = GetFile, put = SetFile))
	_File&	File;
	_File&	GetFile();
	void	SetFile(const _File& file);

	__declspec(property(get = GetThumbSize, put = SetThumbSize))
	int		ThumbSize;
	int		GetThumbSize();
	void	SetThumbSize(int thumbSize);

private:
	_File	m_file;

	ImageControl m_image;
	Label	m_lblName;

	int		m_thumbSize;

	void	UpdateControl();

	//

	void	OnPaint(Graphics::Message& msg);
};

#endif