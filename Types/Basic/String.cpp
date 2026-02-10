#include "String.h"

#include <Math\Math.h>
#include <System\System.h>

#include <stdlib.h>
#include <wchar.h>
#include <assert.h>

DEFINE_TYPE(String, /*Object,*/ Serializable);

String::String()
{
	m_bufferSize = 1 * sizeof(wchar_t);
	m_buffer = (wchar_t*)malloc(m_bufferSize);
	m_buffer[0] = L'\0';
	
	m_length = 0;
}

String::String(const String& s)
{
	m_length = wcslen(s.m_buffer);
	m_bufferSize = Math::GetNextPowerOf2((m_length + 1) * sizeof(wchar_t));
	m_buffer = (wchar_t*)malloc(m_bufferSize);

	wcscpy_s(
		m_buffer, 
		m_bufferSize / sizeof(wchar_t), 
		s.m_buffer);
}

String::String(const wchar_t* s)
{
	assert(s);

	m_length = wcslen(s);
	m_bufferSize = Math::GetNextPowerOf2((m_length + 1) * sizeof(wchar_t));
	m_buffer = (wchar_t*)malloc(m_bufferSize);

	wcscpy_s(
		m_buffer, 
		m_bufferSize / sizeof(wchar_t), 
		s);
}

String::String(int i)
{
	m_length = 
		i == 0 ? 1
		: Math::GetDigitCount(i) + (i < 0 ? 1 : 0);

	m_bufferSize = Math::GetNextPowerOf2((m_length + 1) * sizeof(wchar_t));
	m_buffer = (wchar_t*)malloc(m_bufferSize);

	swprintf_s(
		m_buffer, 
		m_bufferSize / sizeof(wchar_t), 
		L"%i",
		i);
}

String::~String()
{
	if (m_buffer)
		free(m_buffer);
}

//Object* String::Copy()
//{
//	return new String(*this);
//}

String::operator wchar_t*() const
{
	return m_buffer;
}

int String::IndexOf(wchar_t c)
{
	wchar_t* p = wcschr(m_buffer, c);

	return p
		? p - m_buffer
		: -1;
}

int String::IndexOf(const String& s)
{
	wchar_t* p = wcsstr(m_buffer, s);

	return p
		? p - m_buffer
		: -1;
}

int String::LastIndexOf(wchar_t c)
{
	wchar_t* p = wcsrchr(m_buffer, c);

	return p
		? p - m_buffer
		: -1;
}

String String::SubString(int start, int length)
{
	assert(start + length <= m_length);

	wchar_t* _sub = (wchar_t*)calloc(length + 1, sizeof(wchar_t));
	wcsncpy_s(
		_sub, 
		length + 1,
		m_buffer + start, 
		length);

	String sub(_sub);
	free(_sub);
	return sub;
}

String String::Left(int nChars)
{
	return SubString(0, nChars);
}

String String::Right(int nChars)
{
	return SubString(Length - nChars, nChars);
}

String String::ToLower()
{
	wchar_t* _s = _wcsdup(m_buffer);

	if (_s)
		_wcslwr_s(_s, wcslen(_s) + 1);
	
	String lower = _s;
	free(_s);

	return lower;
}

String String::ToUpper()
{
	wchar_t* _s = _wcsdup(m_buffer);
	
	if (_s)
		_wcsupr_s(_s, wcslen(_s) + 1);
	
	String upper = _s;
	free(_s);

	return upper;
}

bool String::Compare(const String& s, bool caseSensitive)
{
	if (caseSensitive)
		return wcscmp(m_buffer, s.m_buffer) == 0;
	else
		return _wcsicmp(m_buffer, s.m_buffer) == 0;
}

String String::FormatValue(double value, int nDecimals)
{
	wchar_t wcValue[64];

	// Use locale-specific decimal separator

	wchar_t decSeparator[5];
	GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, decSeparator, sizeof(decSeparator) / sizeof(wchar_t));

	if (nDecimals < 0)
	{
		_swprintf_s_l(
			wcValue, 
			sizeof(wcValue) / sizeof(wchar_t), 
			L"%f", 
			System::Locale, 
			value);

		// Trim the trailing zeroes

		int nChars = (int)wcslen(wcValue);

		while (nChars > 0 && wcValue[nChars - 1] == '0')
			wcValue[nChars-- - 1] = null;
	}
	else
	{
		wchar_t wcFormat[16];
		swprintf_s(
			wcFormat, 
			sizeof(wcFormat) / sizeof(wchar_t), 
			L"%%.0%if", 
			nDecimals);

		_swprintf_s_l(
			wcValue, 
			sizeof(wcValue) / sizeof(wchar_t), 
			wcFormat, 
			System::Locale, 
			value);

		wchar_t* dec = wcsstr(wcValue, decSeparator);
		
		if (dec)
		{
			dec += wcslen(decSeparator);

			int nChars = (int)wcslen(wcValue);
			int decPos = dec - wcValue;
			int decimalPlaces = min(nDecimals, nChars - decPos);

			for (int i = 0; i < decimalPlaces; i++)
				dec++;

			*dec = null;
		}
	}

	int nChars = (int)wcslen(wcValue);

	while (nChars > 0 && wcscmp(wcValue + nChars - 1, decSeparator) == 0)
		wcValue[nChars-- - 1] = null;

	/*	By trimming the trailing zeroes, the string's length will 
		jump around while adjusting the value, but this might be 
		a useful visual indication of whole numbers. */

	return String(wcValue);
}

wchar_t& String::operator [] (int index)
{
	assert(index < Length);
	return m_buffer[index];
}

//wchar_t& String::c(int index)
//{
//	assert(index < Length);
//	return m_buffer[index];
//}

String& String::operator = (const String& s)
{
	if (m_buffer) 
		free(m_buffer);
	
	m_length = s.m_length;
	m_bufferSize = Math::GetNextPowerOf2((m_length + 1) * sizeof(wchar_t));
	m_buffer = (wchar_t*)malloc(m_bufferSize);

	if (s.m_buffer)
		wcscpy_s(m_buffer, m_bufferSize / sizeof(wchar_t), s);

	return *this;
}

String& String::operator = (const wchar_t* s)
{
	if (m_buffer) 
		free(m_buffer);
	
	assert(s);

	m_length = wcslen(s);
	m_bufferSize = Math::GetNextPowerOf2((m_length + 1) * sizeof(wchar_t));
	m_buffer = (wchar_t*)malloc(m_bufferSize);

	wcscpy_s(m_buffer, m_bufferSize / sizeof(wchar_t), s);
	return *this;
}

bool String::operator == (const String& s)
{
	return wcscmp(m_buffer, s.m_buffer) == 0;
}

bool String::operator == (const wchar_t* s)
{
	assert(s);
	return wcscmp(m_buffer, s) == 0;
}

bool String::operator == (wchar_t* s)
{
	assert(s);
	return wcscmp(m_buffer, s) == 0;
}

bool String::operator != (const String& s)
{
	return !((*this) == s);
}

bool String::operator != (const wchar_t* s)
{
	return !((*this) == s);
}

bool String::operator != (wchar_t* s)
{
	return !((*this) == s);
}

String& String::operator += (const String& s)
{
	int slength = wcslen(s.m_buffer);

	if ((m_length + slength + 1) * 2 > m_bufferSize)
	{
		m_bufferSize = Math::GetNextPowerOf2((m_length + slength + 1) * 2);
		m_buffer = (wchar_t*)realloc(m_buffer, m_bufferSize);
	}

	wcscpy_s(
		m_buffer + m_length, 
		m_bufferSize / sizeof(wchar_t) - m_length, 
		s.m_buffer);

	m_length += slength;

	return *this;
}

String& String::operator += (const wchar_t* s)
{
	int slength = wcslen(s);

	if ((m_length + slength + 1) * 2 > m_bufferSize)
	{
		m_bufferSize = Math::GetNextPowerOf2((m_length + slength + 1) * 2);
		m_buffer = (wchar_t*)realloc(m_buffer, m_bufferSize);
	}

	wcscpy_s(
		m_buffer + m_length, 
		m_bufferSize / sizeof(wchar_t) - m_length, 
		s);

	m_length += slength;

	return *this;
}

String& String::operator += (const wchar_t c)
{
	if ((m_length + 1 + 1) * 2 > m_bufferSize)
	{
		m_bufferSize = Math::GetNextPowerOf2((m_length + 1 + 1) * 2);
		m_buffer = (wchar_t*)realloc(m_buffer, m_bufferSize);
	}

	wchar_t wc[2] = { c, null };

	wcscpy_s(
		m_buffer + m_length, 
		m_bufferSize / sizeof(wchar_t) - m_length, 
		wc);

	m_length++;

	return *this;
}

String String::operator + (const String& s)
{
	return String(*this) += s;
}

String String::operator + (const wchar_t* s)
{
	return String(*this) += s;
}

//

String String::Empty;