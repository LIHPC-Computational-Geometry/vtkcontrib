#include "VtkContrib/vtkConstrainedPointWidget2.h"

#include <vtkCellPicker.h>
#include <vtkLineSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

#include <assert.h>
#include <iostream>
using namespace std;


// =============================================================================
//                     LA CLASSE vtkConstrainedPointWidget2
// =============================================================================

vtkConstrainedPointWidget2::vtkConstrainedPointWidget2 ( )
	: vtkHandleWidget ( )
{
}	// vtkConstrainedPointWidget2::vtkConstrainedPointWidget2


vtkConstrainedPointWidget2::vtkConstrainedPointWidget2 (const vtkConstrainedPointWidget2&)
	: vtkHandleWidget ( )
{
	assert (0 && "vtkConstrainedPointWidget2 copy constructor is not allowed.");
}	// vtkConstrainedPointWidget2::vtkConstrainedPointWidget2


vtkConstrainedPointWidget2& vtkConstrainedPointWidget2::operator = (const vtkConstrainedPointWidget2&)
{
	assert (0 && "vtkConstrainedPointWidget2 assignment operator is not allowed.");
	return *this;
}	// vtkConstrainedPointWidget2::vtkConstrainedPointWidget2


vtkConstrainedPointWidget2::~vtkConstrainedPointWidget2 ( )
{
}	// vtkConstrainedPointWidget2::~vtkConstrainedPointWidget2


void vtkConstrainedPointWidget2::CreateDefaultRepresentation ( )
{
	if (0 == WidgetRep)
	{
		vtkConstrainedPointHandleRepresentation2*	rep	= vtkConstrainedPointHandleRepresentation2::New ( );
		WidgetRep	= rep;
		rep->SetTolerance (1);
		rep->BuildRepresentation ( );
		SetWidgetRepresentation (rep);
	}	// if (0 == WidgetRep)
}	// vtkConstrainedPointWidget2::CreateDefaultRepresentation


vtkConstrainedPointWidget2* vtkConstrainedPointWidget2::New ( )
{
	return new vtkConstrainedPointWidget2 ( );
}	// vtkConstrainedPointWidget2::New


vtkConstrainedPointHandleRepresentation2* vtkConstrainedPointWidget2::GetConstrainedPointRepresentation ( )
{
	return dynamic_cast<vtkConstrainedPointHandleRepresentation2*>(GetRepresentation ( ));
}	// vtkConstrainedPointWidget2::GetConstrainedPointRepresentation


void vtkConstrainedPointWidget2::OnChar ( )
{
	if ((0 != this->Interactor) && (0 != GetConstrainedPointRepresentation ( )))
	{	    
		switch (this->Interactor->GetKeyCode ( ))
		{
			case ' '	: GetConstrainedPointRepresentation ( )->SetConstraintAxis (-1);	break;
			case 'x'	:
			case 'X'	: GetConstrainedPointRepresentation ( )->SetConstraintAxis (0);		break;
			case 'y'	:
			case 'Y'	: GetConstrainedPointRepresentation ( )->SetConstraintAxis (1);		break;
			case 'z'	:
			case 'Z'	: GetConstrainedPointRepresentation ( )->SetConstraintAxis (2);		break;
			default		: vtkHandleWidget::OnChar ( );
		}	// switch (this->Interactor->GetKeyCode ( ))
	}	// if ((0 != this->Interactor) && (0 != GetConstrainedPointRepresentation ( )))
}	// vtkConstrainedPointWidget2::OnChar


void vtkConstrainedPointWidget2::EndInteraction ( )
{
	if (0 != GetConstrainedPointRepresentation ( ))
		GetConstrainedPointRepresentation ( )->SetConstraintAxis (-1);
		
	vtkHandleWidget::EndInteraction ( );
}	// vtkConstrainedPointWidget2::EndInteraction


// =============================================================================
//              LA CLASSE vtkConstrainedPointHandleRepresentation2
// =============================================================================

vtkConstrainedPointHandleRepresentation2::vtkConstrainedPointHandleRepresentation2 ( )
	: vtkSphereHandleRepresentation ( ), _displayConstraintAxis (false), _constraintAxisActor (0)
{
}	// vtkConstrainedPointHandleRepresentation2::vtkConstrainedPointHandleRepresentation2


vtkConstrainedPointHandleRepresentation2::vtkConstrainedPointHandleRepresentation2 (const vtkConstrainedPointHandleRepresentation2&)
	: vtkSphereHandleRepresentation ( ), _displayConstraintAxis (false), _constraintAxisActor (0)
{
	assert (0 && "vtkConstrainedPointHandleRepresentation2 copy constructor is not allowed.");
}	// vtkConstrainedPointHandleRepresentation2::vtkConstrainedPointHandleRepresentation2


vtkConstrainedPointHandleRepresentation2& vtkConstrainedPointHandleRepresentation2::operator = (const vtkConstrainedPointHandleRepresentation2&)
{
	assert (0 && "vtkConstrainedPointHandleRepresentation2 assignment operator is not allowed.");
	return *this;
}	// vtkConstrainedPointHandleRepresentation2::vtkConstrainedPointHandleRepresentation2


vtkConstrainedPointHandleRepresentation2::~vtkConstrainedPointHandleRepresentation2 ( )
{
	
}	// vtkConstrainedPointHandleRepresentation2::~vtkConstrainedPointHandleRepresentation2


vtkConstrainedPointHandleRepresentation2* vtkConstrainedPointHandleRepresentation2::New ( )
{
	return new vtkConstrainedPointHandleRepresentation2 ( );
}	// vtkConstrainedPointHandleRepresentation2::New


void vtkConstrainedPointHandleRepresentation2::WidgetInteraction (double eventPos [2])
{	// Code vtkSphereHandleRepresentation::WidgetInteraction VTK 7.1.1 légèrement retouché
	// Do different things depending on state Calculations everybody does
	double focalPoint[4], pickPoint[4], prevPickPoint[4], startPickPoint[4], z;
  
	// Compute the two points defining the motion vector
	vtkInteractorObserver::ComputeWorldToDisplay (this->Renderer, this->LastPickPosition[0], this->LastPickPosition[1], this->LastPickPosition[2], focalPoint);
	z = focalPoint[2];
	vtkInteractorObserver::ComputeDisplayToWorld (this->Renderer, this->LastEventPosition[0],this->LastEventPosition[1], z, prevPickPoint);
	vtkInteractorObserver::ComputeDisplayToWorld (this->Renderer, eventPos[0], eventPos[1], z, pickPoint);

	// Process the motion
	if (this->InteractionState == vtkHandleRepresentation::Selecting || this->InteractionState == vtkHandleRepresentation::Translating)
	{
		if (!this->WaitingForMotion || this->WaitCount++ > 3)
		{
#ifdef VTK_7
			this->ConstraintAxis = this->DetermineConstraintAxis (this->ConstraintAxis, pickPoint);
#endif	// VTK_7
			if (this->InteractionState == vtkHandleRepresentation::Selecting && !this->TranslationMode)
			{
				this->MoveFocus(prevPickPoint, pickPoint);
			}
			else
			{
				this->Translate(prevPickPoint, pickPoint);
			}
		}
	}
	else if (this->InteractionState == vtkHandleRepresentation::Scaling)
	{
		this->Scale (prevPickPoint, pickPoint, eventPos);
	}

	// Book keeping
	this->LastEventPosition[0] = eventPos[0];
	this->LastEventPosition[1] = eventPos[1];

	this->Modified ( );
}	// vtkConstrainedPointHandleRepresentation2::WidgetInteraction


void vtkConstrainedPointHandleRepresentation2::SetConstraintAxis (int axis)
{
	const	bool	displayConstraintAxis	= _displayConstraintAxis;
	if ((0 != GetConstrained ( )) && (axis != this->ConstraintAxis))
		DisplayConstraintAxis (false);
		
	this->ConstraintAxis	= axis;
	switch (axis)
	{
		case 0	: // x
		case 1	: // y
		case 2	: // z
			ConstrainedOn ( );
			break;
		default	:
			ConstrainedOff ( );
	}	// switch (axis)
	
	if (true == displayConstraintAxis)
		DisplayConstraintAxis (true);
}	// vtkConstrainedPointHandleRepresentation2::SetConstraintAxis


void vtkConstrainedPointHandleRepresentation2::DisplayConstraintAxis (bool onOff)
{
	if ((true == onOff) && (0 != _constraintAxisActor))
		return;
	_displayConstraintAxis	= onOff;
	
	if ((true == onOff) && (0 <= this->ConstraintAxis) && (2 >= this->ConstraintAxis))
	{
		// Le point courrant :
		double	pos [3]	= { 0., 0., 0. };
		GetWorldPosition (pos);
		
		// L'univers :
		double	bounds [6]	= { DBL_MAX, -DBL_MAX, DBL_MAX, -DBL_MAX, DBL_MAX, -DBL_MAX };
		double	dx			= 0., dy	= 0., dz	= 0.;
		if (0 != this->Renderer)
		{
			this->Renderer->ComputeVisiblePropBounds (bounds);
			dx	= bounds [1] - bounds [0];
			dy	= bounds [3] - bounds [2];
			dz	= bounds [5] - bounds [4];
		}	// if (0 != this->Renderer)

		vtkLineSource*	line	= vtkLineSource::New ( );
		vtkPoints*		points	= vtkPoints::New ( );
		points->Initialize ( );
		switch (this->ConstraintAxis)
		{
			case 0	:
				line->SetPoint1 (bounds [0] - dx, pos [1], pos [2]);
				line->SetPoint2 (bounds [1] + dx, pos [1], pos [2]);
				break;
			case 1	:
				line->SetPoint1 (pos [0], bounds [2] - dy, pos [2]);
				line->SetPoint2 (pos [0], bounds [3] + dy, pos [2]);
				break;
			case 2	:
				line->SetPoint1 (pos [0], pos [1], bounds [4] - dz);
				line->SetPoint2 (pos [0], pos [1], bounds [5] + dz);
				break;
		}	// switch (this->ConstraintAxis)
		vtkPolyDataMapper*	mapper	= vtkPolyDataMapper::New ( );
		mapper->SetInputConnection (line->GetOutputPort ( ));
		mapper->ScalarVisibilityOff ( );
		assert (0 == _constraintAxisActor);
		_constraintAxisActor	= vtkActor::New ( );
		_constraintAxisActor->SetProperty (this->GetSelectedProperty ( ));	// => couleur de la sphère lors des interactions
		_constraintAxisActor->SetMapper (mapper);
		if (0 != this->Renderer)
		{
			this->Renderer->AddActor (_constraintAxisActor);
			if (0 != this->Renderer->GetRenderWindow ( ))
				this->Renderer->GetRenderWindow ( )->Render ( );
		}
		line->Delete ( );
		mapper->Delete ( );
	}	// if ((true == onOff) && (0 <= this->ConstraintAxis) && (2 >= this->ConstraintAxis))
	else
	{
		if (0 != _constraintAxisActor)
		{
			if (0 != this->Renderer)
				this->Renderer->RemoveActor (_constraintAxisActor);
			_constraintAxisActor->Delete ( );
		}	// if (0 != _constraintAxisActor)
		_constraintAxisActor	= 0;
	}	// else if ((true == onOff) && (0 <= this->ConstraintAxis) && (2 >= this->ConstraintAxis))
}	// vtkConstrainedPointHandleRepresentation2::DisplayConstraintAxis
