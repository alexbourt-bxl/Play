#include "BrushControl.h"

//#include "Brush.h"
//#include "BrushStroke.h"
//#include "Animation.h"
//#include "PaintTool.h"
//
//void BrushControl::value_OnChange(Message msg)
//{
//	BrushControl* control = (BrushControl*)msg.object;
//	control->Update();
//}
//
//void BrushControl::OnPaint(Control::PaintEvent msg)
//{
//	Rect rect = GetRectForPaint();
//	Rect paintRect = GetPaintRect();
//
//	Region rgnOld = *msg.graphics->GetClipRegion();
//
//	Region rgn(Rect(
//		paintRect.x,
//		rect.y + ClipOffsetY,
//		paintRect.width,
//		rect.height - ClipOffsetY));
//
//	msg.graphics->SetClipRegion(&rgn);
//
//	if (stroke)
//	{
//		//g.DrawImage(
//		//	0,
//		//	0,
//		//	imgStroke->GetSize().width,
//		//	imgStroke->GetSize().height,
//		//	&stroke.spcErase,
//		//	0,
//		//	0,
//		//	imgStroke->GetSize().width,
//		//	imgStroke->GetSize().height);
//
//		msg.graphics->DrawImage(
//			paintRect.x, // - (int)((max(0.0f, Selected - 0.5f) * 2.0f * 1.0f) + 0.5f),
//			paintRect.y,
//			paintRect.width,
//			paintRect.height,
//			&stroke->space,
//			-paintRect.width / 2 + 2,
//			5 + (int)(max(0.0f, Selected - 0.5f) * 2.0f * 6.25f),
//			paintRect.width,
//			paintRect.height,
//			(0.65f + (0.35f * Selected)) * StrokeOpacity * (1.0f - Deleted));
//	}
//
//	rgn = Region(Rect(
//		paintRect.x,
//		rect.y,
//		paintRect.width,
//		rect.height + ClipOffsetY));
//
//	msg.graphics->SetClipRegion(&rgn);
//
//	if (imgBrush)
//	{
//		Size size = imgBrush->GetSize();
//
//		if (canDelete)
//		{
//			size.width = (int)((float)size.width / (1.0f + Deleted));
//			size.height = (int)((float)size.height / (1.0f + Deleted));
//		}
//
//		msg.graphics->DrawImage(
//			rect.x + rect.width / 2 - size.width / 2,
//			rect.y + rect.height / 2 - size.height / 2 + 45 - (int)(35.0f * Selected),
//			size.width,
//			size.height,
//			imgBrush,
//			0,
//			0,
//			imgBrush->GetSize().width,
//			imgBrush->GetSize().height,
//			(0.5f + (0.5f * Selected)));
//	}
//
//	if (canDelete)
//	{
//		msg.graphics->DrawImage(
//			rect.x + (rect.width - s_imgDelete.GetSize().width) / 2,
//			rect.y + (rect.height - s_imgDelete.GetSize().height) * 2 / 3,
//			&s_imgDelete,
//			0.5f * Deleted);
//	}
//
//	msg.graphics->SetClipRegion(&rgnOld);
//}
//
//void BrushControl::OnMouseEnter(Mouse::Message /*msg*/)
//{
//	PaintTool* paintTool = (PaintTool*)brush->data[0];
//
//	if (brush != paintTool->m_curBrush)
//	{
//		FloatAnimation* anim = new FloatAnimation();
//			anim->value = &Selected;
//			anim->to = 0.5f;
//			anim->duration = 0.1f;
//			anim->curve = Animation::EaseIn;
//
//		anim->Start();
//	}
//}
//
//void BrushControl::OnMouseLeave(Mouse::Message /*msg*/)
//{
//	PaintTool* paintTool = (PaintTool*)brush->data[0];
//
//	if (brush != paintTool->m_curBrush)
//	{
//		FloatAnimation* anim = new FloatAnimation();
//			anim->value = &Selected;
//			anim->to = 0.0f;
//			anim->duration = 0.1f;
//			anim->curve = Animation::EaseOut;
//
//		anim->Start();
//	}
//}
//
//void BrushControl::OnLButtonDown(Mouse::Message msg)
//{
//	PaintTool* paintTool = (PaintTool*)brush->data[0];
//
//	if (paintTool->m_curBrush)
//	{
//		FloatAnimation* anim = new FloatAnimation();
//			anim->value = &paintTool->m_curBrush->control->Selected;
//			anim->to = 0.0f;
//			anim->duration = 0.1f;
//			anim->curve = Animation::EaseOut;
//
//		anim->Start();
//	}
//
//	if ((int)msg.data[0] == 1 // stylus
//		&& (int)msg.data[1] == 2) // eraser
//		paintTool->m_curEraserIndex = paintTool->GetBrushIndex(brush);
//	else // stylus tip or mouse
//		paintTool->m_curTipIndex = paintTool->GetBrushIndex(brush);
//	
//	paintTool->SelectBrush(brush);
//
//	FloatAnimation* anim = new FloatAnimation();
//		anim->value = &Selected;
//		anim->to = 1.0f;
//		anim->duration = 0.1f;
//		anim->curve = Animation::EaseIn;
//
//	anim->Start();
//
//	m_pressed = true;
//
//	Point p = Mouse::GetPosition();
//
//	m_posStart = p;
//	m_posOffset = p - GetPosition();
//}
//
//void BrushControl::OnMouseMove(Mouse::Message msg)
//{
//	// BUG: brush flickers if you just hold it outside of the palette... something is incorrectly
//	// updating it (when it shouldn't be).
//
//	PaintTool* paintTool = (PaintTool*)brush->data[0];
//
//	if (m_pressed)
//	{
//		Point p = Mouse::GetPosition();
//
//		if (!paintTool->draggedBrushControl 
//			&& (abs(p.x - m_posStart.x) > 6 || m_posStart.y - p.y > 6))
//		{
//			parent->controls.SetIndexOf(this, parent->controls.count - 1);
//			paintTool->draggedBrushControl = this;
//		}
//
//		if (paintTool->draggedBrushControl == this)
//		{
//			// First handle the border condition
//
//			Rect paletteRect = paintTool->m_pnlBrushes.GetClientRect();
//			paintTool->m_pnlBrushes.RectToScreen(&paletteRect);
//
//			if (!m_markedForDelete) // inside the palette
//			{
//				if (!paletteRect.Contains(p)) // moved outside
//				{
//					// Check that this brush is not the last of its kind
//					// and can be deleted.
//
//					canDelete = false;
//
//					for (int i = 0; i < paintTool->m_nBrushes; i++)
//					{
//						if (paintTool->m_brushes[i] != brush
//							&& paintTool->m_brushes[i]->type == brush->type)
//						{
//							canDelete = true;
//							break;
//						}
//					}
//
//					if (canDelete)
//						BreakOutOfPalette();
//				} // now it's outside
//			}
//			else // outside the palette
//			{
//				if (paletteRect.Contains(p)) // moved inside
//				{
//					if (canDelete)
//						BreakIntoPalette();
//				} // now it's inside
//			}
//
//			// Now handle the control's position
//			// whether it is inside or outside
//
//			//Rect trailRect = GetPaintRect();
//			//trailRect.height += 30;
//			//trail.Add(trailRect);
//			
//			SuspendUpdates();
//			{
//				SetPosition(p - m_posOffset);
//				
//				if (!m_markedForDelete) // inside
//				{
//					int newY = min(0, p.y - m_posOffset.y + 30);
//					
//					StrokeOpacity = 1.0f - min(max(0.0f, newY / -30.0f), 1.0f); 
//					Y = newY;
//
//					ClipOffsetY = -Y;
//
//					// Handle the control sticking out a little bit
//					// before it has been broken out of the palette.
//
//					parent->paintRectOffset.x = 0;
//					parent->paintRectOffset.width = 0;
//
//					if (X < 0)
//					{
//						parent->paintRectOffset.x = X;
//						parent->paintRectOffset.width = -X;
//					}
//					else if (X + Width > parent->GetClientRect().width)
//					{
//						parent->paintRectOffset.width = X + Width - parent->GetClientRect().width;
//					}
//
//					parent->paintRectOffset.y = Y;
//					parent->paintRectOffset.height = -Y;
//				}
//			}
//			ResumeUpdates(true);
//
//			// Adjust the palette
//
//			if (!m_markedForDelete)
//			{
//				int emptyIndex = min(max(0, (X + 22 - 11 + parent->storePoint.x) / 44), paintTool->GetBrushCount() - 1);
//
//				if (emptyIndex != paintTool->emptyBrushIndex)
//				{
//					paintTool->SetBrushIndex(brush, emptyIndex);
//					paintTool->emptyBrushIndex = emptyIndex;
//
//					paintTool->UpdateBrushPalette();
//				}
//			}
//		}
//	}
//	else if (parent)
//	{
//		parent->MouseMoveEvent.Send(Mouse::Message(
//			parent,
//			msg.position + GetPosition(), 
//			msg.prevPosition + GetPosition()));
//	}
//}
//
//void BrushControl::OnLButtonUp(Mouse::Message /*msg*/)
//{
//	if (m_pressed)
//	{
//		PaintTool* paintTool = (PaintTool*)brush->data[0];
//
//		if (m_markedForDelete)
//		{
//			if (canDelete)
//			{
//				FloatAnimation* anim = new FloatAnimation();
//					anim->value = &Opacity;
//					anim->to = 0.0f;
//					anim->duration = 0.2f;
//					anim->data[0] = this;
//					anim->EndEvent += EventHandler(this, &BrushControl::DeleteBrush);
//				anim->Start();
//			}
//			else
//			{
//				BreakIntoPalette();
//
//				Point po(0, 0);
//				paintTool->m_pnlBrushes.PointToWindow(&po);
//
//				SuspendUpdates();
//				{
//					X -= po.x;
//					Y -= po.y;
//				}
//				ResumeUpdates();
//
//				paintTool->draggedBrushControl = null;
//				paintTool->UpdateBrushPalette();
//			}
//		}
//		else
//		{
//			paintTool->draggedBrushControl = null;
//
//			int index = min(max(0, (X + 22 - 11 + parent->storePoint.x) / 44), paintTool->GetBrushCount() - 1);
//
//			paintTool->SetBrushIndex(brush, index);
//			paintTool->emptyBrushIndex = -1;
//			paintTool->UpdateBrushPalette();
//		}
//	}
//
//	m_pressed = false;
//}
//
//void BrushControl::DeleteBrush(AnimationMessage msg)
//{
//	BrushControl* brushControl = (BrushControl*)msg.animation->data[0];
//	PaintTool* paintTool = (PaintTool*)brushControl->brush->data[0];
//
//	paintTool->draggedBrushControl = null;
//	paintTool->m_pnlBrushes.window->dragged.Remove(brushControl);
//
//	paintTool->SelectBrush(paintTool->m_prevBrush);
//	paintTool->UpdateBrushPalette();
//
//	brushControl->Uncapture();
//	brushControl->LoseFocus();
//
//	Brush* _brush = brushControl->brush;
//	delete _brush;
//}