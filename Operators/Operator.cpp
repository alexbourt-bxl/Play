#include "Operator.h"

#include "Connection.h"
#include <Operators/Tree/Tree.h>
#include <Graphics/Graphics.h>
#include <Types/Geometric/Size.h>
#include <Types/Graphics/Font.h>
#include <Types/Graphics/GdiImage8.h>
#include <Controls/Views/TreeView/TreeView.h>
#include <limits>

DEFINE_ABSTRACT_TYPE(Operator, Object, PropertyContainer);

Operator::Operator()
{
	UpdateEvent.Sender = this;

	*Properties.AddEvent    += EventHandler(this, &Operator::property_OnAdd);
	*Properties.RemoveEvent += EventHandler(this, &Operator::property_OnRemove);
	
	m_inputs.m_operator = this;
	*m_inputs.AddEvent  += EventHandler(this, &Operator::Inputs_OnAdd);

	*m_outputs.AddEvent += EventHandler(this, &Operator::Outputs_OnAdd);

	//*m_views.AddEvent    += EventHandler(this, &Operator::Views_OnAdd);
	//*m_views.RemoveEvent += EventHandler(this, &Operator::Views_OnRemove);

	m_dirty = true;
	m_tree = null;

	m_thumbnail = null;

	m_positionX = std::numeric_limits<double>::quiet_NaN();
	m_positionY = std::numeric_limits<double>::quiet_NaN();

	m_selected = false;
}

Operator::~Operator()
{
	/*	Disconnect the operator if necessary. */

	for (int i = In.Count - 1; i >= 0; i--)
	{
		if (In[i]->Connection)
			In[i]->Disconnect();
	}

	for (int i = Out.Count - 1; i >= 0; i--)
	{
		for (int j = Out[i]->Connections.Count - 1; j >= 0; j--)
			Out[i]->Connections[j]->In->Disconnect();
	}

	/*	Disconnect the operator from all views. */

	for (int i = 0; i < Views.Count; i++)
		DisconnectView(Views[i]);

	/*	Remove the operator from its tree. */

	if (m_tree)
		m_tree->Remove(this);

	//

	if (m_thumbnail)
		delete m_thumbnail;
}

String Operator::GetName()
{
	return m_name;
}

void Operator::SetName(const String& name)
{
	m_name = name;

	// TODO: update view
}

String Operator::GetDisplayName()
{
	return typeof(this).DisplayName;
}

InputArray& Operator::GetInputs()
{
	return m_inputs;
}

OutputArray& Operator::GetOutputs()
{
	return m_outputs;
}

bool Operator::IsBefore(Operator* op)
{
	/*	Check if the op follows 
		this one in the tree. */
	
	for (int i = 0; i < Out.Count; i++)
	{
		Output* out = Out[i];

		for (int j = 0; j < out->Connections.Count; j++)
		{
			Input* in = out->Connections[j]->In;
			assert(in); //if (!in) continue;

			Operator* outOp = in->Operator;

			if (outOp == op
				|| outOp->IsBefore(op))
				return true;
		}
	}

	return false;
}

bool Operator::IsAfter(Operator* op)
{
	/*	Check if the op precedes 
		this one in the tree. */
	
	for (int i = 0; i < In.Count; i++)
	{
		Connection* conn = In[i]->Connection;
		if (!conn) continue;

		Operator* inOp = conn->Out->Operator;

		if (inOp == op
			|| inOp->IsAfter(op))
			return true;
	}

	return false;
}

Tree* Operator::GetTree() const
{
	return m_tree;
}

bool Operator::GetIsDirty()
{
	if (m_dirty)
		return true;

	/*	An operator is also considered dirty 
		if one of its inputs is dirty. */

	for (int i = 0; i < In.Count; i++)
	{
		if (In[i]->Connection
			&& In[i]
				->Connection->Out
				->Operator
				->IsDirty)
			return true;
	}

	return false;
}

void Operator::MarkDirty()
{
	m_propLock.Lock();
	m_dirty = true;
	m_propLock.Unlock();
}

void Operator::LockProps()
{
	m_propLock.Lock();
}

void Operator::UnlockProps()
{
	m_propLock.Unlock();
}

void Operator::Update()
{
	/*	Check if the operator is dirty, 
		which means an update is necessary. */

	if (!IsDirty)
		return;

	//

	m_propLock.Lock();

	CopyWorkingValues();
	m_dirty = false;

	m_propLock.Unlock();

	//

	Lock();

	Process();
	UpdateProgress(1.0);

	/*	Mark all the downstream
		operators as dirty. */

	for (int i = 0; i < Out.Count; i++)
	{
		Output* out = Out[i];

		for (int j = 0; j < out->Connections.Count; j++)
			out->Connections[j]->In->Operator->MarkDirty();
	}

	/*	TODO: each function which is called should update the interface with its progress,
		but all are called from the same thread at the end of the chain. */
																								
	Operator::Message msg(this);
	UpdateEvent.Send(msg);

	Unlock();
}

void Operator::UpdateProgress(double /*progress*/)
{
	/*	TODO: 
	
		This method should request that the display thread 
		show the progress, not to slow down the processing itself. Possibly
		using user functions of the Window or something. */
}

/*	TODO:
	
	GetThumbnail should be called by a separate thread, so that UI speed 
	isn't affected by thumbnail generation. Maybe one thread for all such calls. */

GdiImage8* Operator::GetThumbnail(const Size& /*_size*/)
{
	//if (_size.Width == 0 || _size.Height == 0)
	//	return null;

	///*	If the op has no output
	//	thumbnails don't make sense. */
	//if (Out.Count == 0)
	//	return null;

	///*	To get the thumbnail the op 
	//	first has to be updated. */
	////Update();

	///*	If no result has been generated
	//	no thumbnail can be shown. */
	//if (!Out[0]->Result)
	//	return null;

	///*	If a thumnbnail already exists and the op is clean,
	//	return the existing thumbnail. */
	//if (m_thumbnail /*&& !IsDirty*/)
	//	return m_thumbnail;

	///*	Otherwise a thumbnail is generated 
	//	based on the type of output. */

	//if (typeof(Out[0]->Result) == TYPEOF(Image))
	//{
	//	/*	Determine the thumbnail rect. */

	//	Image* out = (Image*)Out[0]->Result;

	//	double scale = max(
	//		(double)out->Width / (double)_size.Width, 
	//		(double)out->Height / (double)_size.Height);

	//	Size size(
	//		(int)((double)out->Width / scale),
	//		(int)((double)out->Height / scale));

	//	/*	If the thumbnail doesn't exist,	
	//		create it. */
	//	if (!m_thumbnail)
	//		m_thumbnail = new GdiImage8(size);

	//	/*	Otherwise resize and clear 
	//		the existing thumbnail. */
	//	else if (m_thumbnail->Size != size)
	//	{
	//		m_thumbnail->SetSize(_size);
	//		m_thumbnail->Clear();
	//	}

	//	/*	Draw the thumbnail. */

	//	Graphics g(m_thumbnail);

	//	//g.FillRect(
	//	//	0, 0,
	//	//	m_thumbnail->Width, m_thumbnail->Height,
	//	//	Color::White);

	//	g.Interpolation = Graphics::Bilinear;
	//	g.DrawImage(
	//		0, 0,
	//		m_thumbnail->Width, m_thumbnail->Height,
	//		out,
	//		0, 0,
	//		out->Width, out->Height);

	//	return m_thumbnail;
	//}

	//

	return null;
}

FPoint Operator::GetPosition()
{
	return FPoint(
		m_positionX,
		m_positionY);
}

void Operator::SetPosition(const FPoint& p)
{
	assert(m_tree);

	/*	Only one of the axes must send the event
		for the position to be updated, so one's
		events must be disabled. */

	m_positionX.DisableEvents();
	m_positionX = p.X;
	m_positionX.EnableEvents();

	m_positionY = p.Y;
}

bool Operator::GetIsSelected()
{
	return m_selected;
}

void Operator::SetIsSelected(bool selected)
{
	if (m_selected == selected)
		return;

	m_selected = selected;

	if (!m_tree)
		return;

	if (m_selected)
	{
		assert(!m_tree->Selected.Contains(this));
		m_tree->Selected.Add(this);
	}
	else
	{
		assert(m_tree->Selected.Contains(this));
		m_tree->Selected.Remove(this);
	}
}

Array<OpView*>& Operator::GetViews()
{
	return m_views;
}

//Array<OpView*>& Operator::GetActiveViews()
//{
//	return m_activeViews;
//}

void Operator::ConnectView(OpView* /*view*/)
{
	//
}

void Operator::DisconnectView(OpView* /*view*/)
{
	//
}