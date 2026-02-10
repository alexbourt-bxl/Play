#include "Brush.h"

//#include "BrushControl.h"

//int Brush::GetSizeInBuffer()
//{
//	int size = 
//		  sizeof(UINT) // ID
//
//		+ sizeof(int)  // type
//		+ sizeof(int)  // colorMixMode
//		+ sizeof(int);  // blendMode
//		
//	List::Node* p = m_paramList.first; while (p)
//	{
//		size += ((BrushParameter*)p->data)->GetSizeInBuffer();
//		p = p->next;
//	}
//
//	size +=
//		  sizeof(antialias)
//		+ sizeof(strokeFilter)
//		+ sizeof(speedFilter)
//		+ sizeof(pressureCurve);
//
//	return size;
//}
//
//void Brush::WriteToBuffer(char* buffer, int* pos)
//{
//	*(UINT*)(buffer + *pos) = /* ID */ 0x0000;	*pos += sizeof(UINT); // 0x0000 means whole brush
//
//	*(int*)(buffer + *pos) = (int)type;			*pos += sizeof(int);
//	*(int*)(buffer + *pos) = (int)colorMixMode;	*pos += sizeof(int);
//	*(int*)(buffer + *pos) = (int)blendMode;	*pos += sizeof(int);
//
//	List::Node* p = m_paramList.first; while (p)
//	{
//		((BrushParameter*)p->data)->WriteToBuffer(buffer, pos);
//		p = p->next;
//	}
//
//	*(float*)(buffer + *pos) = antialias;			*pos += sizeof(antialias);
//	*(float*)(buffer + *pos) = strokeFilter;		*pos += sizeof(strokeFilter);
//	*(float*)(buffer + *pos) = speedFilter;			*pos += sizeof(speedFilter);
//	*(float*)(buffer + *pos) = pressureCurve;		*pos += sizeof(pressureCurve);
//}
//
//bool Brush::ReadFromBuffer(char* buffer, int* pos)
//{
//	UINT id	= *(UINT*)(buffer + *pos) & 0xFF00;
//
//	if (id == 0x0000)
//	{
//		*pos += sizeof(UINT); // ID
//
//		type		 = (BrushType)*(int*)(buffer + *pos);		*pos += sizeof(int);
//		colorMixMode = (ColorMixMode)*(int*)(buffer + *pos);	*pos += sizeof(int);
//		blendMode	 = (BlendMode)*(int*)(buffer + *pos);		*pos += sizeof(int);
//
//		if (control)
//			control->SetBrush(this); // this call sets the appropriate type image and color
//
//		List::Node* p = m_paramList.first; while (p)
//		{
//			((BrushParameter*)p->data)->ReadFromBuffer(buffer, pos);
//			p = p->next;
//		}
//
//		antialias		= *(float*)(buffer + *pos);		*pos += sizeof(antialias);
//		strokeFilter	= *(float*)(buffer + *pos);		*pos += sizeof(strokeFilter);
//		speedFilter		= *(float*)(buffer + *pos);		*pos += sizeof(speedFilter);
//		pressureCurve	= *(float*)(buffer + *pos);		*pos += sizeof(pressureCurve);
//
//		return true;
//	}
//
//	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	// Spesnific parameters
//
//	else if (id == 0x0100)	
//	{ 
//		*pos += sizeof(UINT);	
//		type = (BrushType)*(int*)(buffer + *pos);		*pos += sizeof(int);
//
//		if (control)
//			control->SetBrush(this); // this call sets the appropriate type image and color
//
//		return true;
//	}
//	else if (id == Brush_Parameter_ColorMixMode_ID)  { colorMixMode	= (ColorMixMode)*(int*)(buffer + *pos);	*pos += sizeof(int);   return true; }
//	else if (id == Brush_Parameter_BlendMode_ID)	 { blendMode	= (BlendMode)*(int*)(buffer + *pos);	*pos += sizeof(int);   return true; }
//	else if (id == Brush_Parameter_Antialias_ID)	 { antialias	= *(float*)(buffer + *pos);				*pos += sizeof(float); return true; }
//	else if (id == Brush_Parameter_StrokeFilter_ID)  { strokeFilter	= *(float*)(buffer + *pos);				*pos += sizeof(float); return true; }
//	else if (id == Brush_Parameter_SpeedFilter_ID)	 { speedFilter	= *(float*)(buffer + *pos);				*pos += sizeof(float); return true; }
//	else if (id == Brush_Parameter_PressureCurve_ID) { pressureCurve= *(float*)(buffer + *pos);				*pos += sizeof(float); return true; }
//
//	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	// Normal brush parameters
//
//	else
//	{
//		List::Node* p = m_paramList.first; while (p)
//		{
//			BrushParameter* param = (BrushParameter*)p->data;
//			if (id == param->GetId()) return param->ReadFromBuffer(buffer, pos);
//			p = p->next;
//		}
//	}
//
//	return false;
//}