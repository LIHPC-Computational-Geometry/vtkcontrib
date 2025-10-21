/**
 * Widget de saisie d'un tronc dans l'espace.
 * @author		Charles PIGNEROL, CEA/DAM/DCLC
 * @date		03/10/2025
 */
 
#include "VtkContrib/vtkFrustumWidget.h"

#include <assert.h>
#include <iostream>

#include <vtkCellPicker.h>
#include <vtkConeSource.h>
#include <vtkDoubleArray.h>
#include <vtkFeatureEdges.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSphereSource.h>
#include <vtkTubeFilter.h>

#include <vtkLineSource.h>
#include <vtkOutlineFilter.h>

using namespace std;


// =============================================================================================================================
//                                        LA CLASSE vtkFrustumWidget::vtkInternalPlaneWidget
// =============================================================================================================================

vtkFrustumWidget::vtkInternalPlaneWidget* vtkFrustumWidget::vtkInternalPlaneWidget::New ( )
{
	return new vtkFrustumWidget::vtkInternalPlaneWidget ( );
}	// vtkInternalPlaneWidget::New


vtkFrustumWidget::vtkInternalPlaneWidget::vtkInternalPlaneWidget ( )
	: vtkImplicitPlaneWidget ( ), _placing (false)
{
	// Restreindre l'affichage des 6 plans au tronc => on n'affiche plus le contour du plan de vtkImplicitPlaneWidget.
	if (0 != this->EdgesActor)
		this->EdgesActor->VisibilityOff ( );
	if (0 != this->OutlineActor)
		this->OutlineActor->VisibilityOff ( );

	// On a 6 plans interactifs => pour les opérations de sélection via les 6 manipulateurs, avoir une acuité assez fine
	// lors de leur utilisation => on désactive les sélections "grossières" :
	if (0 != this->Picker)
	{
		if (0 != this->CutActor)
			this->Picker->DeletePickList (this->CutActor);
		if (0 != this->OutlineActor)
			this->Picker->DeletePickList (this->OutlineActor);
	}	// if (0 != this->Picker)
}	// vtkInternalPlaneWidget::vtkInternalPlaneWidget


vtkFrustumWidget::vtkInternalPlaneWidget::vtkInternalPlaneWidget (const vtkFrustumWidget::vtkInternalPlaneWidget&)
	: vtkImplicitPlaneWidget ( ), _placing (false)
{
	assert (0 && "vtkInternalPlaneWidget copy constructor is not allowed.");
}	// vtkInternalPlaneWidget::vtkInternalPlaneWidget


vtkFrustumWidget::vtkInternalPlaneWidget& vtkFrustumWidget::vtkInternalPlaneWidget::operator = (const vtkFrustumWidget::vtkInternalPlaneWidget&)
{
	assert (0 && "vtkFrustumWidget::vtkInternalPlaneWidget assignment operator is not allowed.");
	return *this;
}	// vtkInternalPlaneWidget::operator =


vtkFrustumWidget::vtkInternalPlaneWidget::~vtkInternalPlaneWidget ( )
{
}	// vtkInternalPlaneWidget::~vtkInternalPlaneWidget


void vtkFrustumWidget::vtkInternalPlaneWidget::PlaceWidget (double bounds [6])
{
	_placing	= true;
	vtkImplicitPlaneWidget::PlaceWidget (bounds);
	_placing	= false;
}	// vtkInternalPlaneWidget::PlaceWidget


void vtkFrustumWidget::vtkInternalPlaneWidget::SizeHandles ( )
{
	if (false == _placing)
		return;

	// Code VTK 7.1.1 légèrement retouché :
	double radius = this->vtk3DWidget::SizeHandles(1.);	// 1.35); code VTK 7.1.1

	this->ConeSource->SetHeight(2.0*radius);
	this->ConeSource->SetRadius(radius);
	this->ConeSource2->SetHeight(2.0*radius);
	this->ConeSource2->SetRadius(radius);

	this->Sphere->SetRadius(radius);

	this->EdgesTuber->SetRadius(0.25*radius);
}	// vtkInternalPlaneWidget::SizeHandles


void vtkFrustumWidget::vtkInternalPlaneWidget::UpdateRepresentation (double origin [3])
{
	// Issu du code de vtkImplicitPlaneWidget::UpdateRepresentation ( ) de VTK 7.1.1, mais en prnant pour origine celle transmise en argument.
  if ( ! this->CurrentRenderer )
  {
    return;
  }

//  double *origin = this->Plane->GetOrigin();
  double *normal = this->Plane->GetNormal();
  double p2[3];
/*
  if( !this->OutsideBounds )
  {
    double *bounds = this->GetInput()->GetBounds();
    for (int i=0; i<3; i++)
    {
      if ( origin[i] < bounds[2*i] )
      {
        origin[i] = bounds[2*i];
      }
      else if ( origin[i] > bounds[2*i+1] )
      {
        origin[i] = bounds[2*i+1];
      }
    }
  }
*/
  // Setup the plane normal
  double d = this->Outline->GetOutput()->GetLength();

  const double ratio = this->DiagonalRatio;
  p2[0] = origin[0] + ratio * d * normal[0];
  p2[1] = origin[1] + ratio * d * normal[1];
  p2[2] = origin[2] + ratio * d * normal[2];

  this->LineSource->SetPoint1(origin);
  this->LineSource->SetPoint2(p2);
  this->ConeSource->SetCenter(p2);
  this->ConeSource->SetDirection(normal);

  p2[0] = origin[0] - ratio * d * normal[0];
  p2[1] = origin[1] - ratio * d * normal[1];
  p2[2] = origin[2] - ratio * d * normal[2];

  this->LineSource2->SetPoint1(origin[0],origin[1],origin[2]);
  this->LineSource2->SetPoint2(p2);
  this->ConeSource2->SetCenter(p2);
  this->ConeSource2->SetDirection(normal[0],normal[1],normal[2]);

  // Set up the position handle
  this->Sphere->SetCenter(origin[0],origin[1],origin[2]);

  // Control the look of the edges
  if ( this->Tubing )
  {
    this->EdgesMapper->SetInputConnection(
      this->EdgesTuber->GetOutputPort());
  }
  else
  {
    this->EdgesMapper->SetInputConnection(
      this->Edges->GetOutputPort());
  }
}	// vtkInternalPlaneWidget::UpdateRepresentation


double vtkFrustumWidget::vtkInternalPlaneWidget::GetHandleRadius ( )
{
	return 0 == this->EdgesTuber ? 0. : this->EdgesTuber->GetRadius ( );
}	// vtkInternalPlaneWidget::GetHandleRadius


void vtkFrustumWidget::vtkInternalPlaneWidget::SetInteractorColor (double rgb [3])
{
	if (0 != GetNormalProperty ( ))
		GetNormalProperty ( )->SetColor (rgb);
}	// vtkInternalPlaneWidget::SetInteractorColor


void vtkFrustumWidget::vtkInternalPlaneWidget::SetInteractorEnabled (bool enabled)
{
	if (0 != this->Picker)
	{
		if (true == enabled)
		{
			if (0 != this->ConeActor)
				this->Picker->AddPickList (this->ConeActor);
			if (0 != this->ConeActor)
				this->Picker->AddPickList (this->ConeActor2);
			if (0 != this->LineActor)
				this->Picker->AddPickList (this->LineActor);
			if (0 != this->LineActor2)
				this->Picker->AddPickList (this->LineActor2);
			if (0 != this->SphereActor)
				this->Picker->AddPickList (this->SphereActor);
		}
		else
		{
			if (0 != this->ConeActor)
				this->Picker->DeletePickList (this->ConeActor);
			if (0 != this->ConeActor)
				this->Picker->DeletePickList (this->ConeActor2);
			if (0 != this->LineActor)
				this->Picker->DeletePickList (this->LineActor);
			if (0 != this->LineActor2)
				this->Picker->DeletePickList (this->LineActor2);
			if (0 != this->SphereActor)
				this->Picker->DeletePickList (this->SphereActor);
		}	// if (true == enabled)
	}	// if (0 != this->Picker)
	else
		vtkErrorMacro ("vtkFrustumWidget::SetInteractorEnabled : null picker.")
}	// vtkInternalPlaneWidget::SetInteractorEnabled

		
// =============================================================================================================================
//                                            LA CLASSE vtkPlaneWidgetCallbackCommand
// =============================================================================================================================

void vtkPlaneWidgetCallbackCommand::Execute (vtkObject* caller, unsigned long id, void* calldata)
{
	if (0 != _frustumWidget)
		_frustumWidget->Update (static_cast<vtkFrustumWidget::vtkInternalPlaneWidget*>(caller));
}	// vtkPlaneWidgetCallbackCommand::Execute


// =============================================================================================================================
//                                                  LA CLASSE vtkFrustumWidget
// =============================================================================================================================

vtkFrustumWidget::vtkFrustumWidget ( )
	: vtk3DWidget ( ), _parallelPlanes ( )
{
	_frustum		= vtkSmartPointer<vtkFrustumSource>::Take (vtkFrustumSource::New ( ));
	_planes			= vtkSmartPointer<vtkPlanes>::Take (vtkPlanes::New ( ));
	_frustumMapper	= vtkSmartPointer<vtkPolyDataMapper>::Take (vtkPolyDataMapper::New ( ));
	_frustumActor	= vtkSmartPointer<vtkActor>::Take (vtkActor::New ( ));
	_frustumMapper->SetInputConnection (_frustum->GetOutputPort ( ));
	_frustumActor->SetMapper (_frustumMapper);
	_contourFilter	= vtkSmartPointer<vtkTubeFilter>::Take (vtkTubeFilter::New ( ));
	_contourMapper	= vtkSmartPointer<vtkPolyDataMapper>::Take (vtkPolyDataMapper::New ( ));
	_contourActor	= vtkSmartPointer<vtkActor>::Take (vtkActor::New ( ));
	_planeCallback	= vtkSmartPointer<vtkPlaneWidgetCallbackCommand>::Take (vtkPlaneWidgetCallbackCommand::New ( ));
	_planeCallback->SetFrustumWidget (this);
	for (int i = 0; i < 6; i++)
	{
		_planeWidgets.push_back (vtkSmartPointer<vtkFrustumWidget::vtkInternalPlaneWidget>::Take (vtkFrustumWidget::vtkInternalPlaneWidget::New ( )));
		_planeWidgets [i]->AddObserver (vtkCommand::InteractionEvent, _planeCallback);
	}	// for (int i = 0; i < 6; i++)
	_tmpPlane	= vtkSmartPointer<vtkPlane>::Take (vtkPlane::New ( ));
	vtkSmartPointer<vtkFeatureEdges>	edges	= vtkSmartPointer<vtkFeatureEdges>::Take (vtkFeatureEdges::New ( ));
	edges->SetInputConnection (_frustum->GetOutputPort ( ));
	_contourFilter->SetInputConnection (edges->GetOutputPort ( ));
	_contourFilter->SetNumberOfSides (12);
	_contourFilter->SetRadius (5 * _planeWidgets [0]->GetHandleRadius ( ));
	_contourMapper->SetInputConnection (_contourFilter->GetOutputPort ( ));
	_contourActor->SetMapper (_contourMapper);

	// Emplacement non idéal, il faudrait que ce soit dans une méthode type Init ( ) :
	CreateDefaultRepresentation ( );
}	// vtkFrustumWidget::vtkFrustumWidget


vtkFrustumWidget::vtkFrustumWidget (const vtkFrustumWidget&)
	: vtk3DWidget ( ), _parallelPlanes ( )
{
	assert (0 && "vtkFrustumWidget copy constructor is not allowed.");
}	// vtkFrustumWidget::vtkFrustumWidget


vtkFrustumWidget& vtkFrustumWidget::operator = (const vtkFrustumWidget&)
{
	assert (0 && "vtkFrustumWidget assignment operator is not allowed.");
	return *this;
}	// vtkFrustumWidget::operator =


vtkFrustumWidget::~vtkFrustumWidget ( )
{
	_planeCallback->SetFrustumWidget (0);
}	// vtkFrustumWidget::~vtkFrustumWidget


vtkFrustumWidget* vtkFrustumWidget::New ( )
{
	return new vtkFrustumWidget ( );
}	// vtkFrustumWidget::New


void vtkFrustumWidget::SetInteractor (vtkRenderWindowInteractor* iren)
{
	vtk3DWidget::SetInteractor (iren);
	for (vector<vtkSmartPointer<vtkFrustumWidget::vtkInternalPlaneWidget>>::iterator it = _planeWidgets.begin ( ); _planeWidgets.end ( ) != it; it++)
		(*it)->SetInteractor (iren);
	SetCurrentRenderer (0 == iren ? 0 : iren->FindPokedRenderer (0, 0));
}	// vtkFrustumWidget::SetInteractor


void vtkFrustumWidget::SetEnabled (int enabled)
{
	vtk3DWidget::SetEnabled (enabled);
	
	for (vector<vtkSmartPointer<vtkFrustumWidget::vtkInternalPlaneWidget>>::iterator it = _planeWidgets.begin ( ); _planeWidgets.end ( ) != it; it++)
		(*it)->SetEnabled (enabled);

	if (0 != GetCurrentRenderer ( ))
	{
		if (true == enabled)
		{
			GetCurrentRenderer ( )->AddActor (_frustumActor);
			GetCurrentRenderer ( )->AddActor (_contourActor);
		}
		else
		{
			GetCurrentRenderer ( )->RemoveActor (_frustumActor);
			GetCurrentRenderer ( )->RemoveActor (_contourActor);
		}
	}	// if (0 != GetCurrentRenderer ( ))
	
	Update (0);
}	// vtkFrustumWidget::SetEnabled


void vtkFrustumWidget::PlaceWidget (double bounds [6])
{
	int	i	= 0;
	for (vector<vtkSmartPointer<vtkFrustumWidget::vtkInternalPlaneWidget>>::iterator it = _planeWidgets.begin ( ); _planeWidgets.end ( ) != it; it++, i++)
	{
		double	origin [3]	= { (bounds [0] + bounds [1]) / 2., (bounds [2] + bounds [3]) / 2., (bounds [4] + bounds [5]) / 2. };
		switch (i)
		{
			case 0	: origin [0]	= bounds [0];	break;
			case 1	: origin [0]	= bounds [1];	break;
			case 2	: origin [1]	= bounds [2];	break;
			case 3	: origin [1]	= bounds [3];	break;
			case 4	: origin [2]	= bounds [4];	break;
			case 5	: origin [2]	= bounds [5];	break;
		}	// switch (i)
// On est tenté d'appeler SetOrigin et PlaceWidget pour chacun des plans, mais vtkImplicitPlaneWidget::PlaceWidget ne prend pas en compte le SetOrigin et recalcule
// origine et normale au plan => on récupère ceux demandés et on les utilise :
//		(*it)->SetOrigin (origin);
//		(*it)->PlaceWidget (bounds);
		(*it)->GetPlane (_tmpPlane);
		double	val [3]	= { 0., 0., 0. };
		_tmpPlane->GetOrigin (val);
		(*it)->PlaceWidget (bounds);
		(*it)->SetOrigin (origin);
		(*it)->SetNormal (val [0], val [1], val [2]);
	}	// for (vector<vtkSmartPointer<vtkFrustumWidget::vtkInternalPlaneWidget>>::iterator it = _planeWidgets.begin ( ); _planeWidgets.end ( ) != it; it++, i++)
}	// vtkFrustumWidget::PlaceWidget


void vtkFrustumWidget::SetPlaceFactor (double factor)
{
	vtk3DWidget::SetPlaceFactor (factor);
	
	for (vector<vtkSmartPointer<vtkFrustumWidget::vtkInternalPlaneWidget>>::iterator it = _planeWidgets.begin ( ); _planeWidgets.end ( ) != it; it++)
		(*it)->SetPlaceFactor (factor);
}	// vtkFrustumWidget::SetPlaceFactor


void vtkFrustumWidget::SetPlane (SIDE p, double origin [3], double normal [3])
{
	vtkFrustumWidget::vtkInternalPlaneWidget*	planeWidget	= GetPlane (p);
	if (0 != planeWidget)
	{
		planeWidget->SetOrigin (origin);
		planeWidget->SetNormal (normal);
		Update (planeWidget);
	}	// if (0 != planeWidget)
	else
		vtkErrorMacro ("vtkFrustumWidget::SetPlane : 0 <= p <= 5 is not verified. p =" << p)
}	// vtkFrustumWidget::SetPlane


void vtkFrustumWidget::GetPlane (SIDE p, double origin [3], double normal [3])
{
	vtkFrustumWidget::vtkInternalPlaneWidget*	planeWidget	= GetPlane (p);
	if (0 != planeWidget)
	{
		planeWidget->GetOrigin (origin);
		planeWidget->GetNormal (normal);
	}	// if (0 != planeWidget)
	else
		vtkErrorMacro ("vtkFrustumWidget::GetPlane : 0 <= p <= 5 is not verified. p =" << p)
}	// vtkFrustumWidget::GetPlane


void vtkFrustumWidget::SetPlaneOrigin (SIDE p, double origin [3])
{
	vtkFrustumWidget::vtkInternalPlaneWidget*	planeWidget	= GetPlane (p);
	if (0 != planeWidget)
	{
		planeWidget->SetOrigin (origin);
		Update (planeWidget);
	}	// if (0 != planeWidget)
	else
		vtkErrorMacro ("vtkFrustumWidget::SetPlane : 0 <= p <= 5 is not verified. p =" << p)
}	// vtkFrustumWidget::SetPlaneOrigin


void vtkFrustumWidget::GetPlaneOrigin (SIDE p, double origin [3])
{
	vtkFrustumWidget::vtkInternalPlaneWidget*	planeWidget	= GetPlane (p);
	if (0 != planeWidget)
		planeWidget->GetOrigin (origin);
	else
		vtkErrorMacro ("vtkFrustumWidget::GetPlaneOrigin : 0 <= p <= 5 is not verified. p =" << p)
}	// vtkFrustumWidget::GetPlaneOrigin


void vtkFrustumWidget::SetPlaneNormal (SIDE p, double normal [3])
{
	vtkFrustumWidget::vtkInternalPlaneWidget*	planeWidget	= GetPlane (p);
	if (0 != planeWidget)
	{
		planeWidget->SetNormal (normal);
		Update (planeWidget);
	}	// if (0 != planeWidget)
	else
		vtkErrorMacro ("vtkFrustumWidget::SetPlane : 0 <= p <= 5 is not verified. p =" << p)
}	// vtkFrustumWidget::SetPlaneNormal


void vtkFrustumWidget::GetPlaneNormal (SIDE p, double normal [3])
{
	vtkFrustumWidget::vtkInternalPlaneWidget*	planeWidget	= GetPlane (p);
	if (0 != planeWidget)
		planeWidget->GetNormal (normal);
	else
		vtkErrorMacro ("vtkFrustumWidget::GetPlaneNormal : 0 <= p <= 5 is not verified. p =" << p)
}	// vtkFrustumWidget::GetPlaneNormal


void vtkFrustumWidget::SetOpacity (double opacity)
{
	if ((0 != _frustumActor) && (0 != _frustumActor->GetProperty ( )))
		_frustumActor->GetProperty ( )->SetOpacity (opacity);
}	// vtkFrustumWidget::SetOpacity


void vtkFrustumWidget::SetInteractorColor (SIDE axis, double rgb [3])
{
	if ((axis >= _planeWidgets.size ( )) || (0 == _planeWidgets [axis]))
	{
		vtkErrorMacro ("vtkFrustumWidget::SetInteractorColor : 0 <= axis <= " << _planeWidgets.size ( ) << " is not verified. axis =" << axis)
		return;
	}	// if ((axis >= _planeWidgets.size ( )) || (0 == _planeWidgets [axis]))

	switch (axis)
	{
		case LEFT	:
		case RIGHT	:
			_planeWidgets [0]->SetInteractorColor (rgb);
			_planeWidgets [1]->SetInteractorColor (rgb);
			break;
		case BOTTOM	:
		case TOP	:
			_planeWidgets [2]->SetInteractorColor (rgb);
			_planeWidgets [3]->SetInteractorColor (rgb);
			break;
		default		:
			_planeWidgets [4]->SetInteractorColor (rgb);
			_planeWidgets [5]->SetInteractorColor (rgb);
	}	// switch (axis)
}	// vtkFrustumWidget::SetInteractorColor


void vtkFrustumWidget::SetInteractorEnabled (SIDE p, bool enabled)
{
	vtkFrustumWidget::vtkInternalPlaneWidget*	planeWidget	= GetPlane (p);
	if (0 != planeWidget)
		planeWidget->SetInteractorEnabled (enabled);
	else
		vtkErrorMacro ("vtkFrustumWidget::SetInteractorEnabled : 0 <= p <= 5 is not verified. p =" << p)
}	// vtkFrustumWidget::SetInteractorEnabled


void vtkFrustumWidget::GetBounds (double bounds [6])
{
	vtkPolyData*	frustum	= 0 == _frustum ? 0 : _frustum->GetOutput ( );

	if (0 == frustum)
		vtkErrorMacro ("vtkFrustumWidget::GetBounds : null frustum.")
	else
	{
		// frustum->GetBounds (bounds);
		/* On n'utilise pas frustum->GetBounds (bounds) car notre polydata contient des lignes qui parfois, de manière non reproductible,
		 * sortent carrément du domaine observé, à savoir qu'une composante d'une coordonnée peut être de l'ordre de 10E+30 ! Parfois veut ici
		 * dire environ 1 fois sur 15, et c'est semble-t-il toujours au premier appel de cette fonction. Problème d'initialisation d'une 
		 * variable statique ???
		 * => on recalcule ici la boite englobante du domaine observé à partir de ses seuls (6) polygones :
		 */
		for (int i = 0; i < 3; i++)
		{
			bounds [2 * i]		= DBL_MAX;
			bounds [2 * i + 1]	= -DBL_MAX;
		}	// for (int i = 0; i < 3; i++)
		for (int i = 0; i < frustum->GetNumberOfCells ( ); i++)
		{
			vtkCell*			cell	= frustum->GetCell (i);
			const VTKCellType	type	= (VTKCellType)cell->GetCellType ( );
			if ((type < VTK_TRIANGLE) || (type > VTK_QUAD))
				continue;

			double	cellBounds [6]	= { DBL_MAX, -DBL_MAX, DBL_MAX, -DBL_MAX, DBL_MAX, -DBL_MAX };
			cell->GetBounds (cellBounds);
			for (int j = 0; j < 3; j++)
			{
				bounds [2 * j]		= min (bounds [2 * j], cellBounds [2 * j]);
				bounds [2 * j + 1]	= max (bounds [2 * j + 1], cellBounds [2 * j + 1]);
			}	// for (int j = 0; j < 3; j++)
		}	// for (int i = 0; i < frustum->GetNumberOfCells ( ); i++)
	}	// else if (0 == frustum)
}	// vtkFrustumWidget::GetBounds


const vtkPlanes& vtkFrustumWidget::GetPlanes ( ) const
{
	return *(_planes.GetPointer ( ));
}	// vtkFrustumWidget::GetPlane


vtkPlanes& vtkFrustumWidget::GetPlanes ( )
{
	return *(_planes.GetPointer ( ));
}	// vtkFrustumWidget::GetPlane


void vtkFrustumWidget::SetPlanes (vtkPlanes& planes)
{
	vtkPoints*		points	= planes.GetPoints ( );
	vtkDataArray*	normals	= planes.GetNormals ( );
	if ((6 == planes.GetNumberOfPlanes ( )) && (0 != points) && (0 != normals) && (6 == points->GetNumberOfPoints ( )) && (6 == normals->GetNumberOfTuples ( )))
	{
		for (int p = 0; p < 6; p++)
		{
			double	coords [3]	= { 0., 0., 0. };
			double*	normal		= normals->GetTuple3 (p);
			points->GetPoint (p, coords);
			vtkFrustumWidget::vtkInternalPlaneWidget*	planeWidget	= GetPlane ((SIDE)p);
			if (0 != planeWidget)
			{
				planeWidget->SetOrigin (coords);
				planeWidget->SetNormal (normal);
			}	// if (0 != planeWidget)
			SetPlane ((SIDE)p, coords, normal);	// => Update parallel planes
		}	// for (int p = 0; p < 6; p++)

		Update (0);
	}
	else
		vtkErrorMacro ("vtkFrustumWidget::SetPlane : 0 <= planes.GetNumberOfPlanes ( ) <= 5 is not verified. p =" << planes.GetNumberOfPlanes ( ))
}	// vtkFrustumWidget::SetPlanes


void vtkFrustumWidget::AddParallelPlanes (SIDE p1, SIDE p2)
{
	_parallelPlanes.insert (pair<int, int>(min ((int)p1, (int)p2), max ((int)p1, (int)p2)));
}	// vtkFrustumWidget::AddParallelPlanes


void vtkFrustumWidget::RemoveParallelPlanes (SIDE p1, SIDE p2)
{
	_parallelPlanes.erase (min ((int)p1, (int)p2));
}	// vtkFrustumWidget::RemoveParallelPlanes


void vtkFrustumWidget::AddParallelPlanes (SIDE p)
{
	switch (p)
	{
		case LEFT	:
		case RIGHT	: AddParallelPlanes (LEFT, RIGHT);	break;
		case BOTTOM	:
		case TOP	: AddParallelPlanes (BOTTOM, TOP);	break;
		case FRONT	:
		case BACK	: AddParallelPlanes (FRONT, BACK);	break;
		default		: vtkErrorMacro ("vtkFrustumWidget::AddParallelPlanes : 0 <= p <= 5 is not verified. p =" << p)
	}	// switch (p)
}	// vtkFrustumWidget::AddParallelPlanes


void vtkFrustumWidget::RemoveParallelPlanes (SIDE p)
{
	switch (p)
	{
		case LEFT	:
		case RIGHT	: RemoveParallelPlanes (LEFT, RIGHT);	break;
		case BOTTOM	:
		case TOP	: RemoveParallelPlanes (BOTTOM, TOP);	break;
		case FRONT	:
		case BACK	: RemoveParallelPlanes (FRONT, BACK);	break;
		default		: vtkErrorMacro ("vtkFrustumWidget::RemoveParallelPlanes : 0 <= p <= 5 is not verified. p =" << p)
	}	// switch (p)
}	// vtkFrustumWidget::RemoveParallelPlanes


std::map<int, int> vtkFrustumWidget::GetParallelPlanes ( ) const
{
	return _parallelPlanes;
}	// vtkFrustumWidget::GetParallelPlanes


void vtkFrustumWidget::CreateDefaultRepresentation ( )
{	// vtkFrustumSource::GetPlanes ( ) : The 6 planes are defined in this order: left,right,bottom,top,far,near.
	vtkSmartPointer<vtkPoints>		points	= vtkSmartPointer<vtkPoints>::Take (vtkPoints::New ( ));
	vtkSmartPointer<vtkDoubleArray>	normals	= vtkSmartPointer<vtkDoubleArray>::Take (vtkDoubleArray::New ( ));
	normals->SetNumberOfComponents (3);
	for (int i = 0; i < 6; i++)
	{
		double	coords [3]	= { 0., 0., 0. };
		switch (i)
		{
			case 0 : coords [0]	= -1.;break;
			case 1 : coords [0]	= 1.; break;
			case 2 : coords [1]	= -1.;break;
			case 3 : coords [1]	= 1.; break;
			case 4 : coords [2]	= -1.; break;
			case 5 : coords [2]	= 1.; break;
		}	// switch (i)
		points->InsertNextPoint (coords);
		normals->InsertNextTuple3 (coords [0], coords [1], coords [2]);
		_planeWidgets [i]->SetOrigin (coords);
		_planeWidgets [i]->SetNormal (coords);
		_planeWidgets [i]->TubingOff ( );
		_planeWidgets [i]->DrawPlaneOff ( );
		_planeWidgets [i]->OriginTranslationOff ( );
		_planeWidgets [i]->OutlineTranslationOff ( );

	}	// for (int i = 0; i < 6; i++)
	_planes->SetPoints (points);
	_planes->SetNormals (normals);
	_frustum->SetPlanes (_planes);
	_frustum->Update ( );
	// On confère à ce widget le look de la classe vtkImplicitPlaneWidget :
	_frustumActor->SetProperty (_planeWidgets [0]->GetPlaneProperty ( ));
}	// vtkFrustumWidget::CreateDefaultRepresentation


vtkFrustumWidget::vtkInternalPlaneWidget* vtkFrustumWidget::GetPlane (SIDE p)
{
	return p >= _planeWidgets.size ( ) ? 0 : _planeWidgets [p];
}	// vtkFrustumWidget::GetPlane


const vtkFrustumWidget::vtkInternalPlaneWidget* vtkFrustumWidget::GetPlane (SIDE p) const
{
	return p >= _planeWidgets.size ( ) ? 0 : _planeWidgets [p];
}	// vtkFrustumWidget::GetPlane


void vtkFrustumWidget::Update (vtkFrustumWidget::vtkInternalPlaneWidget* planeWidget)
{
	vtkPoints*		points	= _planes->GetPoints ( );
	vtkDataArray*	normals	= _planes->GetNormals ( );

	// MAJ normale opposée si nécessaire :
	if (0 != planeWidget)
	{
		double	normal [3]	= { 0., 0., 0. };
		planeWidget->GetNormal (normal);
		for (int i = 0; i < 3; i++)
			normal [i]	= -normal [i];

		for (map<int, int>::const_iterator itp = _parallelPlanes.begin ( ); _parallelPlanes.end ( ) != itp; itp++)
		{
			if (planeWidget == _planeWidgets [(*itp).first])
				_planeWidgets [(*itp).second]->SetNormal (normal);
			if (planeWidget == _planeWidgets [(*itp).second])
				_planeWidgets [(*itp).first]->SetNormal (normal);
		}	// for (map<int, int>::const_iterator itp = _parallelPlanes.begin ( ); _parallelPlanes.end ( ) != itp; itp++)
	}	// if (0 != planeWidget)

	// Actualisation du tronc affiché :
	for (int i = 0; i < 6; i++)
	{
		_planeWidgets [i]->GetPlane (_tmpPlane);
		double	val [3]	= { 0., 0., 0. };
		_tmpPlane->GetOrigin (val);
		points->SetPoint (i, val);
		_tmpPlane->GetNormal (val);
		normals->SetTuple3 (i, val [0], val [1], val [2]);
	}	// for (int i = 0; i < 6; i++)
	_planes->SetPoints (points);
	_planes->SetNormals (normals);
	_frustum->SetPlanes (_planes);
	_frustum->Modified ( );
	_frustum->Update ( );

	// Recentrer les manipulateurs sur leur face :
	vtkPolyData*	frustum	= _frustum->GetOutput ( );
	if ((0 == frustum) || (6 != frustum->GetNumberOfPolys ( )))
		return;

	int		currentCell		= 0;
	bool	recentered [6]	= { false, false, false, false, false, false };	// Eviter de modifier 2 fois le même plan (et 0 fois un autre).
	for (int i = 0; i < frustum->GetNumberOfCells ( ); i++)
	{
		vtkCell*	cell	= frustum->GetCell (i);
		if ((cell->GetCellType ( ) < VTK_TRIANGLE) || (cell->GetCellType ( ) > VTK_QUAD))
			continue;

		double	bounds [6]	= { 0., 0., 0., 0., 0., 0. };
		cell->GetBounds (bounds);
		double	center [3]	= { (bounds [0] + bounds [1]) / 2., (bounds [2] + bounds [3]) / 2., (bounds [4] + bounds [5]) / 2. };
		double	distance	= DBL_MAX;
		int		plane		= -1;
		for (int p = 0; p < 6; p++)
		{
			_planeWidgets [p]->GetPlane (_tmpPlane);
			double	d	= _tmpPlane->DistanceToPlane (center);
			if ((d < distance) && (false == recentered [p]))
			{
				distance	= d;
				plane		= p;
			}	// if ((d < distance) && (false == recentered [p]))
		}	// for (int p = 0; p < 6; p++)
		if ((plane >= 0) && (plane < 6))
		{
			recentered [plane]	= true;
			_planeWidgets [plane]->UpdateRepresentation (center);
		}
		currentCell++;
	}	// for (int i = 0; i < frustum->GetNumberOfCells ( ); i++)
	
	this->InvokeEvent (vtkCommand::InteractionEvent, NULL);
}	// vtkFrustumWidget::Update
