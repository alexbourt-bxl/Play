#include "GdiImage8.h"

#include <stdlib.h>

//Data* GdiImage8::Serialize()
//{
//	BITMAP b;
//	::GetObject(m_hbitmap, sizeof(b), &b);
//
//	uint bufferSize = b.bmWidth * b.bmHeight * 4;
//
//	uint size = 0;
//
//	size += sizeof(Marker_PLAY);
//	size += sizeof(Marker_SERI);
//	size += sizeof(ushort); // type ID
//	size += sizeof(int) * 2; // width, height
//
//	size += bufferSize;
//
//	Data* serialized = new Data(size);
//
//	uint pos = 0;
//
//	*((int*)(serialized->Buffer + pos)) = Marker_PLAY;				pos += sizeof(Marker_PLAY);
//	*((int*)(serialized->Buffer + pos)) = Marker_SERI;				pos += sizeof(Marker_SERI);
//	*((ushort*)(serialized->Buffer + pos)) = TYPEOF(GdiImage8).Id;	pos += sizeof(ushort);
//
//	*((int*)(serialized->Buffer + pos)) = b.bmWidth;				pos += sizeof(int);
//	*((int*)(serialized->Buffer + pos)) = b.bmHeight;				pos += sizeof(int);
//
//	memcpy_s(
//		serialized->Buffer + pos,
//		bufferSize,
//		m_buffer,
//		bufferSize);
//
//	return serialized;
//}
//
//bool GdiImage8::Deserialize(Data* serialized)
//{
//	assert(serialized);
//
//	/*	Check that the data is large enough
//	to contain the two necessary markers. */
//	if (serialized->Size < 10)
//		return false;
//
//	uint pos = 0;
//
//	uint marker_PLAY = *((uint*)(serialized->Buffer + pos));	pos += sizeof(Marker_PLAY);
//
//	if (marker_PLAY != Marker_PLAY)
//		return false;
//
//	uint marker_SERI = *((uint*)(serialized->Buffer + pos));	pos += sizeof(Marker_SERI);
//
//	if (marker_SERI != Marker_SERI)
//		return false;
//
//	ushort typeId = *((ushort*)(serialized->Buffer + pos));		pos += sizeof(ushort);
//
//	if (typeId != typeof(this).Id)
//		return false;
//
//	int width  = *((int*)(serialized->Buffer + pos));			pos += sizeof(int);
//	int height = *((int*)(serialized->Buffer + pos));			pos += sizeof(int);
//
//	SetSize(_Size(width, height));
//
//	uint bufferSize = width * height * 4;
//
//	memcpy_s(
//		m_buffer,
//		bufferSize,
//		serialized->Buffer + pos,
//		bufferSize);
//
//	return true;
//}