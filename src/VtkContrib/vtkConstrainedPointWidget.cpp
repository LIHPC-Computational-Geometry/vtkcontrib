#include "VtkContrib/vtkConstrainedPointWidget.h"

#include <vtkCellPicker.h>
#include <vtkRenderer.h>

#include <assert.h>
#include <values.h>	// MAXDOUBLE
#include <iostream>
using namespace std;


// =============================================================================
//                     LA CLASSE vtkConstrainedPointWidget
// =============================================================================

vtkConstrainedPointWidget::vtkConstrainedPointWidget ( )
	: vtkHandleWidget ( )
{
}	// vtkConstrainedPointWidget::vtkConstrainedPointWidget


vtkConstrainedPointWidget::vtkConstrainedPointWidget (
											const vtkConstrainedPointWidget&)
	: vtkHandleWidget ( )
{
	assert (0 && "vtkConstrainedPointWidget copy constructor is not allowed.");
}	// vtkConstrainedPointWidget::vtkConstrainedPointWidget


vtkConstrainedPointWidget& vtkConstrainedPointWidget::operator = (
											const vtkConstrainedPointWidget&)
{
	assert (0 && "vtkConstrainedPointWidget assignment operator is not allowed.");
	return *this;
}	// vtkConstrainedPointWidget::vtkConstrainedPointWidget


vtkConstrainedPointWidget::~vtkConstrainedPointWidget ( )
{
}	// vtkConstrainedPointWidget::~vtkConstrainedPointWidget


void vtkConstrainedPointWidget::CreateDefaultRepresentation ( )
{
	if (0 == WidgetRep)
	{
		vtkConstrainedPointHandleRepresentation*	rep	=
							vtkConstrainedPointHandleRepresentation::New ( );
		WidgetRep	= rep;
		rep->SetTolerance (1);
		rep->SmoothMotionOff ( );
		SetWidgetRepresentation (rep);
	}	// if (0 == WidgetRep)
}	// vtkConstrainedPointWidget::CreateDefaultRepresentation


vtkConstrainedPointWidget* vtkConstrainedPointWidget::New ( )
{
	return new vtkConstrainedPointWidget ( );
}	// vtkConstrainedPointWidget::New


vtkConstrainedPointHandleRepresentation*
				vtkConstrainedPointWidget::GetConstrainedPointRepresentation ( )
{
	return dynamic_cast<vtkConstrainedPointHandleRepresentation*>(GetRepresentation ( ));
}	// vtkConstrainedPointWidget::GetConstrainedPointRepresentation


// =============================================================================
//              LA CLASSE vtkConstrainedPointHandleRepresentation
// =============================================================================

vtkConstrainedPointHandleRepresentation::vtkConstrainedPointHandleRepresentation ( )
	: vtkPointHandleRepresentation3D ( )
{
	vtkPointHandleRepresentation3D::SetPointPlacer (
									vtkUnstructuredGridPointPlacer::New ( ));
}	// vtkConstrainedPointHandleRepresentation::vtkConstrainedPointHandleRepresentation


vtkConstrainedPointHandleRepresentation::vtkConstrainedPointHandleRepresentation (
								const vtkConstrainedPointHandleRepresentation&)
	: vtkPointHandleRepresentation3D ( )
{
	assert (0 && "vtkConstrainedPointHandleRepresentation copy constructor is not allowed.");
}	// vtkConstrainedPointHandleRepresentation::vtkConstrainedPointHandleRepresentation


vtkConstrainedPointHandleRepresentation&
				vtkConstrainedPointHandleRepresentation::operator = (
								const vtkConstrainedPointHandleRepresentation&)
{
	assert (0 && "vtkConstrainedPointHandleRepresentation assignment operator is not allowed.");
	return *this;
}	// vtkConstrainedPointHandleRepresentation::vtkConstrainedPointHandleRepresentation


vtkConstrainedPointHandleRepresentation::~vtkConstrainedPointHandleRepresentation ( )
{
}	// vtkConstrainedPointHandleRepresentation::~vtkConstrainedPointHandleRepresentation


vtkConstrainedPointHandleRepresentation*
								vtkConstrainedPointHandleRepresentation::New ( )
{
	return new vtkConstrainedPointHandleRepresentation ( );
}	// vtkConstrainedPointHandleRepresentation::New


int vtkConstrainedPointHandleRepresentation::ComputeInteractionState (
													int X, int Y, int notUsed)
{
  this->VisibilityOn ( ); //actor must be on to be picked
  this->CursorPicker->Pick (X, Y, 0.0, this->Renderer);
  vtkAssemblyPath*	path	= this->CursorPicker->GetPath ( );
  double focus[3];
  this->Cursor3D->GetFocalPoint (focus);
  double d[3];
  this->GetDisplayPosition (d);

  if ( path != NULL )
    {
    this->InteractionState = vtkHandleRepresentation::Nearby;
    }
  else
    {
    this->InteractionState = vtkHandleRepresentation::Outside;
    if ( this->ActiveRepresentation )
      {
      this->VisibilityOff();
      }
    }

	return this->InteractionState;
}	// vtkConstrainedPointHandleRepresentation::ComputeInteractionState


void vtkConstrainedPointHandleRepresentation::WidgetInteraction (
															double eventPos [2])
{
  // Do different things depending on state
  // Calculations everybody does
  double focalPoint[4], pickPoint[4], prevPickPoint[4], startPickPoint[4], z;

  // Compute the two points defining the motion vector
// CP : Bloc d'instructions utiles au resize du widget (clic bouton gauche +
// glisser => modifie la taille du widget) :
   vtkInteractorObserver::ComputeWorldToDisplay(
      this->Renderer,
      this->LastPickPosition[0], 
      this->LastPickPosition[1],
      this->LastPickPosition[2], focalPoint);
  z = focalPoint[2];
  vtkInteractorObserver::ComputeDisplayToWorld(
      this->Renderer, 
      this->LastEventPosition[0], 
      this->LastEventPosition[1], z, prevPickPoint);
  vtkInteractorObserver::ComputeDisplayToWorld(
      this->Renderer, eventPos[0], eventPos[1], z, pickPoint);
// CP : Fin bloc d'instructions utiles au resize

  // Process the motion
  if ( this->InteractionState == vtkHandleRepresentation::Selecting ||
       this->InteractionState == vtkHandleRepresentation::Translating )
    {
      vtkInteractorObserver::ComputeDisplayToWorld(
          this->Renderer, 
          this->StartEventPosition[0], 
          this->StartEventPosition[1], z, startPickPoint);

        vtkDebugMacro( << "Processing widget interaction for translate" );
          
          double newCenterPointRequested[3]; // displayPosition
          double newCenterPoint[3], worldOrient[9];

          // Make a request for the new position.
          this->MoveFocusRequest( prevPickPoint, 
                                  pickPoint, 
                                  eventPos,
                                  newCenterPointRequested);

          vtkDebugMacro( << "Request for computing world position at "
            << "display position of " << newCenterPointRequested[0] 
            << "," << newCenterPointRequested[1] );

          // See what the placer says.
		if (this->PointPlacer->ComputeWorldPosition (
			this->Renderer, newCenterPointRequested, newCenterPoint, worldOrient ))
		{
			// Once the placer has validated us, update the handle position and its bounds.
			double *p = this->GetWorldPosition();

			//Get the motion vector
			double v[3] = { newCenterPoint[0] - p[0], newCenterPoint[1] - p[1], newCenterPoint[2] - p[2] };
			double *bounds = this->Cursor3D->GetModelBounds(), newBounds[6];
			for (int i=0; i<3; i++)
			{
				double	d	= fabs (bounds[2*i+1] - bounds[2*i]) / 2.;
				newBounds[2*i]		= v[i] - d;
				newBounds[2*i+1]	= v[i] + d;
//				newBounds[2*i]   = bounds[2*i]   + v[i];
//				newBounds[2*i+1] = bounds[2*i+1] + v[i];
			}

//			this->Cursor3D->SetModelBounds(newBounds);	// v 3.2.1
			this->SetWorldPosition( newCenterPoint );
		}	// if (this->PointPlacer->ComputeWorldPosition (...
		else
		{
// CP : On ne garde pas cette position (invalide) car c'est là que sera centré la croix au prochain raffraichissement ...
//			this->LastEventPosition[0] = eventPos[0];
//			this->LastEventPosition[1] = eventPos[1];
			return;
		}
    }

  else if ( this->InteractionState == vtkHandleRepresentation::Scaling )
    {
    // Scaling does not change the position of the handle, we needn't
    // ask the placer..
    this->Scale(prevPickPoint, pickPoint, eventPos);
    }

  // Book keeping
  this->LastEventPosition[0] = eventPos[0];
  this->LastEventPosition[1] = eventPos[1];
  
  this->Modified();
}	// vtkConstrainedPointHandleRepresentation::WidgetInteraction


vtkUnstructuredGridPointPlacer*
				vtkConstrainedPointHandleRepresentation::GetUGridPointPlacer ( )
{
	return dynamic_cast<vtkUnstructuredGridPointPlacer*>(GetPointPlacer ( ));
}	// vtkConstrainedPointHandleRepresentation::GetUGridPointPlacer


// =============================================================================
//                    LA CLASSE vtkUnstructuredGridPointPlacer
// =============================================================================

vtkUnstructuredGridPointPlacer::vtkUnstructuredGridPointPlacer ( )
	: vtkPolyDataPointPlacer ( ), Grid (0)
{
}	// vtkUnstructuredGridPointPlacer::vtkUnstructuredGridPointPlacer


vtkUnstructuredGridPointPlacer::vtkUnstructuredGridPointPlacer (
										const vtkUnstructuredGridPointPlacer&)
	: vtkPolyDataPointPlacer ( ), Grid (0)
{
	assert (0 && "vtkUnstructuredGridPointPlacer copy constructor is not allowed.");
}	// vtkUnstructuredGridPointPlacer::vtkUnstructuredGridPointPlacer


vtkUnstructuredGridPointPlacer& vtkUnstructuredGridPointPlacer::operator = (
										const vtkUnstructuredGridPointPlacer&)
{
	assert (0 && "vtkUnstructuredGridPointPlacer assignment operator is not allowed.");
	return *this;
}	// vtkUnstructuredGridPointPlacer::vtkUnstructuredGridPointPlacer


vtkUnstructuredGridPointPlacer::~vtkUnstructuredGridPointPlacer ( )
{
	SetGrid (0);
}	// vtkUnstructuredGridPointPlacer::~vtkUnstructuredGridPointPlacer


vtkUnstructuredGridPointPlacer* vtkUnstructuredGridPointPlacer::New ( )
{
	return new vtkUnstructuredGridPointPlacer ( );
}	// vtkUnstructuredGridPointPlacer::New


int vtkUnstructuredGridPointPlacer::ComputeWorldPosition (
			vtkRenderer* renderer, double displayPos [2], double worldPos [3],
			double notUsed [9])
{
	if ((0 == Grid) || (0 == renderer))
		return 0;

	// Nouvel algo : on retourne le point le + proche dans le plan écran, même
	// s'il n'est pas pické
	const vtkIdType num		= Grid->GetNumberOfPoints ( );
	double			d		= MAXDOUBLE;
	vtkIdType	nearestId	= (vtkIdType)-1;	// Id du point le + proche
	for (vtkIdType i = 0; i < num; i++)
	{
		double	wp [3], dp [3];
		Grid->GetPoint (i, wp);
		renderer->SetWorldPoint (wp [0], wp [1], wp [2], 1.);
		renderer->WorldToDisplay ( );
		renderer->GetDisplayPoint (dp);
		double	d2	= (displayPos [0] - dp [0]) * (displayPos [0] - dp [0]) +
					  (displayPos [1] - dp [1]) * (displayPos [1] - dp [1]);
		if (d2 < d)
		{
			nearestId	= i;
			d			= d2;
		}	// if (d2 < d)
	}	// for (vtkIdType i = 0; i < num; i++)
	Grid->GetPoint (nearestId, worldPos);

	return 1;
}	// vtkUnstructuredGridPointPlacer::ComputeWorldPosition


void vtkUnstructuredGridPointPlacer::SetGrid (vtkUnstructuredGrid* g)
{
	if (g == Grid)
		return;

	if (0 != Grid)
		Grid->UnRegister (this);
	Grid	= g;
	if (0 != Grid)
		Grid->Register (this);
}	// vtkUnstructuredGridPointPlacer::SetGrid



