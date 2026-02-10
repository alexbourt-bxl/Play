#ifndef TYPE_H
#define TYPE_H

#include <Types/BaseTypes.h>

/*	TODO:

	Different instances of Play (especially across networks) will probably
	have to negotiate type lists to make sure that type IDs are identical.

	Otherwise a better method for generating unique non-conflicting type IDs will be necessary. 
*/

/*	Because of the circular dependency, this class 
	cannot use Strings, but must instead use wchar_t*. */

class __Type;
typedef __Type& Type;

class String;
template <class Class> class Array;
class List;

class __Type
{
	friend class OpList;

public:
	typedef void* (*CreateProc)();

private:
	__Type(const wchar_t* name, ushort id, CreateProc createProc);
	~__Type();

public:
	ushort GetId();

	__declspec(property(get = GetName))
	String Name;
	String GetName();

	__declspec(property(get = GetDisplayName))
	String DisplayName;
	String GetDisplayName();
	int    SetDisplayName(const String& displayName);

	__declspec(property(get = GetIsAbstract))
	bool IsAbstract;
	bool GetIsAbstract();

	__declspec(property(get = GetCreateProc))
	CreateProc CreateObject;
	CreateProc GetCreateProc();

	__declspec(property(get = GetBases))
	Array<__Type*>& Bases;
	Array<__Type*>& GetBases();

	bool operator == (Type type);
	bool operator != (Type type);

	operator __Type*();

	/*	Any class that derives from Object must register its type by calling 
		__Type::Register(), which returns the newly registered type. */
	static __Type* Register(const wchar_t* typeName, CreateProc createProc, ...);

	static Type GetFromId(ushort id);
	static Array<__Type*> GetByType(Type type);

private:
	ushort	 m_id;

	wchar_t* m_name;
	wchar_t* m_displayName;

	/*	This method uses wchar_t* because
		it deals with the individual bytes. */
	static ushort GenerateId(const wchar_t* typeName);

	/*	A type must be able to create instances of itself. 
		Each type therefore contains a pointer to a static 
		function which creates an instance of the type. */
	CreateProc m_createProc;

	/*	A type must know all the types that it's derived 
		from to allow full type comparisons. */
	Array<__Type*>* m_bases;

	/*	The Type class keeps a hashtable of registered 
		types, and checks all new types against it. */
	static const uint s_tableSize = 2048;
	static __Type* s_registeredTypes[];

	/*	The Type class uses a holding queue to store incomplete
		types (whose base classes haven't been registered yet). */
	static List s_incompleteQueue;

	static void ResolveIncomplete(__Type* type);

public:
	static __Type Null;
};

//////////////////////////////////////////////////////////////////////////////////

/*	Workaround for VC++ bug (__VA_ARGS__ treated as a single token) 
	(taken from http://www.gamedev.net/community/forums/viewreply.asp?ID=3631924).
	(I don't understand why this works.) */

#define _L_ (
#define _R_ )

/*	A method of getting the number of arguments in __VA_ARGS__
	(taken from http://groups.google.com/group/comp.std.c/browse_thread/thread/77ee8c8f92e4a3fb/346fc464319b1ee5)
	Modified to work with an empty argument list. */

#define __VA_NARG__(...) __VA_NARG_NUM _L_ ~, ##__VA_ARGS__, __VA_NARG_RET _R_
#define __VA_NARG_NUM(_, _1, _2, _3, _4, _5, _6, _7, _8, n, ...) n 
#define __VA_NARG_RET 8, 7, 6, 5, 4, 3, 2, 1, 0 // up to 8, can be expanded to 63 (C++ max)

/*	A method for iterating over individual arguments in __VA_ARGS__
	(taken from http://stackoverflow.com/questions/1872220/is-it-possible-to-iterate-over-arguments-in-variadic-macros/1872506#1872506)
	Modified to work with an empty argument list. */

#define __CONCAT(a1, a2) a1##a2
#define DEFINE_BASE(...) DEFINE_BASE_N _L_ __VA_NARG__(__VA_ARGS__), ~, ##__VA_ARGS__ _R_ null
#define DEFINE_BASE_N(n, _, ...) __CONCAT(DEFINE_BASE_, n) _L_ ~, ##__VA_ARGS__ _R_

#define DEFINE_BASE_0(_           )
#define DEFINE_BASE_1(_, Base     ) L#Base, Base::GetClassType(),
#define DEFINE_BASE_2(_, Base, ...) L#Base, Base::GetClassType(), DEFINE_BASE_1 _L_ ~, __VA_ARGS__ _R_
#define DEFINE_BASE_3(_, Base, ...) L#Base, Base::GetClassType(), DEFINE_BASE_2 _L_ ~, __VA_ARGS__ _R_
#define DEFINE_BASE_4(_, Base, ...) L#Base, Base::GetClassType(), DEFINE_BASE_3 _L_ ~, __VA_ARGS__ _R_
#define DEFINE_BASE_5(_, Base, ...) L#Base, Base::GetClassType(), DEFINE_BASE_4 _L_ ~, __VA_ARGS__ _R_
#define DEFINE_BASE_6(_, Base, ...) L#Base, Base::GetClassType(), DEFINE_BASE_5 _L_ ~, __VA_ARGS__ _R_
#define DEFINE_BASE_7(_, Base, ...) L#Base, Base::GetClassType(), DEFINE_BASE_6 _L_ ~, __VA_ARGS__ _R_
#define DEFINE_BASE_8(_, Base, ...) L#Base, Base::GetClassType(), DEFINE_BASE_7 _L_ ~, __VA_ARGS__ _R_

//////////////////////////////////////////////////////////////////////////////////
// Must be used in the declaration of any class that derives from Object.

/*	NOTE: GetClassType() must return __Type* 
	because it must be able to return null. */

#define DECLARE_ABSTRACT_TYPE							\
														\
	private:											\
		static __Type* s_type;							\
		static int __init_displayName;					\
														\
	public:												\
		static __Type* GetClassType();					\
		Type GetType() const;


#define DECLARE_TYPE									\
														\
	DECLARE_ABSTRACT_TYPE								\
														\
	public:												\
		static void* CreateInstance();

//////////////////////////////////////////////////////////////////////////////////
// Must be used in the definition of any class that derives from Object.

#define DEFINE_ABSTRACT_TYPE(Class, ...)				\
														\
	DEFINE_TYPE_COMMON(									\
		Class, null,									\
		__VA_ARGS__)


#define DEFINE_TYPE(Class, ...)							\
														\
	DEFINE_TYPE_COMMON(									\
		Class, &Class::CreateInstance,					\
		__VA_ARGS__)									\
														\
	void* Class::CreateInstance()						\
	{													\
		return new Class();								\
	}


#define SET_TYPE_DISPLAY_NAME(Class, displayName)		\
														\
	int Class::__init_displayName =						\
		Class::s_type->SetDisplayName(displayName);


#define DEFINE_TYPE_COMMON(Class, CreateProc, ...)		\
														\
	__Type* Class::s_type = __Type::Register(			\
		L#Class, CreateProc,							\
		DEFINE_BASE(__VA_ARGS__));						\
														\
	__Type* Class::GetClassType()						\
	{													\
		return s_type;									\
	}													\
														\
	Type Class::GetType() const 						\
	{													\
		return *s_type;									\
	}

//////////////////////////////////////////////////////////////////////////////////

// Returns the type of a class
#define TYPEOF(Class) (*Class::GetClassType())

//////////////////////////////////////////////////////////////////////////////////

#endif