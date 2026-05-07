#include "VtkContrib/vtkTransformHelper.h"

#include <VtkContrib/vtkFloatingPointType.h>
#include <assert.h>



// ================================================== LA STRUCTURE vtkSimpleTransformMemento  ==================================================

vtkTransformHelper::vtkSimpleTransformMemento::vtkSimpleTransformMemento ( )
	: isExtrinsic (true), scaleX (1.), scaleY (1.), scaleZ (1.), 
	  xoy (0.), xoz (0.), yoz (0.), dx (0.), dy (0.), dz (0.), translationFirst (true),	userData (0)
{
}	// vtkSimpleTransformMemento

vtkTransformHelper::vtkSimpleTransformMemento::vtkSimpleTransformMemento (const vtkTransformHelper::vtkSimpleTransformMemento& m)
	: isExtrinsic (m.isExtrinsic), scaleX (m.scaleX), scaleY (m.scaleY), scaleZ (m.scaleZ),
	  xoy (m.xoy), xoz (m.xoz), yoz (m.yoz), dx (m.dx), dy (m.dy), dz (m.dz), translationFirst (m.translationFirst), userData (m.userData)
{
}	// vtkSimpleTransformMemento::vtkSimpleTransformMemento


vtkTransformHelper::vtkSimpleTransformMemento& vtkTransformHelper::vtkSimpleTransformMemento::operator = (const vtkTransformHelper::vtkSimpleTransformMemento& m)
{
	isExtrinsic			= m.isExtrinsic;
	scaleX				= m.scaleX;
	scaleY				= m.scaleY;
	scaleZ				= m.scaleZ;
	xoy					= m.xoy;
	xoz					= m.xoz;
	yoz					= m.yoz;
	dx					= m.dx;
	dy					= m.dy;
	dz					= m.dz;
	translationFirst	= m.translationFirst;
	userData			= m.userData;

	return *this;
}	// vtkSimpleTransformMemento::operator =


vtkTransformHelper::vtkSimpleTransformMemento::~vtkSimpleTransformMemento ( )
{
}	// vtkSimpleTransformMemento::~vtkSimpleTransformMemento


bool vtkTransformHelper::vtkSimpleTransformMemento::operator == (const vtkTransformHelper::vtkSimpleTransformMemento& m) const
{
	if ((isExtrinsic != m.isExtrinsic) || (scaleX != m.scaleX) || (scaleY != m.scaleY) || (scaleZ != m.scaleZ))
		return false;

	if ((isExtrinsic != m.isExtrinsic) || (xoy != m.xoy) || (xoz != m.xoz) || (yoz != m.yoz) || (dx != m.dx) || (dy != m.dy) || (dz != m.dz) || (translationFirst != m.translationFirst))
			return false;

	return true;
}	// vtkSimpleTransformMemento::operator ==


bool vtkTransformHelper::vtkSimpleTransformMemento::operator != (const vtkTransformHelper::vtkSimpleTransformMemento& m) const
{
	return !(m == *this);
}	// vtkSimpleTransformMemento::operator !=
		

bool vtkTransformHelper::vtkSimpleTransformMemento::IsIdentity ( ) const
{
	if ((0. != xoy) || (0. != xoz) || (0. != yoz) || (0. != dx) || (0. != dy) || (0. != dz))
			return false;

	return true;
}	// vtkSimpleTransformMemento::IsIdentity


// ======================================================= LA CLASSE vtkTransformHelper  =======================================================

vtkTransformHelper::vtkTransformHelper ( )
{
	assert (0 && "vtkTransformHelper::vtkTransformHelper is not allowed.");
}	// vtkTransformHelper::vtkTransformHelper


vtkTransformHelper::vtkTransformHelper (const vtkTransformHelper&)
{
	assert (0 && "vtkTransformHelper::vtkTransformHelper is not allowed.");
}	// vtkTransformHelper::vtkTransformHelper (const vtkTransformHelper&)


vtkTransformHelper& vtkTransformHelper::operator = (const vtkTransformHelper&)
{
	assert (0 && "vtkTransformHelper::operator = is not allowed.");
	return *this;
}	// vtkTransformHelper::operator =


vtkTransformHelper::~vtkTransformHelper ( )
{
	assert (0 && "vtkTransformHelper::~vtkTransformHelper is not allowed.");
}	// vtkTransformHelper::~vtkTransformHelper


void vtkTransformHelper::ShrinkProperty (vtkProp3D& prop3D, double factor)
{
	vtkFloatingPointType*	center	= prop3D.GetCenter ( );
	ShrinkProperty (prop3D, factor, center [0], center [1], center [2]);
}	// vtkTransformHelper::ShrinkProperty (vtkProp3D& prop3D, double factor)


void vtkTransformHelper::ShrinkProperty (vtkProp3D& prop3D, double factor,
			double xCenter, double yCenter, double zCenter)
{
	vtkTransform*	transform	= vtkTransform::New ( );
	transform->PostMultiply ( );
	vtkFloatingPointType	origin [3];
	prop3D.GetOrigin (origin);
	vtkFloatingPointType*	center	= prop3D.GetCenter ( );
	transform->Translate (-xCenter, -yCenter, -zCenter);
	transform->Scale (factor, factor, factor);
	transform->Translate (xCenter, yCenter, zCenter);
	transform->Translate (-origin [0], -origin [1], -origin [2]);
	transform->PreMultiply ( );
	transform->Translate (origin [0], origin [1], origin [2]);
	prop3D.SetPosition (transform->GetPosition ( ));
	prop3D.SetScale (transform->GetScale ( ));
	transform->Delete ( );
}	// vtkTransformHelper::ShrinkProperty


vtkTransform* vtkTransformHelper::CreateTransform (const vtkTransformHelper::vtkSimpleTransformMemento& memento)
{
	vtkTransform*	transform	= vtkTransform::New ( );
	assert (0 != transform);

	if ((1. != memento.scaleX) || (1. != memento.scaleY) || (1. != memento.scaleZ))
		transform->Scale (memento.scaleX, memento.scaleY, memento.scaleZ);

	if (true == memento.isExtrinsic)
	{
		if (false == memento.translationFirst)
			transform->Translate (memento.dx, memento.dy, memento.dz);
		transform->RotateY (memento.xoz);
		transform->RotateX (memento.yoz);
		transform->RotateZ (memento.xoy);
		if (true == memento.translationFirst)
			transform->Translate (memento.dx, memento.dy, memento.dz);
	}	// if (true == memento.isExtrinsic)
	else
	{
		if (true == memento.translationFirst)
			transform->Translate (memento.dx, memento.dy, memento.dz);
		// RotateY(phi) → RotateZ(theta) → RotateX(omega) : chaque rotation s’applique dans le repère local courant, 
        // méthode standard pour les angles de Tait-Bryan intrinsèques (ZYX) en robotique/aéronautique. Dixit mistral.ai.
        // Pour ce il faut inverser l'ordre des rotations par rapport à la même transformation mais à repère constant
        // (transformation extrinsèque).
		transform->RotateX (memento.yoz);
		transform->RotateZ (memento.xoy);
		transform->RotateY (memento.xoz);
		if (false == memento.translationFirst)
			transform->Translate (memento.dx, memento.dy, memento.dz);
		transform->PostMultiply ( );
	}	// else if (true == memento.isExtrinsic)

	return transform;
}	// vtkTransformHelper::CreateTransform

