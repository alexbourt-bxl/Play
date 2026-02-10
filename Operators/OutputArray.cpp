#include "OutputArray.h"

#include <Operators\Input.h>
#include <Operators\InputArray.h>
#include <Operators\Connection.h>

OutputArray::operator Output* ()
{
	return Count > 0
		? GetItem(0)
		: null;
}

Connection* OutputArray::Connect(Input* in)
{
	return Count > 0
		? GetItem(0)->Connect(in)
		: null;
}

Connection* OutputArray::operator >> (Input* in)
{
	return Count > 0
		? GetItem(0)->Connect(in)
		: null;
}

Connection* OutputArray::operator >> (InputArray& inputs)
{
	return Count > 0
		? inputs.Connect(GetItem(0))
		: null;
}