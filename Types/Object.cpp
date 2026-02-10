#include "Object.h"

#include <Containers/Array.h>
//#include <Types/Basic/String.h>

uint Object::s_lastIssuedId = 0;

//////////////////////////////////////////////////////////////////////////////////

__Type* Object::s_type = __Type::Register(L"Object", null, null);

__Type* Object::GetClassType() 
{ 
	return s_type; 
}

Object* Object::Create(Type type) 
{	
	return type == TYPEOF(Object)
		? (Object*)type.CreateObject()
		: null; 
}

//////////////////////////////////////////////////////////////////////////////////

Object::Object() : m_refCount(0)
{
	m_id = ++s_lastIssuedId; //GetPool().GetNextId();
	//m_owner = null;
	//m_name = null;

	//GetPool().Add(this);
}

Object::~Object()
{
	//GetPool().DisableEvents();
	//GetPool().Remove(this);
	//GetPool().EnableEvents();
	
	//if (m_name)
	//	delete m_name;
}

//uint Object::GetId()
//{
//	return m_id;
//}

//void* Object::GetOwner()
//{
//	return m_owner;
//}
//
//void Object::SetOwner(void* owner)
//{
//	m_owner = owner;
//}
//
//String& Object::GetName()
//{
//	if (!m_name)
//		m_name = new String();
//
//	return *m_name;
//}

//void Object::SetName(const String& name)
//{
//	if (!m_name)
//		m_name = new String();
//
//	*m_name = name;
//}

//void Object::MarkForDelete()
//{
//	GetPool().Lock();
//
//	GetPool().m_deleteQueue.Add(this);
//
//	GetPool().Unlock();
//	GetPool().WakeUp();
//}

//Object* Object::GetById(uint objId)
//{
//	return GetPool().Get(objId);
//}

//ObjectPool& Object::GetPool()
//{
//	static ObjectPool pool;
//	return pool;
//}