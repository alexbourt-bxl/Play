#ifndef GDI_IMAGE_8_H
#define GDI_IMAGE_8_H

#include <Types/Graphics\GdiImage.h>
#include <Serialization\Serializable.h>

#include <windows.h>

class GdiImage8
	: public GdiImage//, public Serializable
{
	DECLARE_TYPE;

public:
	GdiImage8();
	GdiImage8(const GdiImage8& image);
	GdiImage8(uint width, uint height);
	GdiImage8(const _Size& size);
	GdiImage8(HBITMAP hbitmap);
	~GdiImage8();

	void		Init(int width, int height);
	Object*		Copy();

	Image*		ConvertTo(Type type);

	void		SetSize(const _Size& size);

	void		Clear();

	__declspec(property(get = GetBuffer))
	byte*		Buffer;
	byte*		GetBuffer();

	//Data*		Serialize();
	//bool		Deserialize(Data* serialized);

private:
	byte*		m_buffer;
};

#endif