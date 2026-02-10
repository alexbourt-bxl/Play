#include "FileView.h"
#include "FileControl.h"

void FileView::files_OnRemoveControl(Container::Message& msg)
{
	Control* control = *((Control**)msg.Item);
	delete control;
}

void FileView::directory_OnContentsChange(::Message& msg)
{
	UpdateView();
}

void FileView::valThumbSize_OnValueChange(ValueMessage& msg)
{
	SuspendUpdates();

	for (int i = 0; i < m_files.Controls.Count; i++)
	{
		Control* control = m_files.Controls[i];
		
		if (typeof(control) != TYPEOF(FileControl))
			continue;

		FileControl* file = (FileControl*)control;
		file->ThumbSize = (int)m_valThumbSize.Value;
	}

	MarkForLayout();
	ResumeUpdates(true);
}

void FileView::settings_OnHideAnimEnd(Animation::Message& msg)
{
	m_settings.Hide();
}