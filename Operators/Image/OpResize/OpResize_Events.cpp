#include "OpResize.h"

#include <Types/Graphics\Image.h>
#include <Controls\Valuebox\Valuebox.h>

void OpResize::in_OnConnect(Input::Message& msg)
{
	if (m_init)
		return;

	Image* image = (Image*)msg.Input->GetInputObject();

	if (!image)
		return;

	Width.DisableEvents();
	Width.Valuebox->DisableEvents();
	Height.DisableEvents();
	Height.Valuebox->DisableEvents();

	Width = image->Width;
	Width.Valuebox->Value = Width;
	Width.Valuebox->MaxVisible = Width * 2;

	Height = image->Height;
	Height.Valuebox->Value = Height;
	Height.Valuebox->MaxVisible = Height * 2;

	Width.EnableEvents();
	Width.Valuebox->EnableEvents();
	Height.EnableEvents();
	Height.Valuebox->EnableEvents();

	m_refSize = FSize(
		(double)Width,
		(double)Height);

	/*	TODO: each input should send in the event args a variable
		that indicates whether the connected output is new. This way
		the op can decide whether to reinitialize. */

	m_init = true;
}

void OpResize::btnProportional_OnPress(Control::Message& /*msg*/)
{
	/*	The control events must be disabled here
		to avoid an endless loop of updates between
		the control and the property. */

	m_btnProportional.DisableEvents();
	m_btnProportional.Highlight = !m_btnProportional.Highlight;
	m_btnProportional.EnableEvents();
}

void OpResize::Width_Valuebox_PreValueChange(ValueMessage& msg)
{
	if (m_btnProportional.Highlight)
	{
		int newHeight = max((int)Height.Valuebox->MinValue, (int)(*msg.Value * m_refSize.Height / m_refSize.Width));

		Height.DisableEvents();
		Height.Valuebox->DisableEvents();

		Height = newHeight;
		Height.Valuebox->Value = newHeight;

		Height.EnableEvents();
		Height.Valuebox->EnableEvents();
	}
}

void OpResize::Width_Valuebox_OnValueChange(ValueMessage& /*msg*/)
{
	if (!m_btnProportional.Highlight)
	{
		m_refSize.Width = (double)Width;
		m_refSize.Height = (double)Height;
	}
}

void OpResize::Height_Valuebox_PreValueChange(ValueMessage& msg)
{
	if (m_btnProportional.Highlight)
	{
		int newWidth = max((int)Width.Valuebox->MinValue, (int)(*msg.Value * m_refSize.Width / m_refSize.Height));

		Width.DisableEvents();
		Width.Valuebox->DisableEvents();

		Width = newWidth;
		Width.Valuebox->Value = newWidth;

		Width.EnableEvents();
		Width.Valuebox->EnableEvents();
	}
}

void OpResize::Height_Valuebox_OnValueChange(ValueMessage& /*msg*/)
{
	if (!m_btnProportional.Highlight)
	{
		m_refSize.Width = (double)Width;
		m_refSize.Height = (double)Height;
	}
}

void OpResize::Interpolation_OnSelectItem(::Message& /*msg*/)
{
	switch (Interpolation.Button.SelectedIndex)
	{
	case 0:
		_interpolation = Graphics::Nearest;
		break;

	case 1:
		_interpolation = Graphics::Bilinear;
		break;

	//case 2:
	//	_interpolation = Graphics::Bicubic;
	//	break;
	}
}