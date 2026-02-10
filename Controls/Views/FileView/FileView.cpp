#include "FileView.h"

#include <Controls\Views\FileView\FileControl.h>

DEFINE_TYPE(FileView, Layout);

FileView::FileView()
{
	*m_files.Controls.RemoveEvent += EventHandler(this, &FileView::files_OnRemoveControl);
	m_directory.ContentsChangeEvent += EventHandler(this, &FileView::directory_OnContentsChange);

	m_files.Orientation = LineLayout::Horizontal;
	m_files.AutoSize = true;
	m_files.Wrap = true;
	m_files.Dock = DockFillHorizontal;
	m_files.Padding = 20;
	m_files.Gap.X = 15;
	m_files.Gap.Y = 35;
	m_files.Align = Align::Left | Align::Top;
	m_files.Animate = true;

	m_scrollFiles.Dock = DockFill;
	m_scrollFiles.Align = Align::Left | Align::Top;
	m_scrollFiles.Controls.Add(&m_files);

	m_valThumbSize.ValueName = L"Thumbnail size";
	m_valThumbSize.MinValue = 32;
	m_valThumbSize.MinVisible = 0;
	m_valThumbSize.MaxValue = 256;
	m_valThumbSize.MaxVisible = 256;
	m_valThumbSize.Relative = true;
	m_valThumbSize.DecimalPlaces = 0;
	m_valThumbSize.AdjustSpeed = 0.5;
	m_valThumbSize.Suffix = L"px";
	m_valThumbSize.Value = 100;
	m_valThumbSize.ValueChangeEvent += EventHandler(this, &FileView::valThumbSize_OnValueChange);

	m_settings.BackColor = Color::Panel;
	m_settings.Dock = DockFill;
	m_settings.Align = Align::Center | Align::Middle;
	m_settings.Controls.Add(&m_valThumbSize);
	m_settings.Opacity = 0.0;
	m_settings.Hide();

	SuspendUpdates();

	Align = Align::Left | Align::Top;
	//Padding = 20;

	Controls.Add(&m_scrollFiles);
	Controls.Add(&m_settings);

	ResumeUpdates();
}

Directory& FileView::GetDirectory()
{
	return m_directory;
}

void FileView::ShowSettings()
{
	m_settings.Show();

	Animation::Suspend();

	FloatAnimation* anim = new FloatAnimation();
		anim->Property = m_settings.Opacity;
		anim->To = 1.0;
		anim->Speed = 4.0;

	Animation::Add(anim);
	Animation::Resume();
}

void FileView::HideSettings()
{
	Animation::Suspend();

	FloatAnimation* anim = new FloatAnimation();
		anim->Property = m_settings.Opacity;
		anim->To = 0.0;
		anim->Speed = 4.0;
		anim->EndEvent += EventHandler(this, &FileView::settings_OnHideAnimEnd);

	Animation::Add(anim);
	Animation::Resume();
}

void FileView::UpdateView()
{
	/*	Clear the current view. */

	m_files.Controls.Clear();

	/*	Get a list of supported files. */

	Array<File>& files = m_directory.Files;

	/*	For each file create an ImageControl
		with a the file's thumbnail. */

	for (int i = 0; i < files.Count; i++)
	{
		FileControl* fileControl = new FileControl();
		fileControl->ThumbSize = (int)m_valThumbSize.Value;
		fileControl->File = files[i];
		m_files.Controls.Add(fileControl);
	}

	UpdateLayout();
}