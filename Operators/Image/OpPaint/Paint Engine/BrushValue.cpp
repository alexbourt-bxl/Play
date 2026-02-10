#include "BrushValue.h"

BrushValue::BrushValue(/*UINT id*/)
	//: Object(id)
{
	m_value  = 0;
	Heavy    = 1;
	Light    = 1;
	Slow     = 1;
	Fast     = 1;
	Vertical = 1;
	Tilted   = 1;
}

BrushValue::BrushValue(const BrushValue& val)
	//: Object(const_cast<BrushValue&>(val).GetId())
{
	m_value	 = val.m_value;
	Heavy 	 = val.Heavy;
	Light 	 = val.Light;
	Slow  	 = val.Slow;
	Fast  	 = val.Fast;
	Vertical = val.Vertical;
	Tilted   = val.Tilted;
}

BrushValue::BrushValue(double value)
	//: Object(id)
{
	m_value  = value;
	Heavy    = 1;
	Light    = 1;
	Slow     = 1;
	Fast     = 1;
	Vertical = 1;
	Tilted   = 1;
}

BrushValue::operator double()
{
	return m_value;
}

BrushValue& BrushValue::operator = (double value)
{
	m_value = value;
	return *this;
}

double BrushValue::GetAdjusted(double pressure, double speed, double tilt)
{
	double adjusted = m_value;

	adjusted *= Heavy * pressure + Light * (1 - pressure);
	adjusted *= Fast * speed + Slow * (1 - speed);
	adjusted *= Tilted * tilt + Vertical * (1 - tilt);

	return adjusted;
}

//int BrushValue::GetSizeInBuffer()
//{
//	return
//		  sizeof(UINT) // ID
//		+ sizeof(set)
//		+ sizeof(heavy)
//		+ sizeof(light)
//		+ sizeof(slow)
//		+ sizeof(fast)
//		+ sizeof(vertical)
//		+ sizeof(tilted);
//}
//
//void BrushValue::WriteToBuffer(char* buffer, int* pos)
//{
//	*(UINT*)(buffer + *pos) = GetId();		*pos += sizeof(UINT); // ID
//	*(double*)(buffer + *pos) = set;			*pos += sizeof(set);
//	*(double*)(buffer + *pos) = heavy;		*pos += sizeof(heavy);
//	*(double*)(buffer + *pos) = light;		*pos += sizeof(light);
//	*(double*)(buffer + *pos) = slow;		*pos += sizeof(slow);
//	*(double*)(buffer + *pos) = fast;		*pos += sizeof(fast);
//	*(double*)(buffer + *pos) = vertical;	*pos += sizeof(vertical);
//	*(double*)(buffer + *pos) = tilted;		*pos += sizeof(tilted);
//}
//
//bool BrushValue::ReadFromBuffer(char* buffer, int* pos)
//{
//	UINT id	= *(UINT*)(buffer + *pos);	
//	
//	if ((GetId() & 0x000000FF) != (id & 0x000000FF))
//		return false;
//	else									*pos += sizeof(UINT); // ID
//	set		= *(double*)(buffer + *pos); 	*pos += sizeof(set);
//	heavy	= *(double*)(buffer + *pos); 	*pos += sizeof(heavy);
//	light	= *(double*)(buffer + *pos); 	*pos += sizeof(light);
//	slow	= *(double*)(buffer + *pos); 	*pos += sizeof(slow);
//	fast	= *(double*)(buffer + *pos); 	*pos += sizeof(fast);
//	vertical= *(double*)(buffer + *pos);		*pos += sizeof(vertical);
//	tilted	= *(double*)(buffer + *pos); 	*pos += sizeof(tilted);
//
//	return true;
//}