#include "Sidebar.h"

DEFINE_TYPE(Sidebar, LineLayout);

Sidebar::Sidebar()
{
	PaintEvent += EventHandler(this, &Sidebar::OnPaint);

	SuspendUpdates();

	Width = 55;
	Orientation = Vertical;

	ResumeUpdates();
}