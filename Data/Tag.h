#ifndef TAG_H
#define TAG_H

#include <Types/Basic/String.h>

class Tag
{
public:
	Tag();
	Tag(const String& name);
	Tag(const Tag& tag);

	__declspec(property(get = GetName, put = SetName))
	String& Name;
	String& GetName();
	void SetName(const String& name);

	__declspec(property(get = GetId))
	int Id;
	int GetId();

	Tag& operator =  (const Tag& tag);
	Tag& operator =  (const String& string);

	bool operator == (const Tag& tag);
	bool operator != (const Tag& tag);

private:
	String m_name;
	int m_id;

	int GetId(const String& string);

	/*	The Tag class keeps a hashtable of used tags
		and checks all new tags against it. */
	static const uint s_tableSize = 2048;
	static int s_tags[];

public:
	static Tag Null;

	static Tag Image;
	static Tag ImagePreview;
	static Tag ImageOffset;
};

#endif