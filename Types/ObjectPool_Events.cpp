#include "ObjectPool.h"

#include <Types/Object.h>

void ObjectPool::deleteQueue_OnRemove(Container::Message& msg)
{
	Object* obj = *((Object**)msg.Item);
	delete obj;
}