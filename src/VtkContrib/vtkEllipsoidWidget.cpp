/**
 * Widget de saisie d'une ellipsoïde dans l'espace.
 * @author		Charles PIGNEROL, CEA/DAM/DCLC
 * @date		09/12/2025
 */
 
#include "VtkContrib/vtkEllipsoidWidget.h"

#include <assert.h>
#include <iostream>

#include <vtkMath.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSphereSource.h>
#include <vtkTransform.h>



using namespace std;


// =============================================================================================================================
//                                     LA CLASSE vtkEllipsoidWidget::vtkBoxInteractionCommand
// =============================================================================================================================

vtkEllipsoidWidget::vtkBoxInteractionCommand* vtkEllipsoidWidget::vtkBoxInteractionCommand::New ( )
{
	return new vtkEllipsoidWidget::vtkBoxInteractionCommand ( );
}	// vtkBoxInteractionCommand::New


void vtkEllipsoidWidget::vtkBoxInteractionCommand::Execute (vtkObject* caller, unsigned long id, void* calldata)
{
	if (0 != _widget)
		_widget->UpdateEllipsoid ( );
}	// vtkBoxInteractionCommand::Execute


// =============================================================================================================================
//                                                  LA CLASSE vtkEllipsoidWidget
// =============================================================================================================================

vtkEllipsoidWidget::vtkEllipsoidWidget ( )
	: vtkBoxWidget ( ), _ellipsoid ( ), _ellipsoidSource ( ), _polygonFilter ( ), _ellipsoidMapper ( ), _ellipsoidActor ( ), _interactionCallback ( ), _transform ( )
{
	_transform			= vtkSmartPointer<vtkTransform>::Take (vtkTransform::New ( ));
	_ellipsoid			= vtkSmartPointer<vtkParametricEllipsoid>::Take (vtkParametricEllipsoid::New ( ));
	_ellipsoidSource	= vtkSmartPointer<vtkParametricFunctionSource>::Take (vtkParametricFunctionSource::New ( ));
	_ellipsoidSource->SetParametricFunction (_ellipsoid);
	_polygonFilter		= vtkSmartPointer<vtkPolygonFilter>::Take (vtkPolygonFilter::New ( ));
	_polygonFilter->SetInputData (_ellipsoidSource->GetOutput ( ));
	_ellipsoidMapper	= vtkSmartPointer<vtkPolyDataMapper>::Take (vtkPolyDataMapper::New ( ));
	_ellipsoidActor		= vtkSmartPointer<vtkActor>::Take (vtkActor::New ( ));
	_ellipsoidMapper->SetInputData (_polygonFilter->GetOutput ( ));
	_ellipsoidActor->SetMapper (_ellipsoidMapper);

	// Emplacement non idéal, il faudrait que ce soit dans une méthode type Init ( ) :
	CreateDefaultRepresentation ( );
	
	// Actualiser la représentation de l'ellipsoïde lors des interactions avec le box widget :
	_interactionCallback	= vtkSmartPointer<vtkBoxInteractionCommand>::Take (vtkBoxInteractionCommand::New ( ));
	_interactionCallback->SetWidget (this);
	AddObserver (vtkCommand::InteractionEvent, _interactionCallback);
}	// vtkEllipsoidWidget::vtkEllipsoidWidget


vtkEllipsoidWidget::vtkEllipsoidWidget (const vtkEllipsoidWidget&)
	: vtkBoxWidget ( ), _ellipsoid ( ), _ellipsoidSource ( ), _polygonFilter ( ), _ellipsoidMapper ( ), _ellipsoidActor ( ), _interactionCallback ( ), _transform ( )
{
	assert (0 && "vtkEllipsoidWidget copy constructor is not allowed.");
}	// vtkEllipsoidWidget::vtkEllipsoidWidget


vtkEllipsoidWidget& vtkEllipsoidWidget::operator = (const vtkEllipsoidWidget&)
{
	assert (0 && "vtkEllipsoidWidget assignment operator is not allowed.");
	return *this;
}	// vtkEllipsoidWidget::operator =


vtkEllipsoidWidget::~vtkEllipsoidWidget ( )
{
}	// vtkEllipsoidWidget::~vtkEllipsoidWidget


vtkEllipsoidWidget* vtkEllipsoidWidget::New ( )
{
	return new vtkEllipsoidWidget ( );
}	// vtkEllipsoidWidget::New


void vtkEllipsoidWidget::SetEnabled (int enabled)
{
	vtkRenderer*	renderer	= GetCurrentRenderer ( );
	vtkBoxWidget::SetEnabled (enabled);	// (Dés)affecte this->CurrentRenderer selon enable.
	
	if (true == enabled)
	{
		_polygonFilter->Update ( );
		if (0 != GetCurrentRenderer ( ))
			GetCurrentRenderer ( )->AddActor (_ellipsoidActor);
	}
	else
	{
		if (0 != renderer)
			renderer->RemoveActor (_ellipsoidActor);
	}
}	// vtkEllipsoidWidget::SetEnabled


void vtkEllipsoidWidget::PlaceWidget (double bounds [6])
{
cout << __FILE__ << ' ' << __LINE__ << " vtkEllipsoidWidget::PlaceWidget" << endl;
	vtkBoxWidget::PlaceWidget (bounds);
	
	UpdateEllipsoid ( );
}	// vtkEllipsoidWidget::PlaceWidget


void vtkEllipsoidWidget::PlaceWidget (double xRadius, double yRadius, double zRadius, vtkTransform* transform)
{
cout << __FILE__ << ' ' << __LINE__ << " vtkEllipsoidWidget::PlaceWidget" << endl;
	const double	x	= 0 == transform ? 0. : transform->GetPosition ( )[0];
	const double	y	= 0 == transform ? 0. : transform->GetPosition ( )[1];
	const double	z	= 0 == transform ? 0. : transform->GetPosition ( )[2];
	double	bounds [6]	= { x - xRadius, x + xRadius, y - yRadius, y + yRadius, z - zRadius, z + zRadius };
	vtkBoxWidget::PlaceWidget (bounds);
	vtkSmartPointer<vtkTransform>	t	= vtkSmartPointer<vtkTransform>::New ( );
	t->DeepCopy (transform);
	t->Translate (-x, -y, -z);
	vtkBoxWidget::SetTransform (t);

	if (0 != _ellipsoid)
	{
		_ellipsoid->SetXRadius (xRadius);
		_ellipsoid->SetYRadius (yRadius);
		_ellipsoid->SetZRadius (zRadius);
	}
	if (0 != _ellipsoidActor)
	{
		vtkTransform*	te	= vtkTransform::New ( );
		te->DeepCopy (transform);
		_ellipsoidActor->SetUserTransform (te);
	}
	_ellipsoidSource->Update ( );
	_polygonFilter->Update ( );
}	// vtkEllipsoidWidget::PlaceWidget


void vtkEllipsoidWidget::GetPosition (double& x, double& y, double& z)
{
	x	= this->HandleGeometry [6]->GetCenter ( )[0];
	y	= this->HandleGeometry [6]->GetCenter ( )[1];
	z	= this->HandleGeometry [6]->GetCenter ( )[2];
	cout << __FILE__ << ' ' << __LINE__ << " vtkEllipsoidWidget::GetPosition : " << x << ", " << y << ", " << z << endl;
}	// vtkEllipsoidWidget::GetPosition


double vtkEllipsoidWidget::GetXRadius ( )
{
	return sqrt (vtkMath::Distance2BetweenPoints (this->HandleGeometry [0]->GetCenter ( ), this->HandleGeometry [1]->GetCenter ( ))) / 2.;
}	// vtkEllipsoidWidget::GetXRadius


double vtkEllipsoidWidget::GetYRadius ( )
{
	return sqrt (vtkMath::Distance2BetweenPoints (this->HandleGeometry [2]->GetCenter ( ), this->HandleGeometry [3]->GetCenter ( ))) / 2.;
}	// vtkEllipsoidWidget::GetYRadius


double vtkEllipsoidWidget::GetZRadius ( )
{
	return sqrt (vtkMath::Distance2BetweenPoints (this->HandleGeometry [4]->GetCenter ( ), this->HandleGeometry [5]->GetCenter ( ))) / 2.;
}	// vtkEllipsoidWidget::GetZRadius


void vtkEllipsoidWidget::GetDiscretisation (unsigned int& u, unsigned int& v, unsigned int& w)
{
	if (0 != _ellipsoidSource)
	{
		u	= _ellipsoidSource->GetUResolution ( );
		v	= _ellipsoidSource->GetVResolution ( );
		w	= _ellipsoidSource->GetWResolution ( );
	}	// if (0 != _ellipsoidSource)
}	// vtkEllipsoidWidget::GetDiscretisation


void vtkEllipsoidWidget::SetDiscretisation (unsigned int u, unsigned int v, unsigned int w)
{
	if (0 != _ellipsoidSource)
	{
		_ellipsoidSource->SetUResolution (u);
		_ellipsoidSource->SetVResolution (v);
		_ellipsoidSource->SetWResolution (w);
	}	// if (0 != _ellipsoidSource)
}	// vtkEllipsoidWidget::SetDiscretisation


void vtkEllipsoidWidget::SetEllipsoidOpacity (double opacity)
{
	if ((0 != _ellipsoidActor) && (0 != _ellipsoidActor->GetProperty ( )))
		_ellipsoidActor->GetProperty ( )->SetOpacity (opacity);
}	// vtkEllipsoidWidget::SetEllipsoidOpacity


void vtkEllipsoidWidget::SetEllipsoidColor (double rgb [3])
{
	if ((0 != _ellipsoidActor) && (0 != _ellipsoidActor->GetProperty ( )))
		_ellipsoidActor->GetProperty ( )->SetColor (rgb);
}	// vtkEllipsoidWidget::SetEllipsoidColor


void vtkEllipsoidWidget::SetEllipsoidColor (double r, double g, double b)
{
	double	rgb [3]	= { r, g, b };
	SetEllipsoidColor (rgb);
}	// vtkEllipsoidWidget::SetEllipsoidColor


void vtkEllipsoidWidget::CreateDefaultRepresentation ( )
{
	const double	x		= (InitialBounds [1] + InitialBounds [0]) / 2.;
	const double	y		= (InitialBounds [3] + InitialBounds [2]) / 2.;
	const double	z		= (InitialBounds [5] + InitialBounds [4]) / 2.;
	const double	xRadius	= (InitialBounds [1] - InitialBounds [0]) / 2.;
	const double	yRadius	= (InitialBounds [3] - InitialBounds [2]) / 2.;
	const double	zRadius	= (InitialBounds [5] - InitialBounds [4]) / 2.;
	_ellipsoid->SetXRadius (xRadius);
	_ellipsoid->SetYRadius (yRadius);
	_ellipsoid->SetZRadius (zRadius);
	vtkTransform*	translation	= vtkTransform::New ( );
	translation->Translate (x, y, z);
	_ellipsoidActor->SetUserTransform (translation);
	_ellipsoidSource->SetUResolution (10);
	_ellipsoidSource->SetVResolution (10);
	_ellipsoidSource->SetWResolution (10);
	_ellipsoidSource->Update ( );
	_polygonFilter->Update ( );
}	// vtkEllipsoidWidget::CreateDefaultRepresentation


void vtkEllipsoidWidget::UpdateEllipsoid ( )
{
	const double	xRadius	= sqrt (vtkMath::Distance2BetweenPoints (this->HandleGeometry [0]->GetCenter ( ), this->HandleGeometry [1]->GetCenter ( ))) / 2.;
	const double	yRadius	= sqrt (vtkMath::Distance2BetweenPoints (this->HandleGeometry [2]->GetCenter ( ), this->HandleGeometry [3]->GetCenter ( ))) / 2.;
	const double	zRadius	= sqrt (vtkMath::Distance2BetweenPoints (this->HandleGeometry [4]->GetCenter ( ), this->HandleGeometry [5]->GetCenter ( ))) / 2.;
	vtkBoxWidget::GetTransform (_transform);
	_ellipsoid->SetXRadius (xRadius);
	_ellipsoid->SetYRadius (yRadius);
	_ellipsoid->SetZRadius (zRadius);
	_ellipsoidSource->Update ( );
	_polygonFilter->Update ( );
	vtkTransform*	transformation	= vtkTransform::New ( );
	transformation->Translate (this->HandleGeometry [6]->GetCenter ( ));
// OK POUR LES ROTATIONS : Z X Y
	transformation->RotateZ (_transform->GetOrientation ( )[2]);
	transformation->RotateX (_transform->GetOrientation ( )[0]);
	transformation->RotateY (_transform->GetOrientation ( )[1]);
	_ellipsoidActor->SetUserTransform (transformation);
}	// vtkEllipsoidWidget::UpdateEllipsoid
