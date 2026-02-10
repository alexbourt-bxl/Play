#include "Control.h"

#include <Devices\Keyboard\KeyCombo.h>

/// <summary>The Paint event is fired when the control needs to be painted.</summary>
/// <remarks>This base method automatically handles control transparency. Because the parent control will always (?) be
/// a window whose transparency is handled differently (not using a scratch surface), I am assuming that this
/// will not be a problem. We will see how it goes.</remarks>
//void Control::OnPaint(Graphics::Message& msg)
//{ 
//	// The painting is done relative to the top left corner of the surface
//	// that is being painted on, which is usually common to all the controls
//	// of the window.
//
//	msg.Graphics.FillRect(
//		PaintRect,
//		BackColor);
//}

/*
void Control::OnLButtonUp(Mouse::Message& msg) 
{
	// Setting msg.Result to true, you indicate to the application that the ButtonUp event
	// was the last action, and that no Click event should be fired. If msg.Result is false,
	// the Click event will be fired.

	msg.Result = true;
}
*/

void Control::Position_PreChange(Property::Message& /*msg*/)
{
	Trail.Add(FullPaintRect);
}

void Control::Position_OnChange(Property::Message& /*msg*/)
{
	Update();

	Control::Message ce(this);
	MoveEvent.Send(ce);
}

void Control::Width_PreChange(Property::Message& msg)
{
	Trail.Add(FullPaintRect);

	/*	Limit the width. */

	int& width = *(int*)msg.Value;

	int minWidth = m_minSize.Width > 0
		? m_minSize.Width
		: 1;

	width = max(minWidth, width);

	if (m_maxSize.Width > 0)
		width = min(width, m_maxSize.Width);
}

void Control::Height_PreChange(Property::Message& msg)
{
	Trail.Add(FullPaintRect);

	/*	Limit the height. */

	int& height = *(int*)msg.Value;

	int minHeight = m_minSize.Height > 0
		? m_minSize.Height
		: 1;

	height = max(minHeight, height);

	if (m_maxSize.Height > 0)
		height = min(height, m_maxSize.Height);
}

void Control::Size_OnChange(Property::Message& /*msg*/)
{
	UpdateLayout();

	Control::Message ce(this);
	SizeEvent.Send(ce);
}

void Control::Opacity_PreChange(Property::Message& msg)
{
	double& opacity = *(double*)msg.Value;
	opacity = min(max(0.0, opacity), 1.0);
}

void Control::Opacity_OnChange(Property::Message& /*msg*/)
{
	Update();
}

void Control::Controls_OnAdd(Container::Message& msg)
{
	Control* control = *((Control**)msg.Item);
	assert(control);
	
	control->Parent = this;

	MarkForLayout();
	Update();
}

void Control::Controls_OnRemove(Container::Message& msg)
{
	Control* control = *((Control**)msg.Item);
	assert(control);

	control->Parent = null;

	MarkForLayout();
	Update();
}

void Control::Keys_OnAdd(Container::Message& msg)
{
	/*	Shortcuts should be registered in a specific way. If a shortcut is a subset of another shortcut,
		for example Ctrl+Z is a subset of Ctrl+Shift+Z, then Ctrl+Z should be registered first. The comparison
		is performed backwards, and unless the subset is registered first, the larger combination will never
		be triggered. 
		
		Therefore when a shortcut is added, the array is scanned for any existing shortcuts of which the new one 
		is a subset, and if so, the new shortcut is moved up before the existing shortcut. */

	KeyCombo* keys = *((KeyCombo**)msg.Item);
	
	bool subset = true;

	do
	{
		int index = m_keys.GetIndexOf(keys);

		if (index <= 0)
			return;

		/*	For every previous shortcut check 
			if the new one is its subset. */
		
		for (int i = index - 1; i >= 0; i--)
		{
			for (int j = 0; j < keys->Count; j++)
				subset &= m_keys[i]->Contains((*keys)[j]);

			subset &= m_keys[i]->Count > keys->Count;

			/*	If so, move the new shortcut 
				in front of the existing one. */
			
			if (subset)
			{
				m_keys.SetIndexOf(keys, max(0, i - 1));
				
				/*	Start the next loop from the new position 
					of the new shortcut. */
				break; 
			}
		}
	}
	while (subset);
}