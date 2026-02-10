#ifndef FILE_VIEW_H
#define FILE_VIEW_H

#include <Controls\Layout\ScrollLayout\ScrollLayout.h>
#include <Controls\Layout\LineLayout\LineLayout.h>
#include <Controls\Valuebox\Valuebox.h>
#include <Files\Directory.h>
#include <Animation\Animation.h>

class FileView
	: public Layout
{
	DECLARE_TYPE;

	typedef Directory _Directory;

public:
	FileView();

	__declspec(property(get = GetDirectory))
	_Directory& Directory;
	_Directory& GetDirectory();

	void	ShowSettings();
	void	HideSettings();

private:
	ScrollLayout m_scrollFiles;
	LineLayout m_files;
	_Directory m_directory;

	void	UpdateView();

	void	files_OnRemoveControl(Container::Message& msg);
	void	directory_OnContentsChange(::Message& msg);

	//

	LineLayout	m_settings;

	Valuebox m_valThumbSize;
	void	valThumbSize_OnValueChange(ValueMessage& msg);

	void	settings_OnHideAnimEnd(Animation::Message& msg);
};

#endif