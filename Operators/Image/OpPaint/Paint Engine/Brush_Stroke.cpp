#include "Brush.h"
//#include "Layer.h"
//#include "Canvas.h"
//#include "Action_Stroke.h"

//void Brush::MoveStroke(BrushStroke* stroke, Rect& updateRect)
//{
//	if (updateRect.IsNull)
//		return;
//
//	// First copy the current contents of the target space
//
//	//stroke->targetSpace->Lock();
//
//	//Space space(stroke->targetSpace->mode);
//	//Graphics gs(&space);
//
//	//gs.CopyImage(
//	//	updateRect.x,
//	//	updateRect.y,
//	//	stroke->targetSpace,
//	//	updateRect.x - stroke->targetSpace->offset.x,
//	//	updateRect.y - stroke->targetSpace->offset.y,
//	//	updateRect.width,
//	//	updateRect.height);
//
//	//stroke->targetSpace->Unlock();
//
//	// Now composite the stroke layers onto the temp space
//
//	//gs.BlendImage(
//	//	updateRect.x,
//	//	updateRect.y,
//	//	&stroke->spcErase,
//	//	updateRect.x,
//	//	updateRect.y,
//	//	updateRect.width,
//	//	updateRect.height,
//	//	1.0f,
//	//	Erase,
//	//	stroke->targetLayer
//	//		? stroke->targetLayer->lockedOpacity
//	//		: false);
//
//	//gs.BlendImage(
//	//	updateRect.x,
//	//	updateRect.y,
//	//	&stroke->space,
//	//	updateRect.x,
//	//	updateRect.y,
//	//	updateRect.width,
//	//	updateRect.height,
//	//	1.0f,
//	//	stroke->brush->blendMode,
//	//	stroke->targetLayer
//	//		? stroke->targetLayer->lockedOpacity
//	//		: false);
//
//	//UpdateEvent.Send(UpdateEvent(&space, updateRect));
//}
//
//void Brush::EndStroke(BrushStroke* stroke, Rect& updateRect)
//{
//	//if (updateRect.IsNull())
//	//	return;
//
//	//Space* spcResult = new Space(stroke->targetLayer->space.mode);
//	//Graphics g(spcResult);
//
//	//g.BlendImage(
//	//	updateRect.x,
//	//	updateRect.y,
//	//	&stroke->spcErase,
//	//	updateRect.x,
//	//	updateRect.y,
//	//	updateRect.width,
//	//	updateRect.height,
//	//	1.0f,
//	//	Erase);
//
//	//g.BlendImage(
//	//	updateRect.x,
//	//	updateRect.y,
//	//	&stroke->space,
//	//	updateRect.x,
//	//	updateRect.y,
//	//	updateRect.width,
//	//	updateRect.height,
//	//	1.0f,
//	//	stroke->brush->blendMode,
//	//	stroke->targetLayer
//	//		? stroke->targetLayer->lockedOpacity
//	//		: false);
//
//	//if (stroke->targetLayer)
//	//{
//	//	Action_Stroke* action = new Action_Stroke(
//	//		stroke->targetLayer,
//	//		spcResult,
//	//		updateRect);
//
//	//	stroke->targetLayer->canvas->actionManager.Perform(action);
//	//}
//}