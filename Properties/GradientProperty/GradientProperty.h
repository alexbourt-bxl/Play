#ifndef GRADIENT_PROPERTY_H
#define GRADIENT_PROPERTY_H
//
//#include <Properties\Property.h>
//#include <Types/Color\Gradient.h>
//#include <Controls\ColorFan\ColorFan.h>
//#include <Controls\ColorButton\ColorButton.h>
//#include <Controls\Layout\LineLayout\LineLayout.h>
//#include <Controls\Valuebox\Valuebox.h>
//
//class GradientProperty
//	: public Property
//{
//	DECLARE_TYPE;
//
//public:
//	GradientProperty();
//	explicit GradientProperty(const GradientProperty& prop);
//	~GradientProperty();
//
//	void		Init();
//	Object*		Copy();
//
//	operator	Gradient&();
//	
//	_Control*	GetControl() const;
//
//protected:
//	void		PerformAction();
//
//private:
//	Gradient	m_gradient;
//	void		OnChange(Property::Message& msg);
//
//	ColorFan	m_fan;
//
//	ColorButton m_button;
//	void		button_OnColorChange(::Message& msg);
//	//void		button_OnPress(Control::Message& msg);
//
//	Button		m_btnFan;		
//	Button		m_btnSliders;	
//	Button		m_btnPicker;		
//
//	void		btnFan_OnPress(Control::Message& msg);
//	void		btnSliders_OnPress(Control::Message& msg);
//	void		btnPicker_OnPress(Control::Message& msg);
//
//	static GdiImage8 s_imgFan;
//	static GdiImage8 s_imgSliders;
//	static GdiImage8 s_imgPicker;
//
//	LineLayout	m_layoutRgb;
//	Valuebox	m_valRed;
//	Valuebox	m_valGreen;
//	Valuebox	m_valBlue;
//	void		val_OnValueChange(ValueMessage& msg);
//
//	void		fan_OnColorChange(::Message& msg);
//	void		fan_OnLoseFocus(Control::Message& msg);
//};

#endif