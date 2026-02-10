#ifndef OUTPUT_ARRAY_H
#define OUTPUT_ARRAY_H

#include <Containers/Array.h>
#include "Output.h"
#include <Graphics\Graphics.h>

class Input;
class InputArray;
class Connection;

class OutputArray
	: public Array<Output*>
{
	friend class Operator;

public:
	operator Output* ();

	Connection* Connect(Input* in);

	Connection* operator >> (Input* in);
	Connection* operator >> (InputArray& inputs);
};

#endif