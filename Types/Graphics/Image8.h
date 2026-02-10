#ifndef IMAGE_8_H
#define IMAGE_8_H

#include <Types/Graphics\Image.h>
#include <Serialization\Serializable.h>

class Image8
	: public Image//, public Serializable
{
	DECLARE_TYPE;

public:
	Image8();
	Image8(const Image8& image);
	Image8(int width, int height);
	Image8(const _Size& size);
	~Image8();

	void		Init(int width, int height);
	Object*		Copy();

	Image*		ConvertTo(Type type);

	_Size		GetSize();
	void		SetSize(const _Size& size);

	void		Clear();

	__declspec(property(get = GetBuffer))
	byte*		Buffer;
	byte*		GetBuffer();

	//Data*		Serialize();
	//bool		Deserialize(Data* serialized);

private:
	_Size		m_size;

	uint		m_bufferSize;
	byte*		m_buffer;
};

#endif