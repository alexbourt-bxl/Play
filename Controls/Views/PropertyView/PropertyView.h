#ifndef PROPERTY_VIEW_H
#define PROPERTY_VIEW_H

#include <Controls\Layout\LineLayout\LineLayout.h>
#include <Controls\Label\Label.h>
#include <Controls\Layout\Spacer\Spacer.h>

class PropertyContainer;

class PropertyView 
	: public LineLayout
{
	DECLARE_TYPE;

	typedef PropertyContainer _PropertyContainer;

public:
	PropertyView();
	~PropertyView();

	__declspec(property(get = GetPropertyContainer))
	_PropertyContainer* PropertyContainer;
	_PropertyContainer* GetPropertyContainer() const;
	void SetPropertyContainer(_PropertyContainer* propContainer, const String& containerName);

private:
	_PropertyContainer* m_propContainer;

	Label m_lblName;
	Spacer m_spacer;
};

#endif