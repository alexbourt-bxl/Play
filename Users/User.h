#ifndef USER_H
#define USER_H

#include <Types/Object.h>

class UserMetric;

class User
	: public Object
{
	DECLARE_TYPE;

public:
	Object* Copy();

	static User* local;
	
	UserMetric*	metric;

public:
	User();
	virtual ~User();

	uint CreateId();
};

#endif