#include "OpNet.h"

DEFINE_TYPE(OpNet, Operator);

OpNet::OpNet() 
{
	m_mode = Automatic;
	
	m_object = null;
	
	Init();
}

OpNet::OpNet(const String& address, Mode mode) 
{
	m_address = address;
	m_mode = mode;

	m_object = null;
	
	Init();
}

OpNet::OpNet(const OpNet& op) 
{
	m_address = op.m_address;
	m_mode = op.m_mode;
	
	m_object = null;
	
	Init();
}

OpNet::~OpNet()
{
	if (m_object)
		delete m_object;
}

String OpNet::GetOpName()
{
	return L"Net";
}

void OpNet::Init()
{
	In.Add(new Input());
	In[0]->AcceptedTypes.Add(TYPEOF(Object));

	Out.Add(new Output());
	Out[0]->SetResult(m_object);

	m_server.Start();
}

Object* OpNet::Copy()
{
	return OpNet(*this);
}

bool OpNet::UsesSpec(Type /*specType*/, Type /*objType*/)
{
	return false;
}

bool OpNet::ForwardsSpec(Type /*specType*/)
{
	return false;
}

void OpNet::UpdateResult()
{
	if (m_mode == Client)
	{
		m_client.Connect(m_address);
	}
}

void OpNet::_PushUpdate()
{
	// If there is an input and the connection is outgoing,
	// the push must be sent over the network.

	if (In[0]->ConnectedOutput)
	{
		// TODO: send push over the network
	}

	// Otherwise use rthe default behavior

	else
	{
		Operator::_PushUpdate();
	}
}
