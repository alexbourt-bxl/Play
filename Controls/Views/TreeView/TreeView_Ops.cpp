#include "TreeView.h"

#include <Operators/Operator.h>
#include <Operators/Tree/Tree.h>
#include <Operators/Input.h>
#include <Operators/Output.h>
#include <Operators/Connection.h>
#include <Types/Graphics/Font.h>
#include <Types/Geometric/FRect.h>
#include <Controls/Views/OpView/OpView.h>
#include <Operators/Actions/ActDeleteOps.h>

FSize TreeView::GetOpSize(Operator* op)
{
	int nInputs = op->In.Count;
	if (!op->In.CountIsFixed)
		nInputs++;
	
	int nOutputs = op->Out.Count;
	int nConnections = max(nInputs, nOutputs);

	Font font(L"Segoe UI", 18, Font::SemiBold);

	Graphics g(this);
	_Size textSize = g.MeasureText(op->DisplayName, font);

	double width = 
		  s_hPadding 
		+ s_hPadding2 
		+ (double)textSize.Width
		+ s_hPadding2 
		+ s_hPadding;

	double height =
		  s_vPadding 
		+ (double)nConnections * (double)s_imgConnectionRing.Height
		+ (double)(nConnections - 1) * s_vGap 
		+ s_vPadding;

	Image* thumbnail = op->GetThumbnail(_Size(100, 100));

	if (thumbnail)
	{
		if ((double)(thumbnail->Width + 4) > width)
			width = (double)(thumbnail->Width + 4);

		if ((double)(thumbnail->Height + 4) > height)
			height = (double)(thumbnail->Height + 4);
	}

	height = max(31, height);

	return FSize(width, height);
}

FRect TreeView::GetInputRect(Input* in)
{
	Operator* op = in->GetOperator();

	int nInputs = op->In.NewInput
		? op->In.Count + 1
		: op->In.Count;
	
	int index = in == op->In.NewInput
		? op->In.Count
		: op->In.GetIndexOf(in);

	double span = nInputs * s_imgConnectionRing.Height + (nInputs - 1) * s_vGap;
	FSize opSize = GetOpSize(op);

	return FRect(
		s_hPadding + s_hGap,
		(opSize.Height + span) / 2 - (double)((index + 1) * s_imgConnectionRing.Height) - (double)index * s_vGap,
		(double)s_imgConnectionRing.Width,
		(double)s_imgConnectionRing.Height);
}

FRect TreeView::GetOutputRect(Output* out)
{
	Operator* op = out->GetOperator();

	if (!op)
		return FRect::Null;
	// TODO: need to figure out how outputs will sit in OpGraph

	int index = op->Out.GetIndexOf(out);

	FSize opSize = GetOpSize(op);
	double span = op->Out.Count * s_imgConnectionRing.Height + (op->Out.Count - 1) * s_vGap;

	return FRect(
		opSize.Width - s_hPadding - s_hGap - (double)s_imgConnectionRing.Width,
		(opSize.Height + span) / 2 - (double)((index + 1) * s_imgConnectionRing.Height) - (double)index * s_vGap,
		(double)s_imgConnectionRing.Width,
		(double)s_imgConnectionRing.Height);
}

Rect TreeView::GetOpPaintRect(Operator* op)
{
	FSize size = GetOpSize(op);

	return _Rect(
		PaintRect.X + (int)op->Position.X,
		PaintRect.Y + (int)op->Position.Y,
		(int)size.Width,
		(int)size.Height);
}

Rect TreeView::GetOpFullPaintRect(Operator* op)
{
	_Rect fullPaintRect = GetOpPaintRect(op);

	fullPaintRect.X      += s_skinOp.OX;
	fullPaintRect.Y      += s_skinOp.OY;
	fullPaintRect.Width  += s_skinOp.OW;
	fullPaintRect.Height += s_skinOp.OH;

	return fullPaintRect;
}

Region TreeView::GetOpRegion(Operator* op)
{
	Region opTrail;
	
	_Rect fullPaintRect = GetOpFullPaintRect(op);
	opTrail.Add(fullPaintRect);

	// Inputs

	for (int i = 0; i < op->In.Count; i++)
	{
		if (op->In[i]->Connection)
		{
			Operator* opOut = op->In[i]->Connection->Out->GetOperator();
			_Rect outFullPaintRect = GetOpFullPaintRect(opOut);
			opTrail.Add(outFullPaintRect);
		
			opTrail.Add(GetConnectionRegion(op->In[i]->Connection));
		}
	}

	// Outputs

	for (int i = 0; i < op->Out.Count; i++)
	{
		for (int j = 0; j < op->Out[i]->Connections.Count; j++)
		{
			Operator* opIn = op->Out[i]->Connections[j]->In->GetOperator();
			_Rect inFullPaintRect = GetOpFullPaintRect(opIn);
			opTrail.Add(inFullPaintRect);

			opTrail.Add(GetConnectionRegion(op->Out[i]->Connections[j]));
		}
	}

	return opTrail;
}

void TreeView::GetConnectionPoints(Output* out, Input* in, FPoint* p0, FPoint* p1, FPoint* p2, FPoint* p3)
{
	Operator* opOut = out 
		? out->GetOperator()
		: null;

	Operator* opIn = in
		? in->GetOperator()
		: null;

	/*	Set up bezier points. */

	if (out)
	{
		_Rect outPaintRect = GetOpPaintRect(opOut);
		FRect outRect = GetOutputRect(out);

		*p0 = FPoint(
			(double)(outPaintRect.X + (int)outRect.Right - 1), 
			(double)(outPaintRect.Y + (int)outRect.Y + (int)outRect.Height / 2));
	}

	if (in)
	{
		_Rect inPaintRect = GetOpPaintRect(opIn);
		FRect inRect = GetInputRect(in);

		*p3 = FPoint(
			(double)(inPaintRect.X + (int)inRect.X - s_imgConnectedArrow.Width + 1), 
			(double)(inPaintRect.Y + (int)inRect.Y + (int)inRect.Height / 2));
	}

	double backLimit = 50;

	double fy = fabs(p3->Y - p0->Y);
	double fx = fabs(max(0, p0->X - p3->X)) / backLimit;

	fx = fx < 1
		? SQR(fx) / 4
		: fx / 2 - 0.25;

	double pExtents = fy / 4 + fx * backLimit / 2;

	*p1 = FPoint(p0->X + pExtents, p0->Y);
	*p2 = FPoint(p3->X - pExtents, p3->Y);
}

Region TreeView::GetConnectionRegion(Connection* conn)
{
	FPoint p0, p1, p2, p3;
	GetConnectionPoints(conn->Out, conn->In, &p0, &p1, &p2, &p3);

	Region rgn;

	//double cageLength = 
	//	  Math::GetDistance(p0, p1)
	//	+ Math::GetDistance(p1, p2)
	//	+ Math::GetDistance(p2, p3);

	//if (cageLength == 0.0)
	//	cageLength = ZERO;

	//int trailGrain = 40;
	//double bezierStep = (double)trailGrain / cageLength;

	//for (double t = 0; t <= 1.0; t += bezierStep)
	//{
	//	double x = Math::LerpBezier(p0.X, p1.X, p2.X, p3.X, t);
	//	double y = Math::LerpBezier(p0.Y, p1.Y, p2.Y, p3.Y, t);
	//	
	//	rgn.Add(Rect(
	//		(int)x - trailGrain / 2,
	//		(int)y - trailGrain / 2,
	//		trailGrain,
	//		trailGrain));
	//}

	_Rect rect;
	
	rect.Expand(Point((int)p0.X, (int)p0.Y));
	rect.Expand(Point((int)p1.X, (int)p1.Y));
	rect.Expand(Point((int)p2.X, (int)p2.Y));
	rect.Expand(Point((int)p3.X, (int)p3.Y));

	rect.Left   -= 2;
	rect.Right  += 2;
	rect.Top    -= 2;
	rect.Bottom += 4;

	rect.Top    -= 10;
	rect.Bottom += 10;

	rgn.Add(rect);

	return rgn;
}

Rect TreeView::GetHotInputRect(const FPoint& p, Input* in)
{
	FPoint p0, p1, p2, p3;

	p0 = p;
	GetConnectionPoints(null, in, &p0, &p1, &p2, &p3);

	_Rect rect;
	
	rect.Expand(Point((int)p0.X, (int)p0.Y));
	rect.Expand(Point((int)p1.X, (int)p1.Y));
	rect.Expand(Point((int)p2.X, (int)p2.Y));
	rect.Expand(Point((int)p3.X, (int)p3.Y));

	rect.Left   -= 2;
	rect.Right  += 2;
	rect.Top    -= 2;
	rect.Bottom += 4;
	
	rect.Left   -= s_imgConnectedCircle.Width + s_imgConnectedArrow.Width;
	rect.Right  += s_imgConnectedCircle.Width + s_imgConnectedArrow.Width;
	rect.Top    -= 10;
	rect.Bottom += 10;

	return rect;
}

Rect TreeView::GetHotOutputRect(Output* out, const FPoint& p)
{
	FPoint p0, p1, p2, p3;

	p3 = p;
	GetConnectionPoints(out, null, &p0, &p1, &p2, &p3);

	_Rect rect;
	
	rect.Expand(Point((int)p0.X, (int)p0.Y));
	rect.Expand(Point((int)p1.X, (int)p1.Y));
	rect.Expand(Point((int)p2.X, (int)p2.Y));
	rect.Expand(Point((int)p3.X, (int)p3.Y));

	rect.Left   -= 2;
	rect.Right  += 2;
	rect.Top    -= 2;
	rect.Bottom += 4;
	
	rect.Left   -= s_imgConnectedCircle.Width + s_imgConnectedArrow.Width;
	rect.Right  += s_imgConnectedCircle.Width + s_imgConnectedArrow.Width;
	rect.Top    -= 10;
	rect.Bottom += 10;

	return rect;
}

FPoint TreeView::GetHotPoint(const Point& p)
{
	Object* obj = HitTestOp(p);

	if (m_hotOutput)
	{
		if (obj && typeof(obj) == TYPEOF(Input)
			&& ((Input*)obj)->GetOperator() && ((Input*)obj)->GetOperator() != m_hotOutput->GetOperator())
		{
			Input* in = (Input*)obj;
			Operator* op = in->GetOperator();

			_Rect paintRect = GetOpPaintRect(op);
			FRect inRect = GetInputRect(in);
			
			return FPoint(
				(double)paintRect.X + inRect.X + inRect.Width / 2 + 1,
				(double)paintRect.Y + inRect.Y + inRect.Height / 2);
		}
		else if (obj && typeof(obj) == TYPEOF(Operator)
			&& obj != m_hotOutput->GetOperator())
		{
			Operator* op = (Operator*)obj;

			_Rect paintRect = GetOpPaintRect(op);
			
			if (op->In.CountIsFixed && op->In.Count == 1)
			{
				FRect inRect = GetInputRect(op->In[0]);
				return FPoint(
					(double)paintRect.X + inRect.X + inRect.Width / 2 + 1,
					(double)paintRect.Y + inRect.Y + inRect.Height / 2);
			}
			else if (op->In.NewInput)
			{
				FRect inRect = GetInputRect(op->In.NewInput);
				return FPoint(
					(double)paintRect.X + inRect.X + inRect.Width / 2 + 1,
					(double)paintRect.Y + inRect.Y + inRect.Height / 2);
			}
		}
	}
	else if (m_hotInput)
	{
		if (obj && typeof(obj) == TYPEOF(Output)
			&& ((Output*)obj)->GetOperator() != m_hotInput->GetOperator())
		{
			Output* out = (Output*)obj;
			Operator* op = out->GetOperator();

			_Rect paintRect = GetOpPaintRect(op);
			FRect outRect = GetOutputRect(out);
			
			return FPoint(
				(double)paintRect.X + outRect.X + outRect.Width / 2 + 1,
				(double)paintRect.Y + outRect.Y + outRect.Height / 2);
		}
		else if (obj && typeof(obj) == TYPEOF(Operator)
			&& obj != m_hotInput->GetOperator())
		{
			Operator* op = (Operator*)obj;
			
			if (op->Out.Count == 1)
			{
				_Rect paintRect = GetOpPaintRect(op);
				FRect outRect = GetOutputRect(op->Out[0]);
				
				return FPoint(
					(double)paintRect.X + outRect.X + outRect.Width / 2 + 1,
					(double)paintRect.Y + outRect.Y + outRect.Height / 2);
			}
		}
	}

	//

	return FPoint(
		(double)(p.X + PaintRect.X),
		(double)(p.Y + PaintRect.Y));
}

void TreeView::PaintOp(Graphics::Message& msg, Operator* op)
{
	_Rect paintRect = GetOpPaintRect(op);

	msg.Graphics.FillRect(
		paintRect.X + 2,
		paintRect.Y + 2,
		paintRect.Width - 4,
		paintRect.Height - 4,
		Color(0.19, 0.19, 0.19, 0.88));

	/*	Draw the thumbnail if available. */

	Image* thumbnail = op->GetThumbnail(_Size(100, 100));

	if (thumbnail)
	{
		msg.Graphics.DrawImage(
			paintRect.X + (paintRect.Width - thumbnail->Width) / 2,
			paintRect.Y + (paintRect.Height - thumbnail->Height) / 2,
			thumbnail);
	}

	/*	Draw the selected skin. 
	
		Go through the tree's assigned views and 
		check if one of them is showing this op. */

	for (int i = 0; i < op->Tree->Views.Count; i++)
	{
		View* view = op->Tree->Views[i];

		if (typeof(view) != TYPEOF(OpView))
			continue;

		if (((OpView*)view)->ViewedOperator == op)
		{
			msg.Graphics.DrawSkin(
				paintRect,
				s_skinOpShown, 
				op->IsSelected 
					? 0.65 
					: 0.4);

			break;
		}
	}

	/*	Draw the skin. */

	Skin* skinOp = &s_skinOp;

	for (int i = 0; i < op->Tree->Views.Count; i++)
	{
		View* view = op->Tree->Views[i];

		if (typeof(view) != TYPEOF(OpView))
			continue;

		if (((OpView*)view)->ActiveOperator == op)
		{
			skinOp = &s_skinOpActive;
			break;
		}
	}

	assert(skinOp);

	msg.Graphics.DrawSkin(
		paintRect, 
		*skinOp,
		op->IsSelected 
			? 1 
			: 0.5);

	/*	Draw the shadow. */

	Font font(L"Segoe UI", 18, Font::SemiBold);

	msg.Graphics.DrawText(
		op->DisplayName,
		font,
		paintRect.X + paintRect.Width / 2 - 1,
		paintRect.Y + paintRect.Height / 2 - 1,
		Color(0.19, 0.19, 0.19, 0.88), //Color(0xE0303030),
		&paintRect,
		Align::Center | Align::Middle);

	msg.Graphics.DrawText(
		op->DisplayName,
		font,
		paintRect.X + paintRect.Width / 2 + 1,
		paintRect.Y + paintRect.Height / 2 - 1,
		Color(0.19, 0.19, 0.19, 0.88), //Color(0xE0303030),
		&paintRect,
		Align::Center | Align::Middle);

	msg.Graphics.DrawText(
		op->DisplayName,
		font,
		paintRect.X + paintRect.Width / 2,
		paintRect.Y + paintRect.Height / 2 - 2,
		Color(0.19, 0.19, 0.19, 0.88), //Color(0xE0303030),
		&paintRect,
		Align::Center | Align::Middle);

	msg.Graphics.DrawText(
		op->DisplayName,
		font,
		paintRect.X + paintRect.Width / 2,
		paintRect.Y + paintRect.Height / 2,
		Color(0.19, 0.19, 0.19, 0.88), //Color(0xE0303030),
		&paintRect,
		Align::Center | Align::Middle);

	/*	Draw the name. */

	msg.Graphics.DrawText(
		op->DisplayName,
		font,
		paintRect.X + paintRect.Width / 2,
		paintRect.Y + paintRect.Height / 2 - 1,
		op->IsSelected 
			? Color(Color::White, 0.94) //Color(0xF0FFFFFF) 
			: Color(Color::White, 0.75), //Color(0xC0FFFFFF),
		&paintRect,
		Align::Center | Align::Middle);

	/*	Draw the inputs. */

	for (int i = 0; i < op->In.Count; i++)
	{
		FRect inRect = GetInputRect(op->In[i]);

		if (op->In[i]->Connection || op->In[i] == m_hotInput)
		{
			msg.Graphics.DrawImage(
				paintRect.X + (int)inRect.X, 
				paintRect.Y + (int)inRect.Y,
				&s_imgConnectedCircle,
				s_connectedOpacity);
		}
		else
		{
			msg.Graphics.DrawImage(
				paintRect.X + (int)inRect.X, 
				paintRect.Y + (int)inRect.Y,
				&s_imgConnectionRing,
				s_disconnectedOpacity);
		}
	}

	/*	Draw the new input plus. */

	if (op->In.NewInput)
	{
		FRect plusRect = GetInputRect(op->In.NewInput);

		msg.Graphics.DrawImage(
			paintRect.X + (int)plusRect.X + 1, 
			paintRect.Y + (int)plusRect.Y,
			&s_imgConnectionPlus,
			s_disconnectedOpacity);
	}

	/*	Draw the outputs. */

	for (int i = 0; i < op->Out.Count; i++)
	{
		FRect outRect = GetOutputRect(op->Out[i]);

		if (op->Out[i]->Connections.Count > 0 || op->Out[i] == m_hotOutput)
		{
			msg.Graphics.DrawImage(
				paintRect.X + (int)outRect.X, 
				paintRect.Y + (int)outRect.Y,
				&s_imgConnectedCircle,
				s_connectedOpacity);
		}
		else
		{
			msg.Graphics.DrawImage(
				paintRect.X + (int)outRect.X, 
				paintRect.Y + (int)outRect.Y,
				&s_imgConnectionRing,
				s_disconnectedOpacity);
		}
	}
}

void TreeView::PaintOpConnections(Graphics::Message& msg, Operator* op)
{
	/*	Paint connections. */

	for (int i = 0; i < op->Out.Count; i++)
	{
		FRect outRect = GetOutputRect(op->Out[i]);

		Output* out = op->Out[i];

		for (int j = 0; j < out->Connections.Count; j++)
		{
			if (out->Connections[j]->In == m_hotInput)
				continue;

			Input* in = out->Connections[j]->In;
			Operator* opIn = in->GetOperator();

			FPoint p0, p1, p2, p3;
			GetConnectionPoints(out, in, &p0, &p1, &p2, &p3);

			PaintConnection(
				msg.Graphics, 
				p0, 
				p1, 
				p2, 
				p3, 
				false);
		}
	}
}

void TreeView::PaintConnection(Graphics& g, FPoint p0, FPoint p1, FPoint p2, FPoint p3, bool hotOutput)
{
	double dist = Math::GetDistance(p0, p3);
	double factor = min(dist / 30, 1);
	factor = (1 - cos(factor * PI)) * 0.5;

	double cursorOffset = 14;

	if (hotOutput)
	{
		cursorOffset *= factor;

		// Make room for arrow
		p3.X -= cursorOffset;
		p2.X -= cursorOffset * 2 / 3;
		p1.X -= cursorOffset / 3;
	}

	FPoint po(0, 2);
	FPoint ps;
	Color cs(Color::Black, 0.1);
	double fo = 0.5;

	//ps = FPoint(-fo, -fo); g.DrawBezier(p0 + po + ps, p1 + po + ps, p2 + po + ps, p3 + po + ps, cs);
	//ps = FPoint(-fo,  fo); g.DrawBezier(p0 + po + ps, p1 + po + ps, p2 + po + ps, p3 + po + ps, cs);
	//ps = FPoint( fo, -fo); g.DrawBezier(p0 + po + ps, p1 + po + ps, p2 + po + ps, p3 + po + ps, cs);
	//ps = FPoint( fo,  fo); g.DrawBezier(p0 + po + ps, p1 + po + ps, p2 + po + ps, p3 + po + ps, cs);

	//g.DrawBezier(p0 + po, p1 + po, p2 + po, p3 + po, cs);
	
	/*	Draw connection. */

	//g.Subpixel = true;
	g.DrawBezier(p0, p1, p2, p3, Color::White);

	/*	Draw connected arrow. */

	g.DrawImage(
		(int)p3.X,
		(int)p3.Y - s_imgConnectedArrow.Height / 2,
		&s_imgConnectedArrow,
		hotOutput 
			? factor 
			: 1);
}

Object* TreeView::HitTestOp(const Point& p)
{
	FPoint fp((double)p.X, (double)p.Y);

	for (int i = m_tree->Count - 1; i >= 0; i--)
	{
		Operator* op = (*m_tree)[i];

		_Rect opRect = GetOpPaintRect(op);
		opRect.Position -= PaintRect.Position;

		if (opRect.Contains(p))
		{
			double padding = 4;

			/*	Test for inputs. */

			if (op->In.NewInput)
			{
				FRect inRect = GetInputRect(op->In.NewInput);
				
				inRect.Offset(FPoint(
					(double)opRect.X, 
					(double)opRect.Y));

				inRect.Left   -= padding;
				inRect.Right  += padding;
				inRect.Top    -= padding;
				inRect.Bottom += padding;

				if (inRect.Contains(fp))
					return op->In.NewInput;
			}
			
			for (int i = 0; i < op->In.Count; i++)
			{
				FRect inRect = GetInputRect(op->In[i]);
				
				inRect.Offset(FPoint(
					(double)opRect.X, 
					(double)opRect.Y));

				inRect.Left   -= padding;
				inRect.Right  += padding;
				inRect.Top    -= padding;
				inRect.Bottom += padding;

				if (inRect.Contains(fp))
					return op->In[i];
			}

			/*	Test for outputs. */

			for (int i = 0; i < op->Out.Count; i++)
			{
				FRect outRect = GetOutputRect(op->Out[i]);

				outRect.Offset(FPoint(
					(double)opRect.X, 
					(double)opRect.Y));

				outRect.Left   -= padding;
				outRect.Right  += padding;
				outRect.Top    -= padding;
				outRect.Bottom += padding;

				if (outRect.Contains(fp))
					return op->Out[i];
			}

			//

			return op;
		}
	}

	return null;
}

void TreeView::SelectAll()
{
	m_tree->Selected.Clear();

	for (int i = 0; i < m_tree->Count; i++)
		(*m_tree)[i]->IsSelected = true;

	Update();
}

void TreeView::DeselectAll()
{
	m_tree->Selected.Clear();
	Update();
}

void TreeView::DeleteSelected()
{
	Action::Perform(new ActDeleteOps(m_tree->Selected));
}

FRect TreeView::GetOpBounds(Array<Operator*>& ops)
{
	FRect bounds;

	for (int i = 0; i < ops.Count; i++)
	{
		assert(m_tree->Contains(ops[i]));

		FPoint p = ops[i]->Position;
		FSize sz = GetOpSize(ops[i]);
		
		bounds.Expand(FRect(
			p.X, p.Y,
			sz.Width, sz.Height));
	}

	return bounds;
}
