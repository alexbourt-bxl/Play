#ifndef STRING_H
#define STRING_H

#include <Types/Object.h>
#include <Serialization\Serializable.h>

//#define TEST

/*
	A Play string can contain formatting as well as character data.
*/

class String
	//: /*public Object,*/ public Serializable
{
	DECLARE_TYPE;

public:
	String();
	String(const String& s);
	String(const wchar_t* s);
	String(int i);
	~String();

	//Object* Copy();

	__declspec(property(get = GetLength)) 
	int Length;
	inline int GetLength() { return m_length; }

	__declspec(property(get = GetSizeInBytes)) 
	int SizeInBytes;
	inline int GetSizeInBytes() { return m_length * sizeof(wchar_t); }

	operator wchar_t*() const;

	int		IndexOf(wchar_t c);
	int		IndexOf(const String& s);

	int		LastIndexOf(wchar_t c);

	String	SubString(int start, int length);
	String	Left(int nChars);
	String	Right(int nChars);
	
	String	ToLower();
	String	ToUpper();

	bool	Compare(const String& s, bool caseSensitive = true);

	static String FormatValue(double value, int nDecimals = -1);

	wchar_t& operator [] (int index);

	String&	operator =  (const String& s);
	String&	operator =  (const wchar_t* s);

	bool	operator == (const String& s);
	bool	operator == (const wchar_t* s);
	bool	operator == (wchar_t* s);
	bool	operator != (const String& s);
	bool	operator != (const wchar_t* s);
	bool	operator != (wchar_t* s);

	String&	operator += (const String& s);
	String& operator += (const wchar_t* s);
	String& operator += (const wchar_t c);

	String	operator +  (const String& s);
	String	operator +  (const wchar_t* s);

	//Data*	Serialize();
	//bool	Deserialize(Data* serialized);

private:
	int	m_bufferSize;
	wchar_t* m_buffer;
	int	m_length;

public:
	static String Empty;
};

#endif