#include "OpHSV.h"

#include <Controls\Valuebox\Valuebox.h>

DEFINE_TYPE(OpHSV, Operator);
SET_TYPE_DISPLAY_NAME(OpHSV, L"HSV");

OpHSV::OpHSV()
{
	Hue        = 0;
	Saturation = 0;
	Value      = 0;

	Init();
}

OpHSV::OpHSV(const OpHSV& op) 
{
	Hue        = op.Hue;
	Saturation = op.Saturation;
	Value      = op.Value;

	Init();
}

OpHSV::~OpHSV()
{

}

void OpHSV::Init()
{
	In.AcceptedTypes.Add(TYPEOF(Image8));
	In.Add(&m_in);

	Out.Add(&m_out);

	Hue.Name = L"Hue";
	Hue.Valuebox->Relative = true;
	Hue.Valuebox->MinValue = -180;
	Hue.Valuebox->MinVisible = -180;
	Hue.Valuebox->MaxValue = 180;
	Hue.Valuebox->MaxVisible = 180;
	Hue.Valuebox->Granularity = 1;
	Hue.Valuebox->AdjustSpeed = 1;
	Hue.Valuebox->DecimalPlaces = 0;
	Hue.Valuebox->Suffix = L"°";
	Properties.Add(&Hue);

	Saturation.Name = L"Saturation";
	Saturation.Valuebox->Relative = true;
	Saturation.Valuebox->MinValue = -1;
	Saturation.Valuebox->MinVisible = -1;
	Saturation.Valuebox->MaxValue = 1;
	Saturation.Valuebox->MaxVisible = 1;
	Saturation.Valuebox->Granularity = 0.01;
	Saturation.Valuebox->DisplayMultiplier = 100;
	Saturation.Valuebox->AdjustSpeed = 0.0025;
	Saturation.Valuebox->DecimalPlaces = 0;
	Saturation.Valuebox->Suffix = L"%";
	Properties.Add(&Saturation);

	Value.Name = L"Value";
	Value.Valuebox->Relative = true;
	Value.Valuebox->MinValue = -1;
	Value.Valuebox->MinVisible = -1;
	Value.Valuebox->MaxValue = 1;
	Value.Valuebox->MaxVisible = 1;
	Value.Valuebox->Granularity = 0.01;
	Value.Valuebox->DisplayMultiplier = 100;
	Value.Valuebox->AdjustSpeed = 0.0025;
	Value.Valuebox->DecimalPlaces = 0;
	Value.Valuebox->Suffix = L"%";
	Properties.Add(&Value);
}

Object* OpHSV::Copy()
{
	return OpHSV(*this);
}

void OpHSV::LayoutControls(LineLayout* layout)
{
	m_layoutVertical.Orientation = LineLayout::Vertical;

	m_layoutVertical.Gap = 5;
	m_layoutVertical.AutoSize = true;
	m_layoutVertical.Controls.Add(Hue.Valuebox);
	m_layoutVertical.Controls.Add(Saturation.Valuebox);
	m_layoutVertical.Controls.Add(Value.Valuebox);

	layout->Controls.Add(&m_layoutVertical);
}

void OpHSV::CopyWorkingValues()
{
	m_hue = Hue;
	m_saturation = Saturation;
	m_value = Value;
}

void OpHSV::Process()
{
	Image* in = (Image*)m_in.GetInputObject();

	if (!in)
	{
		m_out.SetResult(null);
		return; 
	}

	assert(typeof(in) == TYPEOF(Image8));

	/*	Create the output image. */

	Size outSize = in->Size;

	Image8* out = 
		m_out.Result && typeof(m_out.Result) == TYPEOF(Image8) && ((Image8*)m_out.Result)->Size == outSize
		? (Image8*)m_out.Result
		: new Image8(outSize);

	/*	Change the HSV. */

	int* inBuffer = (int*)((Image8*)in)->Buffer;
	int* outBuffer = (int*)out->Buffer;

	int yWidth;

	double h, s, v, a;
	Color c;

	/*	BUG: this produces incorrect results (S desaturates lighter and saturates darder, and V doesn't add uniformly). */

	for (int y = 0; y < outSize.Height; y++)
	{
		yWidth = y * outSize.Width;

		for (int x = 0; x < outSize.Width; x++)
		{
			c = Color8(inBuffer[yWidth + x]);
			a = c.A;

			c.ToHsv(&h, &s, &v);

			h += m_hue;

			if (h > 360) h -= 360;
			if (h < 0)   h += 360;

			s = min(max(0, s + m_saturation), 1);
			v = min(max(0, v + m_value), 1);

			c = Color::FromHsv(h, s, v);
			c.A = a;

			outBuffer[yWidth + x] = (Color8)c;
		}
	}

	m_out.SetResult(out);
}