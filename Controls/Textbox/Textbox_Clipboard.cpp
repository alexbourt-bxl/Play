#include "Textbox.h"

#include <Clipboard\Clipboard.h>

void Textbox::CutSelection()
{
	if (CopySelection())
		DeleteSelection();
}

bool Textbox::CopySelection()
{
	if (m_selLength == 0)
		return false;

	int selStart = min(m_selStart, m_selStart + m_selLength);
	int selEnd = max(m_selStart, m_selStart + m_selLength);
	int selLength = selEnd - selStart;

	wchar_t* copy = (wchar_t*)calloc(selLength + 1, sizeof(wchar_t));
	wcsncpy_s(copy, selLength + 1, (wchar_t*)m_text + selStart, selLength);

	String text = copy;

	free(copy);

	return false; // TODO: return Clipboard::Set(&text);
}

void Textbox::PasteSelection()
{
	String* text = (String*)Clipboard::Get(TYPEOF(String));

	if (!text)
		return;
	
	Insert(*text);

	delete text;
}