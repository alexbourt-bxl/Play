#include "Image8.h"

#include <stdlib.h>
#include <memory.h>
#include <assert.h>

DEFINE_TYPE(Image8, Image, Serializable);

Image8::Image8()
{
	Init(1, 1);
}

Image8::Image8(const Image8& _image) : Image(_image)
{
	Image8& image = (Image8&)_image;

	Init(image.Width, image.Height);
	memcpy_s(
		m_buffer, m_bufferSize, 
		image.m_buffer, image.m_bufferSize);
}

Image8::Image8(int width, int height)
{
	Init(width, height);
}

Image8::Image8(const _Size& size)
{
	Init(size.Width, size.Height);
}

Image8::~Image8()
{
	if (m_buffer)
		free(m_buffer);
}

void Image8::Init(int width, int height)
{
	m_size = _Size(width, height);
	m_bufferSize = m_size.Width * m_size.Height * 4;
	m_buffer = (byte*)malloc(m_bufferSize);
	memset(m_buffer, 0, m_bufferSize);
}

Object* Image8::Copy()
{
	return new Image8(*this);
}

Size Image8::GetSize()
{
	return m_size;
}

void Image8::SetSize(const _Size& size)
{
	/*	If the current size and the desired 
		size are the same, do nothing. */

	if (m_size == size)
		return;

	/*	Resize the buffer. */

	//Image8 copy(*this);

	uint _bufferSize = size.Width * size.Height * 4;
	byte* _buffer = (byte*)realloc(m_buffer, _bufferSize);
	
	assert(_buffer);

	m_buffer = _buffer;
	m_bufferSize = _bufferSize;
	m_size = size;
	
	//memset(m_buffer, 0, m_bufferSize);

	//uint* sourceBuffer = (uint*)copy.buffer;
	//uint* destBuffer = (uint*)buffer;

	//int minWidth = min(width, copy.width);
	//int minHeight = min(height, copy.height);

	//int ywidth, ywidthx;

	//for (int y = 0; y < minHeight; y++)
	//{
	//	ywidth = y * width;

	//	for (int x = 0; x < minWidth; x++)
	//	{
	//		ywidthx = ywidth + x;
	//		destBuffer[ywidthx] = sourceBuffer[ywidthx];
	//	}
	//}
}

void Image8::Clear()
{
	memset(m_buffer, 0, m_bufferSize);
}

byte* Image8::GetBuffer()
{
	return m_buffer;
}