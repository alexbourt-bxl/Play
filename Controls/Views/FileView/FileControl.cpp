#include "FileControl.h"

#include <Types/Graphics\GdiImage8.h>

DEFINE_TYPE(FileControl, LineLayout);

FileControl::FileControl()
{
	SuspendUpdates();

	m_image.FitImage = true;

	m_lblName.Dock = Control::DockFillHorizontal;
	m_lblName.TextAlign = Align::Center | Align::Middle;
	m_lblName.TextFont = Font(Font::Default.Name, 18.0, Font::Normal);
	m_lblName.Height = 22;
	//m_lblName.BackColor = Color(Color::Orange, 0.2);

	m_thumbSize = 100;

	AutoSize = true;
	Orientation = Vertical;
	Controls.Add(&m_image);
	Controls.Add(&m_lblName);

	ResumeUpdates();
}

File& FileControl::GetFile()
{
	return m_file;
}

void FileControl::SetFile(const _File& file)
{
	m_file = file;
	UpdateControl();
}

int FileControl::GetThumbSize()
{
	return m_thumbSize;
}

void FileControl::SetThumbSize(int thumbSize)
{
	m_thumbSize = thumbSize;
	
	m_image.SetSize(m_thumbSize, m_thumbSize);
}

void FileControl::UpdateControl()
{
	SuspendUpdates();

	/*	Set the file name. */
	m_lblName.Text = m_file.Name;

	Object* obj = m_file.Load();

	if (!obj)
	{
		ResumeUpdates(true);
		return;
	}

	/*	If the object is an image, create a thumbnail.
		Otherwise draw a file icon. */

	if (typeof(obj) == TYPEOF(Image))
	{
		Image* img = (Image*)obj;
		
		double scale = (double)img->Width > (double)img->Height
			? (double)m_thumbSize / (double)img->Width
			: (double)m_thumbSize / (double)img->Height;

		_Size size(
			(int)((double)img->Width * scale),
			(int)((double)img->Height * scale));

		GdiImage8* thumb = new GdiImage8(m_thumbSize, m_thumbSize);

		_Size _size(
			min(size.Width, img->Width * 2),
			min(size.Height, img->Height * 2));

		Graphics g(thumb);

		g.Interpolation = size.Width > _size.Width && size.Height > _size.Height
			? Graphics::Nearest
			: Graphics::Bilinear;
		
		g.DrawImage(
			(thumb->Width - _size.Width) / 2,
			(thumb->Height - _size.Height) / 2,
			_size.Width,
			_size.Height,
			img,
			0, 0,
			img->Width,
			img->Height);

		m_image.SetImage(thumb, true);
		m_image.SetSize(m_thumbSize, m_thumbSize);
	}
	else
	{
		// TODO
	}

	delete obj;

	ResumeUpdates(true);
}