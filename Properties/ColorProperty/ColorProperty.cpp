#include "ColorProperty.h"

DEFINE_TYPE(ColorProperty, Property);

ColorProperty::ColorProperty()
{
	Init();
}

ColorProperty::ColorProperty(const ColorProperty& prop)
{
	m_value = prop.m_value;
	Init();
}

ColorProperty::~ColorProperty()
{

}

void ColorProperty::Init()
{
	m_fan.SelectedColor = m_value;

	m_button.DefaultToLast = true;
	m_button.SetColor(m_value);

	m_btnFan.SetIcon(&s_imgFan);
	m_btnSliders.SetIcon(&s_imgSliders);
	m_btnPicker.SetIcon(&s_imgPicker);

	m_button.Strip.Controls.Add(&m_btnFan);
	m_button.Strip.Controls.Add(&m_btnSliders);
	m_button.Strip.Controls.Add(&m_btnPicker);

	m_btnFan.PressEvent += EventHandler(this, &ColorProperty::btnFan_OnPress);
	m_btnSliders.PressEvent += EventHandler(this, &ColorProperty::btnSliders_OnPress);
	m_btnPicker.PressEvent += EventHandler(this, &ColorProperty::btnPicker_OnPress);
	
	m_fan.AllowFocus = true;

	m_valRed.ValueName = L"Red";
	m_valRed.ValueColor = Color(1.0, 0.15, 0.15);
	m_valRed.Relative = true;
	m_valRed.MinValue = 0.0;
	m_valRed.MinVisible = 0.0;
	m_valRed.MaxValue = 1.0;
	m_valRed.MaxVisible = 1.0;
	m_valRed.Granularity = 1.0 / 256.0;
	m_valRed.DisplayMultiplier = 100.0;
	m_valRed.AdjustSpeed = 0.0025;
	m_valRed.DecimalPlaces = 0;
	m_valRed.Suffix = L"%";
	m_valRed.ValueChangeEvent += EventHandler(this, &ColorProperty::val_OnValueChange);

	m_valGreen.ValueName = L"Green";
	m_valGreen.ValueColor = Color(0.15, 1.0, 0.15);
	m_valGreen.Relative = true;
	m_valGreen.MinValue = 0.0;
	m_valGreen.MinVisible = 0.0;
	m_valGreen.MaxValue = 1.0;
	m_valGreen.MaxVisible = 1.0;
	m_valGreen.Granularity = 1.0 / 256.0;
	m_valGreen.DisplayMultiplier = 100.0;
	m_valGreen.AdjustSpeed = 0.0025;
	m_valGreen.DecimalPlaces = 0;
	m_valGreen.Suffix = L"%";
	m_valGreen.ValueChangeEvent += EventHandler(this, &ColorProperty::val_OnValueChange);

	m_valBlue.ValueName = L"Blue";
	m_valBlue.TextColor = Color::Black;
	m_valBlue.ValueColor = Color(0.12, 0.35, 1.0);
	m_valBlue.Relative = true;
	m_valBlue.MinValue = 0.0;
	m_valBlue.MinVisible = 0.0;
	m_valBlue.MaxValue = 1.0;
	m_valBlue.MaxVisible = 1.0;
	m_valBlue.Granularity = 1.0 / 256.0;
	m_valBlue.DisplayMultiplier = 100.0;
	m_valBlue.AdjustSpeed = 0.0025;
	m_valBlue.DecimalPlaces = 0;
	m_valBlue.Suffix = L"%";
	m_valBlue.ValueChangeEvent += EventHandler(this, &ColorProperty::val_OnValueChange);

	m_layoutRgb.Padding = 10;
	m_layoutRgb.Gap = 5;
	m_layoutRgb.Orientation = LineLayout::Vertical;
	m_layoutRgb.BackColor = Color(Color::Back, 0.82);
	m_layoutRgb.AutoSize = true;
	m_layoutRgb.Controls.Add(&m_valRed);
	m_layoutRgb.Controls.Add(&m_valGreen);
	m_layoutRgb.Controls.Add(&m_valBlue);

	ChangeEvent += EventHandler(this, &ColorProperty::OnChange);

	m_fan.ColorChangeEvent += EventHandler(this, &ColorProperty::fan_OnColorChange);
	m_fan.LoseFocusEvent += EventHandler(this, &ColorProperty::fan_OnLoseFocus);

	m_button.ColorChangeEvent += EventHandler(this, &ColorProperty::button_OnColorChange);
	//m_button.PressEvent += EventHandler(this, &ColorProperty::button_OnPress);
}

Object* ColorProperty::Copy()
{
	return new ColorProperty(*this);
}

ColorProperty::operator Color()
{
	return m_value;
}

ColorProperty& ColorProperty::operator = (const Color& color)
{
	Color newVal = color;
	Property::Message msg(&newVal);

	PreChangeEvent.Send(msg);
	m_value = newVal;
	ChangeEvent.Send(msg);

	return *this;
}

bool ColorProperty::operator == (const Color& color) { return m_value == color; }
bool ColorProperty::operator != (const Color& color) { return m_value != color; }

Control* ColorProperty::GetControl() const
{
	return (ColorButton*)&m_button;
}

void ColorProperty::PerformAction()
{
	
}