#ifndef OP_GRADIENT_H
#define OP_GRADIENT_H

/*
	The color positions are 0-1.
*/

#include <Operators\Operator.h>
#include <Properties\IntProperty\IntProperty.h>
#include <Properties\ListProperty\ListProperty.h>
#include <Properties\ColorProperty\ColorProperty.h>
#include <Controls\Layout\LineLayout\LineLayout.h>
#include <Controls\ColorFan\ColorFan.h>
#include <Controls\ColorButton\ColorButton.h>

#include <Types/Graphics\Image8.h>

class OpGradient
	: public Operator
{
	DECLARE_TYPE;

	typedef Color _Color;
	typedef Type _Type;

public:
	OpGradient();
	OpGradient(const OpGradient& op);
	~OpGradient();

	Object*	Copy();

	void	Init();
	void	LayoutControls(LineLayout* layout);

	void	CopyWorkingValues();
	void	Process();

	ListProperty Type;

	Array<ColorProperty> Colors;
	Array<FloatProperty> Positions;

	/*	The positions are stored as properties
		so that they can be animated. */

	FloatProperty X0, Y0;
	FloatProperty X1, Y1;

	void ConnectView(OpView* view);
	void DisconnectView(OpView* view);

private:
	Input	m_in;

	Output	m_out;
	Image*	m_outImage;

	/* Working values. */
	int m_type;
	Array<_Color> m_colors;
	Array<double> m_positions;
	FPoint m_p0, m_p1;

	int m_selected;
	int m_held;

	double m_colorRadius;
	double m_colorBorder;
	double m_selectedBorder;

	//

	void Update_Linear(int* image, int width, int height);
	void Update_Radial(int* image, int width, int height);
	void Update_Angle(int* image, int width, int height);

	//

	void OnPaint(Graphics::Message& msg);
	void OnLButtonDown(Mouse::Message& msg);
	void OnLButtonUp(Mouse::Message& msg);
	void OnMouseMove(Mouse::Message& msg);
};

#endif