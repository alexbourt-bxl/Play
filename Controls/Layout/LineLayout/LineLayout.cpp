#include "LineLayout.h"

#include <Controls\Layout\Spacer\Spacer.h>
#include <Animation\Animation.h>

DEFINE_TYPE(LineLayout, Layout);

LineLayout::LineLayout()
{
	PaintEvent += EventHandler(this, &LineLayout::OnPaint);
	PreUpdateLayoutEvent += EventHandler(this, &LineLayout::OnPreUpdateLayout);
	PreVisibleChangeEvent += EventHandler(this, &LineLayout::OnPreVisibleChange);

	*Controls.AddEvent += EventHandler(this, &LineLayout::OnAddControl);
	*Controls.RemoveEvent += EventHandler(this, &LineLayout::OnRemoveControl);

	SuspendUpdates();

	m_orientation = Horizontal;
	m_wrap = false;
	m_reverse = false;
	m_gap = Point(0, 0);
	
	m_animate = false;
	m_forceNoAnimate = false;
	
	m_cachedContentSize = _Size(0, 0);

	ResumeUpdates();
}

Size LineLayout::GetContentSize(_Size* refSize)
{
	if (m_cachedContentSize.Width > 0 
		&& m_cachedContentSize.Height > 0)
		return m_cachedContentSize;

	int refSizeAlong = refSize 
		? (m_orientation == Horizontal
			? refSize->Width
			: refSize->Height)
		: 0;

	int along = 0;
	int across = 0;

	int lineAlong = 0;
	int maxLineAlong = 0;
	int lineAcross = 0;

	for (int _i = 0; _i < Controls.Count; _i++)
	{
		int i = m_reverse
			? Controls.Count - 1 - _i
			: _i;

		int _gapAlong = _i < Controls.Count - 1
			? GetGapAlong()
			: 0;

		Control* control = Controls[i];

		int controlSizeAlong = GetControlSizeAlong(control);
		int controlSizeAcross = GetControlSizeAcross(control);

		if ((m_orientation == Horizontal 
				&& (control->Dock == DockFillVertical || control->Dock == DockFill))
			|| (m_orientation == Vertical 
				&& (control->Dock == DockFillHorizontal || control->Dock == DockFill)))
		{
			controlSizeAcross = 0;
		}

		/*	If Wrap is enabled and the current control (plus gap)
			stretches out past the end of the client rect, don't
			count the control as part of this line. */
		if (m_wrap
			&& _i > 0 
			&& refSizeAlong > 0
			&& along + controlSizeAlong > refSizeAlong)                                                                                                                                                                                                               
		{
			// take away the previous control's gap
			along -= GetGapAlong();

			if (along > lineAlong)
				lineAlong = along;
			
			if (lineAlong > maxLineAlong)
				maxLineAlong = lineAlong;

			along = 0;
			across += lineAcross + GetGapAcross(); // going to a new line always involves a gap
			
			lineAlong = 0; // the line length must be reset
			lineAcross = 0;
		}

		if (controlSizeAcross > lineAcross)
			lineAcross = controlSizeAcross;

		along += controlSizeAlong + _gapAlong;
	}

	/*	Add the last line. */

	across += lineAcross;

	if (along > lineAlong)
		lineAlong = along;

	if (lineAlong > maxLineAlong)
		maxLineAlong = lineAlong;

	m_cachedContentSize = m_orientation == Horizontal
		? _Size(maxLineAlong, across)
		: _Size(across, maxLineAlong);

	return m_cachedContentSize;

	/*	BUG:

		If there are four controls, two of width 50 and two of width 120,
		and the LineLayout they're in wraps, then if the layout width is less
		than 120, the content size will give a height of four controls,
		but the controls will be laid out as three. */
}

bool LineLayout::AdjustSize(_Size& size)
{
	if (AutoSize)
	{
		size.Width  -= Padding * 2;
		size.Height -= Padding * 2;

		bool _refSize = 
			Dock == DockFill
			|| (m_orientation == Horizontal && (Dock == DockFillHorizontal || Dock == DockTop || Dock == DockBottom))
			|| (m_orientation == Vertical && (Dock == DockFillVertical || Dock == DockLeft || Dock == DockRight));

		_Size* refSize = _refSize
			? &size
			: null;

		_Size contentSize = GetContentSize(refSize);
		
		size = _Size(
			contentSize.Width + Padding * 2,
			contentSize.Height + Padding * 2);

		return true;
	}

	return false;
}

void LineLayout::PerformLayout()
{
	/*	Create the initial array 
		to hold target positions. */

	Array<Point> points;

	for (int i = 0; i < Controls.Count; i++)
		points.Add(Controls[i]->Position);

	/*	Controls are positioned and aligned in four passes:

		1) Each control is positioned and across-aligned to the middle of the line.
		2) The line is adjusted based on spacers it contains.
		2) Docked controls are positioned and scaled on the line.
		3) Each line is along-aligned to the client rect.
		4) All the controls as a unity are aligned to the client rect. 
	*/

	int along = 0;
	int across = 0;

	int lineAlong = 0;
	int lineAcross = 0;
	int lineStartIndex = 0;

	_Size clientSize = ClientRect.Size;

	clientSize.Width  -= Padding * 2;
	clientSize.Height -= Padding * 2;

	int clientSizeAcross = m_orientation == Horizontal
		? clientSize.Height
		: clientSize.Width;

	_Size contentSize = GetContentSize(&clientSize);

	int contentSizeAlong = m_orientation == Horizontal
		? contentSize.Width
		: contentSize.Height;

	int contentSizeAcross = m_orientation == Horizontal
		? contentSize.Height
		: contentSize.Width;

	for (int _i = 0; _i < Controls.Count; _i++)
	{
		int i = m_reverse
			? Controls.Count - 1 - _i
			: _i;

		Control* control = Controls[i];

		int controlSizeAlong = GetControlSizeAlong(control);
		int controlSizeAcross = GetControlSizeAcross(control);

		if ((m_orientation == Horizontal 
				&& (control->Dock == DockFillVertical || control->Dock == DockFill))
			|| (m_orientation == Vertical 
				&& (control->Dock == DockFillHorizontal || control->Dock == DockFill)))
		{
			controlSizeAcross = 0;
		}

		int _gapAlong = _i < Controls.Count - 1
			? GetGapAlong()
			: 0;

		/*	If Wrap is enabled and the control (plus gap) stretches out 
			past the end of the client rect, or the control is a line break, 
			move it to the next line and align the current line. */
		if (m_wrap
			&& _i > 0 
			&& along + controlSizeAlong > contentSizeAlong)
		{
			along -= GetGapAlong(); // take away the previous control's gap

			if (along > lineAlong)
				lineAlong = along;

			///*	TODO:

			//	If this is the only control on the line, set its reference rect
			//	to the available space remaining. This will ensure that child
			//	LineLayouts also have a chance to wrap. */
			//if (i - lineStartIndex == 1)
			//{
			//	_Rect refRect = Controls[i]->GetRefRect();
			//	refRect.Width = contentSizeAlong;
			//	Controls[i]->SetRefRect(refRect);
			//}

			PositionAlignControls(
				points,
				lineStartIndex, 
				i, 
				lineAlong, 
				lineAcross,
				across,
				false); // since we're wrapping, ignore spacer weights

			along = 0;
			across += lineAcross + GetGapAcross(); // going to a new line always involves a gap

			lineAlong = 0; // the line length must be reset
			lineAcross = 0;

			lineStartIndex = _i;
		}

		SetControlPosition(
			points[i],
			along, 
			across);

		if (controlSizeAcross > lineAcross)
			lineAcross = controlSizeAcross;
	
		along += controlSizeAlong + _gapAlong;
	}

	if (along > lineAlong)
		lineAlong = along;

	/*	If the layout doesn't wrap, position 
		and align the only line. */
	if (!m_wrap) 
	{
		PositionAlignControls(
			points,
			0, 
			Controls.Count, 
			lineAlong, 
			lineAcross,
			across,
			true);
	}

	/*	Otherwise position and align the last line. */
	else 
	{
		PositionAlignControls(
			points,
			lineStartIndex, 
			Controls.Count, 
			lineAlong, 
			lineAcross,
			across,
			false); // since we're wrapping, ignore spacer weights
	}

	across += lineAcross; // the last line

	/*	Now across-align all the controls
		relative to the client rectangle. */

	int acrossOffset = 0;

	if (m_orientation == Horizontal)
	{
		if		(m_align == Align::Top)    acrossOffset = 0;
		else if (m_align == Align::Middle) acrossOffset = (clientSizeAcross - contentSizeAcross) / 2;
		else if (m_align == Align::Bottom) acrossOffset = clientSizeAcross - contentSizeAcross;
	}
	else
	{
		if		(m_align == Align::Left)   acrossOffset = 0;
		else if (m_align == Align::Center) acrossOffset = (clientSizeAcross - contentSizeAcross) / 2;
		else if (m_align == Align::Right)  acrossOffset = clientSizeAcross - contentSizeAcross;
	}

	/*	^ TODO: put the above code in some way into Layout. */

	for (int _i = 0; _i < Controls.Count; _i++)
	{
		int i = m_reverse
			? Controls.Count - 1 - _i
			: _i;

		//Control* control = Controls[i];

		SetControlPosition(
			points[i],
			Padding + GetControlPositionAlong(points[i]),
			Padding + GetControlPositionAcross(points[i]) + acrossOffset);

		/*	Derived controls that override this method should 
			call UpdateLayout() for all child controls. */
		//control->UpdateLayout(false);
	}

	/*	Make sure the number of cached destinations is the same
		as the number of current destinations. Otherwise the cache
		is invalid. */
	bool cached = m_destCache.Count == points.Count;

	/*	Update child control positions. */

	if (m_animate && !m_forceNoAnimate)
	{
		Animation::Suspend();

		for (int i = 0; i < Controls.Count; i++)
		{
			if (!cached 
				|| (cached && m_destCache[i] != points[i])) // don't animate if the destination doesn't change
			{
				PointAnimation* anim = new PointAnimation();
					anim->PropertyX = Controls[i]->X;
					anim->PropertyY = Controls[i]->Y;
					anim->To = points[i];
					//anim->Speed = 1000.0;
					anim->Duration = 0.6;
					anim->Random = 0.2;
					anim->ScaleInertia = 0.1;
					anim->Curve = AnimationCurve::Smooth;

				Animation::Add(anim);
			}
		}

		Animation::Resume();
	}
	else
	{
		m_forceNoAnimate = false; // one-time action

		for (int i = 0; i < Controls.Count; i++)
		{
			if (!cached 
				|| (cached && m_destCache[i] != points[i])) // don't animate if the destination doesn't change
				Controls[i]->Position = points[i];
		}
	}

	/*	Cache the destinations. */
	m_destCache = points;
}

void LineLayout::PositionAlignControls(Array<Point>& points,
									   int lineStartIndex, int nextLineStartIndex, 
									   int lineSizeAlong, int lineSizeAcross, 
									   int acrossPos,
									   bool checkSpacers)
{
	/*	Across-align all the controls on the line 
		to the middle of the line. */

	for (int i = lineStartIndex; i < nextLineStartIndex; i++)
	{
		Control* control = Controls[i];

		int alongPos = GetControlPositionAlong(points[i]);
		int alongSize = GetControlSizeAlong(control);

		if (lineStartIndex == nextLineStartIndex - 1) // only one control fits on the line
			alongSize = lineSizeAlong;

		/*	The child controls can still 
			be docked, but only across. */
		_Rect fitRect;
		
		if (m_orientation == Horizontal)
		{
			fitRect = _Rect(
				alongPos, 
				acrossPos, 
				alongSize, 
				lineSizeAcross);
		}
		else
		{
			fitRect = _Rect(
				acrossPos, 
				alongPos, 
				lineSizeAcross, 
				alongSize);
		}

		control->SetRefRect(fitRect);

		int acrossSize = GetControlSizeAcross(control);

		SetControlPosition(
			points[i],
			alongPos,
			acrossPos + (lineSizeAcross - acrossSize) / 2);
	}

	/*	Check the line for weighted spacers. */

	int clientSizeAlong = m_orientation == Horizontal
		? ClientRect.Width - Padding * 2
		: ClientRect.Height - Padding * 2;

	if (checkSpacers && lineSizeAlong > clientSizeAlong)
	{
		/*	Find all spacers that can shrink. */

		for (int i = lineStartIndex; i < nextLineStartIndex; i++)
		{
			Control* control = Controls[i];

			if (typeof(control) != TYPEOF(Spacer))
				continue;

			Spacer* spacer = (Spacer*)control;

			if (spacer->CanShrink)
				m_spacers.Add(spacer);
		}

		if (m_spacers.Count > 0)
		{
			/*	Determine the room available 
				for the weighted spacers. */

			int nonSpacerSize = 0;

			for (int i = lineStartIndex; i < nextLineStartIndex; i++)
			{
				Control* control = Controls[i];

				if (typeof(control) != TYPEOF(Spacer) || !m_spacers.Contains((Spacer*)control))
					nonSpacerSize += GetControlSizeAlong(control);
			}

			int spacerRoom = max(0, clientSizeAlong - nonSpacerSize);

			/*	Adjust normalized weights. */

			int totalSize = 0;

			for (int i = 0; i < m_spacers.Count; i++)
			{
				Spacer* spacer = m_spacers[i];

				totalSize += m_orientation == Horizontal
					? spacer->m_setWidth
					: spacer->m_setHeight;
			}

			if (totalSize > 0)
			{
				for (int i = 0; i < m_spacers.Count; i++)
				{
					Spacer* spacer = m_spacers[i];

					spacer->m_weight = m_orientation == Horizontal
						? (double)spacer->m_setWidth / (double)totalSize
						: (double)spacer->m_setHeight / (double)totalSize;
				}

				/*	Adjust the spacer sizes. */

				for (int i = 0; i < m_spacers.Count; i++)
				{
					Spacer* spacer = m_spacers[i];

					spacer->Width.DisableEvents();
					spacer->Height.DisableEvents();

					if (m_orientation == Horizontal)
						spacer->Width = (int)((double)spacerRoom * spacer->m_weight);
					else
						spacer->Height = (int)((double)spacerRoom * spacer->m_weight);

					spacer->Width.EnableEvents();
					spacer->Height.EnableEvents();

					/*	Reposition all the controls 
						that follow the spacer. */

					int spacerIndex	= Controls.GetIndexOf(spacer);
					int along = GetControlPositionAlong(points[spacerIndex]) + GetControlSizeAlong(spacer, false) + GetGapAlong();

					for (int i = spacerIndex + 1; i < nextLineStartIndex; i++)
					{
						Control* control = Controls[i];
						int acrossPos = GetControlPositionAcross(points[i]);

						SetControlPosition(
							points[i],
							along,
							acrossPos);

						along += GetControlSizeAlong(control, false) + GetGapAlong();
					}
				}
			}

			// Clean up
			m_spacers.Clear();
		}
	}

	/*	Along-align all the controls on 
		the line to the client rect. */

	int alongOffset = 0;

	if (m_orientation == Horizontal)
	{
		if		(m_align == Align::Left)   alongOffset = 0;
		else if (m_align == Align::Center) alongOffset = (clientSizeAlong - lineSizeAlong) / 2;
		else if (m_align == Align::Right)  alongOffset = clientSizeAlong - lineSizeAlong;
	}
	else
	{
		if		(m_align == Align::Top)    alongOffset = 0;
		else if (m_align == Align::Middle) alongOffset = (clientSizeAlong - lineSizeAlong) / 2;
		else if (m_align == Align::Bottom) alongOffset = clientSizeAlong - lineSizeAlong;
	}

	for (int i = lineStartIndex; i < nextLineStartIndex; i++)
	{
		int alongPos = GetControlPositionAlong(points[i]);
		int acrossPos = GetControlPositionAcross(points[i]);

		SetControlPosition(
			points[i],
			alongPos + alongOffset,
			acrossPos);
	}
}

LineLayout::_Orientation LineLayout::GetOrientation()
{
	return m_orientation;
}

void LineLayout::SetOrientation(_Orientation orientation)
{
	m_orientation = orientation;
	UpdateLayout();
}

bool LineLayout::GetWrap()
{
	return m_wrap;
}

void LineLayout::SetWrap(bool wrap)
{
	m_wrap = wrap;
	UpdateLayout();
}

bool LineLayout::GetReverse()
{
	return m_reverse;
}

void LineLayout::SetReverse(bool reverse)
{
	m_reverse = reverse;
	UpdateLayout();
}

Point& LineLayout::GetGap()
{
	return m_gap;
}

void LineLayout::SetGap(const Point& gap)
{
	m_gap = gap;
	UpdateLayout();
}

void LineLayout::SetGap(int gap)
{
	m_gap.X = gap;
	m_gap.Y = gap;
	UpdateLayout();
}

bool LineLayout::GetAnimate()
{
	return m_animate;
}

void LineLayout::SetAnimate(bool animate)
{
	m_animate = animate;
}

inline int LineLayout::GetControlPositionAlong(Point& p)
{
	return m_orientation == Horizontal
		? p.X
		: p.Y;
}

inline int LineLayout::GetControlPositionAcross(Point& p)
{
	return m_orientation == Horizontal
		? p.Y
		: p.X;
}

inline int LineLayout::GetControlSizeAlong(Control* control, bool checkSpacer)
{
	if (checkSpacer && typeof(control) == TYPEOF(Spacer)) // this is a special case
	{
		Spacer* spacer = (Spacer*)control;
		return m_orientation == Horizontal
			? spacer->m_setWidth
			: spacer->m_setHeight;
	}
	else
	{
		return m_orientation == Horizontal
			? control->Width
			: control->Height;
	}
}

inline int LineLayout::GetControlSizeAcross(Control* control)
{
	return m_orientation == Horizontal
		? control->Height
		: control->Width;
}

inline void LineLayout::SetControlPosition(Point& p, int along, int across)
{
	if (m_orientation == Horizontal)
		p = Point(along, across);
	else
		p = Point(across, along);
}

inline int LineLayout::GetGapAlong()
{
	return m_orientation == Horizontal
		? m_gap.X
		: m_gap.Y;
}

inline int LineLayout::GetGapAcross()
{
	return m_orientation == Horizontal
		? m_gap.Y
		: m_gap.X;
}