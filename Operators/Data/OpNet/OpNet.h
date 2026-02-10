#ifndef OP_NET_H
#define OP_NET_H

/*
	This operator will directly 
	and load any kind of object, serializing and deserializing them as necessary
	
	TODO:

	a text ID (name) will be necessary to differentiate various net connections in the same instance of Play.
	This way multiple things can be sent.

	Multiple OpNets might have to share the same connection. Or maybe not, I don't understand networks yet.
*/

#include "NetConnection.h"
#include "NetServer.h"

#include <Operators\Operator.h>
#include <Types\Basic\String.h>

class OpNet
	: public Operator
{
	DECLARE_TYPE;

public:
	enum Mode { Automatic, Client, Server };

	// Automatic mode allows updates to be both requested and pushed through the same connection.

public:
	OpNet();
	OpNet(const String& address, Mode mode);
	OpNet(const OpNet& op);
	~OpNet();

	String	GetOpName();

	void	Init();
	Object*	Copy();

	bool	UsesSpec(Type specType, Type inType);
	bool	ForwardsSpec(Type specType);

	void	UpdateResult();
	void	_PushUpdate();

	/* TODO:
		
		If a different file is selected and the output is connected to
		an operator who doesn't support the new file type, disconnect them.

	*/

private:
	String	m_address;
	Mode	m_mode;

	NetConnection m_client;
	NetServer	  m_server;

	Object*	m_object;
};

#endif