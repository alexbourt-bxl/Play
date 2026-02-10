#ifndef PROPERTY_CONTAINER_H
#define PROPERTY_CONTAINER_H

#include <Types/Type.h>
#include <Containers/Array.h>

class Property;
class LineLayout;

class PropertyContainer
{
	DECLARE_ABSTRACT_TYPE;

	typedef LineLayout _LineLayout;

public:
	PropertyContainer();
	~PropertyContainer();

	__declspec(property(get = GetProperties))
	Array<Property*>& Properties;
	Array<Property*>& GetProperties();

	__declspec(property(get = GetLayout))
	_LineLayout* Layout;
	_LineLayout* GetLayout() const;

	virtual void LayoutControls(LineLayout* layout);

private:
	Array<Property*> m_properties;
	void properties_OnAdd(Container::Message& msg);

	LineLayout* m_layout;
	void CheckCreateLayout();
};

#endif