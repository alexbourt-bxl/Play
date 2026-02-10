#include "User.h"
#include <Users\UserMetric.h>

DEFINE_TYPE(User, Object);

User* User::local = null;

User::User()
{
	metric = new UserMetric(this);
}

User::~User()
{
	if (metric)
		delete metric;
}

Object* User::Copy()
{
	return null;
}

uint User::CreateId()
{
	// Create a user ID that is machine dependent,
	// maybe a hash of the person's current system name
	// and a machine ID.

	return 0x00001234;
}