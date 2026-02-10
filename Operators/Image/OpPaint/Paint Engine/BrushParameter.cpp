#include "BrushParameter.h"
#include <limits>

BrushParameter::BrushParameter(/*UINT paramId*/) //:
	//: Object(Brush_Parameter_ID | paramId),
	//Value		(GetId() | Brush_Parameter_Value_ID),
	//Random		(GetId() | Brush_Parameter_Random_ID),
	//RandomScale	(GetId() | Brush_Parameter_RandomScale_ID),
	//Filter		(GetId() | Brush_Parameter_Filter_ID)
{
	//m_id = paramId;

	m_value		= 1;

	Random		= 0;
	RandomScale	= 1;
	Filter		= 0;
	RandomRange = 0;

	m_prevValue	= std::numeric_limits<double>::quiet_NaN();
}

BrushParameter::BrushParameter(const BrushParameter& param) //:
	//: Object(Brush_Parameter_ID | const_cast<BrushParameter&>(param).GetId()),
	//Value		(GetId() | Brush_Parameter_Value_ID),
	//Random		(GetId() | Brush_Parameter_Random_ID),
	//RandomScale	(GetId() | Brush_Parameter_RandomScale_ID),
	//Filter		(GetId() | Brush_Parameter_Filter_ID)
{
	//m_id = param.m_id;

	m_value		= param.m_value;

	Random		= param.Random;
	RandomScale	= param.RandomScale;
	Filter		= param.Filter;
	RandomRange	= param.RandomRange;

	m_prevValue	= std::numeric_limits<double>::quiet_NaN();
}

BrushParameter::BrushParameter(double value) : BrushValue(value)
	//: Object(Brush_Parameter_ID | paramId),
	//Value		(GetId() | Brush_Parameter_Value_ID),
	//Random		(GetId() | Brush_Parameter_Random_ID),
	//RandomScale	(GetId() | Brush_Parameter_RandomScale_ID),
	//Filter		(GetId() | Brush_Parameter_Filter_ID)
{
	Random		= 0;
	RandomScale	= 1;
	Filter		= 0;
	RandomRange = 0;

	m_prevValue	= std::numeric_limits<double>::quiet_NaN();
}

//UINT BrushParameter::GetId()
//{
//	return m_id;
//}

double BrushParameter::GetAdjusted(double pressure, double speed, double tilt)
{
	double value		= BrushValue::GetAdjusted(pressure, speed, tilt);
	
	double random		= Random.GetAdjusted(pressure, speed, tilt);
	double randomScale	= RandomScale.GetAdjusted(pressure, speed, tilt);
	double filter		= Filter.GetAdjusted(pressure, speed, tilt);
	
	double adjusted = value;

	// Random

	double randomSpan = random * (RandomRange != 0 ? RandomRange / 2 : adjusted);
	
	adjusted += -randomSpan / 2 + randomSpan * Noise.Next(randomScale);

	// Filter

	if (!_isnan(m_prevValue))
		adjusted = filter * m_prevValue + (1 - filter) * adjusted;

	m_prevValue = adjusted;

	return adjusted;
}

//int BrushParameter::GetSizeInBuffer()
//{
//	return
//		  sizeof(UINT) // ID
//		+ value.GetSizeInBuffer()
//		+ random.GetSizeInBuffer()
//		+ randomScale.GetSizeInBuffer()
//		+ filter.GetSizeInBuffer()
//		+ sizeof(randomRange);
//}
//
//void BrushParameter::WriteToBuffer(char* buffer, int* pos)
//{
//	// The noise seeds are not saved
//
//	*(UINT*)(buffer + *pos) = GetId();		*pos += sizeof(UINT); // ID
//
//	value.WriteToBuffer(buffer, pos);
//	random.WriteToBuffer(buffer, pos);
//	randomScale.WriteToBuffer(buffer, pos);
//	filter.WriteToBuffer(buffer, pos);
//
//	*(double*)(buffer + *pos) = randomRange;	*pos += sizeof(randomRange);
//}
//
//bool BrushParameter::ReadFromBuffer(char* buffer, int* pos)
//{
//	// The noise seeds are not loaded
//
//	UINT id	= *(UINT*)(buffer + *pos);	
//	
//	if ((GetId() & 0xFFFFFF00) != (id & 0xFFFFFF00))
//		return false;
//
//	id &= 0x000000FF;
//
//	if (id == 0x00)
//	{
//		*pos += sizeof(UINT); // ID
//
//		value.ReadFromBuffer(buffer, pos);
//		random.ReadFromBuffer(buffer, pos);
//		randomScale.ReadFromBuffer(buffer, pos);
//		filter.ReadFromBuffer(buffer, pos);
//
//		randomRange = *(double*)(buffer + *pos);	*pos += sizeof(randomRange);
//
//		return true;
//	}
//	else if (id == Brush_Parameter_Value_ID)		{ value.ReadFromBuffer(buffer, pos);		return true; }
//	else if (id == Brush_Parameter_Random_ID)		{ random.ReadFromBuffer(buffer, pos);		return true; }
//	else if (id == Brush_Parameter_RandomScale_ID)	{ randomScale.ReadFromBuffer(buffer, pos);	return true; }
//	else if (id == Brush_Parameter_Filter_ID)		{ filter.ReadFromBuffer(buffer, pos);		return true; }
//	else if (id == Brush_Parameter_RandomRange_ID) 
//	{ 
//		*pos += sizeof(UINT); // ID
//		randomRange = *(double*)(buffer + *pos); *pos += sizeof(randomRange); 
//		return true; 
//	}
//
//	return false;
//}