#include "Grip.h"
//#include "Graphics.h"

DEFINE_TYPE(Grip, Control);

Grip::Grip()
{

}

Grip::Grip(Grip::Result result)
{
	m_result = result;
	//PaintEvent = EventHandler(this, &Grip::OnPaint);
}

Grip::Result Grip::GetResult()
{
	return m_result;
}

void Grip::SetResult(Grip::Result result)
{
	m_result = result;
}