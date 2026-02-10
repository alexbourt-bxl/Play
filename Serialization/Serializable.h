#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include <Types/Object.h>

const uint Marker_SERI = 0x49524553; // little endian

class Data;

class Serializable
{
	DECLARE_ABSTRACT_TYPE;

public:
	virtual Data* Serialize() = 0;
	virtual bool Deserialize(Data* serialized) = 0;
};

#endif