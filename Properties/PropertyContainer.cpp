#include "PropertyContainer.h"

#include <Properties\Property.h>
#include <Controls\Layout\LineLayout\LineLayout.h>

DEFINE_ABSTRACT_TYPE(PropertyContainer);

PropertyContainer::PropertyContainer()
{
	*m_properties.AddEvent += EventHandler(this, &PropertyContainer::properties_OnAdd);

	m_layout = null;
}

PropertyContainer::~PropertyContainer()
{
	if (m_layout)
		delete m_layout;
}

Array<Property*>& PropertyContainer::GetProperties()
{
	return m_properties;
}

LineLayout* PropertyContainer::GetLayout() const
{
	((PropertyContainer*)this)->CheckCreateLayout();
	return m_layout;
}

void PropertyContainer::CheckCreateLayout()
{
	if (!m_layout)
	{
		m_layout = new LineLayout();

		//m_layout->Dock = Control::DockFill;
		m_layout->Orientation = LineLayout::Horizontal;
		m_layout->Gap = 10;
		//m_layout->Wrap = true;
		m_layout->AutoSize = true;
		m_layout->ClipChildren = false;
		m_layout->Align = Align::Center | Align::Middle;
		//m_layout->BackColor = Color::Blue;

		m_layout->SuspendUpdates();
		LayoutControls(m_layout);
		m_layout->ResumeUpdates();
	}
}

void PropertyContainer::LayoutControls(LineLayout* layout)
{
	for (int i = 0; i < m_properties.Count; i++)
		layout->Controls.Add(m_properties[i]->Control);
}