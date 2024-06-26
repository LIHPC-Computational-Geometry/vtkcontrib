#include "VtkContrib/vtkConstrainedPointWidget2.h"

#include <vtkCellPicker.h>
#include <vtkRenderer.h>
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


// =============================================================================
//              LA CLASSE vtkConstrainedPointHandleRepresentation2
// =============================================================================

vtkConstrainedPointHandleRepresentation2::vtkConstrainedPointHandleRepresentation2 ( )
	: vtkSphereHandleRepresentation ( )
{
}	// vtkConstrainedPointHandleRepresentation2::vtkConstrainedPointHandleRepresentation2


vtkConstrainedPointHandleRepresentation2::vtkConstrainedPointHandleRepresentation2 (const vtkConstrainedPointHandleRepresentation2&)
	: vtkSphereHandleRepresentation ( )
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
			this->ConstraintAxis = this->DetermineConstraintAxis (this->ConstraintAxis, pickPoint);
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
}	// vtkConstrainedPointHandleRepresentation2::SetConstraintAxis


