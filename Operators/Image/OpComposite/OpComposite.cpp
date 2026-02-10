#include "OpComposite.h"

#include <Native\Gdi.h>

#include <Operators\Components\Specs\TypeSpec.h>
#include <Operators\Components\Specs\CropSpec.h>
#include <Operators\Components\Specs\OffsetSpec.h>
#include <Operators\Components\Specs\ClipSpec.h>
#include <Operators\Components\Specs\OpacitySpec.h>
#include <Operators\Components\Specs\SizeSpec.h>
#include <Operators\Components\Specs\TargetSpec.h>

DEFINE_TYPE(OpComposite, Operator);

OpComposite::OpComposite() 
{
	Init();

	TargetBottomLayer = false;
}

OpComposite::OpComposite(const OpComposite& op) 
{
	Init();

	TargetBottomLayer = op.TargetBottomLayer;
}

OpComposite::~OpComposite()
{

}

String OpComposite::GetOpName()
{
	return L"Composite";
}

void OpComposite::Init()
{
	In.AcceptedTypes.Add(TYPEOF(Image8));
	In.AcceptedTypes.Add(TYPEOF(GdiImage));
	In.AcceptedTypes.Add(TYPEOF(GdiImage8));
	In.CountIsFixed = false;

	Out.Add(&m_out);
}

Object* OpComposite::Copy()
{
	return OpComposite(*this);
}

/*	TODO:

	Doing things like quick resizing of any type to a thumbnail should
	be possible with a direct OpConvert between the source and the OpComposite.
	
	*/

/*	TODO:

	OpComposite should be the one drawing the interface of the layers
	and allowing layer manipulations (reordering, hiding, etc.) in the view.
	 
	*/

bool OpComposite::UsesSpec(Type specType, Type inType)
{
	return
		  (specType == TYPEOF(SizeSpecNearest) && inType == TYPEOF(GdiImage))
		|| specType == TYPEOF(OffsetSpec)
		|| specType == TYPEOF(OpacitySpec)
		|| specType == TYPEOF(CropSpec);
}

bool OpComposite::ForwardsSpec(Type /*specType*/)
{
	return false;
}

void OpComposite::UpdateResult(SpecList& _specs)
{
	SpecList specs = _specs;

	/*	If no inputs are connected, clear the result and return. */

	if (In.Count == 0)
	{
		m_out.SetResult(null, SpecList::Empty);
		return; 
	}





	/*	First decide what the output is.

		If a target spec was passed
		{
			use it as output
			use its output size
			use its output type

			can a passed opacity spec be used as well?
		}
		else
		{
			Determine output type
				Check if a type spec was passed

			If the bottom layer is the target
			{
				if type spec was passed 
					get bottom layer with passed type
				otherwise 
					get type from bottom layer
				
				use it as output
				use it as output size
				use its output type
			}
			else
			{
				otherwise determine extents of result
					get each layer's output specs with output type
					add to extents (size + sizeSpec + offsetSpec + crop)
			}
		}

		paint all layers
			if target is botom layer
				skip first layer

			for each layer 
			{
				if target spec was passed
					with TargetSpec
				else if bottom layer is target
					with TargetSpec of bottom layer
				
				get layer's output specs

				paint layer to target
			}
	*/























	/*	Check spec list for clip region. */

	Region specClip = ClipSpec::Scan(specs);

	/*	Get the bottom layer. */
		
	Image* inBottom = (Image*)In[0]->GetInputObject(specs);;
	assert(inBottom);

	/*	If the bottom layer is the target,
		mark it as dirty. */

	if (TargetBottomLayer)
		In[0]->Connection_->Out->GetOperator()->MarkDirty();

	/*	Check requested type. */

	__Type* outType = typeof(inBottom);
	
	/*	Determine the output size. */

	Size outSize;

	/*	If a size or crop spec is available, use it to determine the output size, 
		and to pass it along (properly adjusted) as a clip spec with all input requests . */

	Size specSize = SizeSpec::Scan(specs);
	Rect specCrop = CropSpec::Scan(specs);

	if (TargetBottomLayer)
		outSize = inBottom->Size;
	else if (specSize.Width > 0 && specSize.Height > 0)
		outSize = specSize;
	else if (!specCrop.IsNull)
		outSize = specCrop.Size;
	else
	{
		outSize = inBottom->Size;
		// TODO: get the extents of all the layers.
	}

	/*	Create the output image. If it has already been
		created, check if it needs to be resized. */

	Image* out = (Image*)m_out.Result;

	/*	If the bottom layer is the target,
		do not create a result. */

	if (TargetBottomLayer)
	{
		out = inBottom;
	}
	else if (!out)
	{
		out = Image::Create(
			outSize.Width, 
			outSize.Height,
			*outType);
	}
	else if (out->Width != outSize.Width 
		 || out->Height != outSize.Height)
	{
		out->SetSize(
			outSize.Width, 
			outSize.Height);
		
		out->Clear();
	}
	else
	{
		out->Clear();
	}

	/*	Composite inputs. */

	for (int i = 0; i < In.Count; i++)
	{
		/*	Ignore the first layer if 
			it is the target. */

		if (TargetBottomLayer && i == 0)
			continue; 

		SpecList outSpecs = specs;

		Image* in = (Image*)In[i]->GetInputObject(outSpecs);

		if (!in)
			continue;

		SpecList inSpecs = In[i]->Connection_->Out->Specs;

		/*	Scan for supported specs. */

		Point offset = OffsetSpec::Scan(inSpecs);
		float opacity = OpacitySpec::Scan(inSpecs);

		/*	Determine crop. */

		Rect crop = CropSpec::Scan(inSpecs);

		Rect inRect = crop.IsNull
			? Rect(0, 0, in->Width, in->Height)
			: crop;

		/*	Image8  */

		if (*outType == TYPEOF(Image8))
		{
			Image8* _out = (Image8*)out;
			Image8* _in = (Image8*)in;

			Composite_Image8(
				(int*)_out->Buffer,
				out->Width, out->Height,
				offset.X, offset.Y,
				//
				(int*)_in->Buffer,
				in->Width, in->Height,
				inRect.X, inRect.Y,
				inRect.Width, inRect.Height,
				//
				Math::ToInt8(opacity),
				specClip);
		}

		/*	GdiImage  */

		else if (*outType == TYPEOF(GdiImage))
		{
			/*	Check in specs for a size. */

			Size _outSize = SizeSpec::Scan(inSpecs);

			if (_outSize.Width == 0 || _outSize.Height == 0)
				_outSize = inRect.Size;

			Composite_GdiImage(
				((GdiImage*)out)->Hdc,
				offset.X, offset.Y,
				_outSize.Width, _outSize.Height,
				//
				((GdiImage*)in)->Hdc,
				inRect.X, inRect.Y,
				inRect.Width, inRect.Height,
				//
				Math::ToInt8(opacity),
				specClip);
		}
	}

	if (TargetBottomLayer)
		m_out.SetResultTransitive(out, SpecList::Empty);
	else
		m_out.SetResult(out, SpecList::Empty);
}