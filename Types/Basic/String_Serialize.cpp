#include "String.h"

#include <assert.h>

//Data* String::Serialize()
//{
//	uint size = 0;
//
//	size += sizeof(Marker_PLAY);
//	size += sizeof(Marker_SERI);
//	size += sizeof(ushort); // type ID
//	size += sizeof(int); // length
//	size += sizeof(int); // buffer size
//	size += m_bufferSize;
//
//	Data* serialized = new Data(size);
//
//	uint pos = 0;
//
//	*((int*)(serialized->Buffer + pos)) = Marker_PLAY;			pos += sizeof(Marker_PLAY);
//	*((int*)(serialized->Buffer + pos)) = Marker_SERI;			pos += sizeof(Marker_SERI);
//	// TODO:
//	//*((ushort*)(serialized->Buffer + pos)) = typeof(this).Id;	pos += sizeof(ushort);
//	
//	*((int*)(serialized->Buffer + pos)) = m_length;				pos += sizeof(int);
//	*((int*)(serialized->Buffer + pos)) = m_bufferSize;			pos += sizeof(int);
//
//	memcpy_s(
//		serialized->Buffer + pos,
//		m_bufferSize,
//		m_buffer,
//		m_bufferSize);
//
//	return serialized;
//}
//
//bool String::Deserialize(Data* serialized)
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
//	// TODO:
//	//if (typeId != typeof(this).Id)
//	//	return false;
//
//	m_length = *((int*)(serialized->Buffer + pos));				pos += sizeof(int);
//	m_bufferSize = *((int*)(serialized->Buffer + pos));			pos += sizeof(int);
//
//	wchar_t* _buffer = (wchar_t*)realloc(m_buffer, m_bufferSize);
//
//	assert(_buffer);
//	m_buffer = _buffer;
//
//	memcpy_s(
//		m_buffer,
//		m_bufferSize,
//		serialized->Buffer + pos,
//		m_bufferSize);
//
//	return true;
//}