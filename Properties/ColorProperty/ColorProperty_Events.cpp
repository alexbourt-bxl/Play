#include "ColorProperty.h"

#include <Controls\ColorFan\ColorFan.h>
#include <Controls\Window\Window.h>

void ColorProperty::OnChange(Property::Message& msg)
{
	m_button.DisableEvents();
	m_button.SetColor(*((Color*)msg.Value), false);
	m_button.EnableEvents();
}

void ColorProperty::val_OnValueChange(ValueMessage& /*msg*/)
{
	m_button.DisableEvents();
	m_fan.DisableEvents();
	
	Color color(
		m_valRed.Value,
		m_valGreen.Value,
		m_valBlue.Value);

	*this = color;

	m_button.EnableEvents();
	m_fan.EnableEvents();
}

void ColorProperty::fan_OnColorChange(::Message& /*msg*/)
{
	/*	The control events must be disabled here to 
		avoid an endless loop of updates between the 
		controls and the property. */

	m_button.DisableEvents();
	m_fan.DisableEvents();
	
	*this = m_fan.SelectedColor;

	m_button.EnableEvents();
	m_fan.EnableEvents();
}

void ColorProperty::button_OnColorChange(::Message& /*msg*/)
{
	m_button.DisableEvents();
	m_fan.DisableEvents();

	*this = m_button.Color;

	m_button.EnableEvents();
	m_fan.EnableEvents();
}

void ColorProperty::fan_OnLoseFocus(Control::Message& /*msg*/)
{
	m_button.DisableEvents();
	m_button.SetColor(m_button.Color, true);
	m_button.EnableEvents();
}

void ColorProperty::btnFan_OnPress(Control::Message& /*msg*/)
{
	m_fan.DisableEvents();
	m_fan.SelectedColor = m_button.Color;
	m_fan.EnableEvents();

	Point p = m_button.PaintRect.Position;

	m_fan.UpdateLayout();
	m_fan.Position = Point(
		p.X + (m_button.Width - m_fan.Width) / 2,
		m_button.Parent->PaintRect.Y - m_fan.Height);

	m_button.Window->ShowFloating(&m_fan);
}

void ColorProperty::btnSliders_OnPress(Control::Message& /*msg*/)
{
	m_valRed.DisableEvents();
	m_valGreen.DisableEvents();
	m_valBlue.DisableEvents();

	m_valRed.Value   = m_button.Color.R;
	m_valGreen.Value = m_button.Color.G;
	m_valBlue.Value  = m_button.Color.B;

	m_valRed.EnableEvents();
	m_valGreen.EnableEvents();
	m_valBlue.EnableEvents();

	Point p = m_button.PaintRect.Position;

	m_layoutRgb.UpdateLayout();
	m_layoutRgb.Position = Point(
		p.X + (m_button.Width - m_layoutRgb.Width) / 2,
		m_button.Parent->PaintRect.Y - m_layoutRgb.Height);

	m_button.Window->ShowFloating(&m_layoutRgb);
}

void ColorProperty::btnPicker_OnPress(Control::Message& )
{

}