#include "VtkContrib/vtkLandmarkActor.h"
#include "VtkContrib/vtkImplicitTorus.h"

#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkMapper.h>
#include <vtkContourFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkSampleFunction.h>
#include <vtkTransform.h>


#include <assert.h>
#include <values.h>
#include <iostream>
using namespace std;

//
// Displays a torus center at (2, 0.5, 0.) along z-axis, ring radius = 0.5, sectio radius = 0.05.
//

int main ( int argc, char *argv[] )
{
	vtkSmartPointer<vtkRenderer>		renderer	= vtkSmartPointer<vtkRenderer>::New ( );
	vtkSmartPointer<vtkRenderWindow>	window		= vtkSmartPointer<vtkRenderWindow>::New ( );
	window->AddRenderer (renderer);
	vtkCamera*							camera		= renderer->GetActiveCamera ( );
	camera->ParallelProjectionOn ( );
	
	vtkSmartPointer<vtkRenderWindowInteractor>	interactor	= vtkSmartPointer<vtkRenderWindowInteractor>::New ( );
	interactor->SetRenderWindow (window);

	// Torus definition :
	const double	center [3]	= { 2., 0.5, 0. };	// Be carefull to set others values if transform usage ...
	const double	ringRadius	= 0.5, crossSectionRadius	= ringRadius / 10;
	const double	bounds [6]	= { center [0] - 1.1 * (ringRadius + crossSectionRadius), center [0] + 1.1 * (ringRadius + crossSectionRadius),
								    center [1] - 1.1 * (ringRadius + crossSectionRadius), center [1] + 1.1 * (ringRadius + crossSectionRadius),
								    center [2] - 1.1 * (ringRadius + crossSectionRadius), center [2] + 1.1 * (ringRadius + crossSectionRadius) };
	vtkSmartPointer<vtkImplicitTorus>	torus		= vtkSmartPointer<vtkImplicitTorus>::New ( );
//	torus->SetCenter ((double*)center);	// Done by transform
	torus->SetRingRadius (ringRadius);
	torus->SetCrossSectionRadius (crossSectionRadius);
	// Place tore in another plane :
	vtkSmartPointer<vtkTransform>	transform	= vtkSmartPointer<vtkTransform>::New ( );
	transform->PostMultiply ( );
	transform->Translate (- center [0], - center [1], - center [2]);
	transform->RotateY (90.);
	torus->SetTransform (transform);
	vtkSmartPointer<vtkSampleFunction>	torusSample	= vtkSmartPointer<vtkSampleFunction>::New ( );
	torusSample->SetImplicitFunction (torus);
	torusSample->SetModelBounds (bounds);
	torusSample->ComputeNormalsOff ( );
	vtkSmartPointer<vtkContourFilter>	torusSurface	= vtkSmartPointer<vtkContourFilter>::New ( );
	torusSurface->SetInputConnection (torusSample->GetOutputPort ( ));
	vtkSmartPointer<vtkPolyDataMapper>	mapper	= vtkSmartPointer<vtkPolyDataMapper>::New ( );
	mapper->SetInputConnection (torusSurface->GetOutputPort ( ));
	mapper->ScalarVisibilityOff ( );
	vtkSmartPointer<vtkActor>			actor	=vtkSmartPointer<vtkActor>::New ( );
	actor->SetMapper (mapper);
	renderer->AddActor (actor);
	
	// A landmark in order to show torus position and size :
	vtkLandmarkActor*	landmarkActor	= vtkLandmarkActor::New ( );
	landmarkActor->SetBounds ((double*)bounds);
	landmarkActor->GetProperty ( )->SetColor (0., 1., 0.);
	landmarkActor->SetXAxisColor (1., 0.67, 0.);
	landmarkActor->SetYAxisColor (0., 0.82, 1.);
	landmarkActor->SetZAxisColor (0., 0., 1.);
	landmarkActor->SetXTitle ("X");
	landmarkActor->SetYTitle ("Y");
	landmarkActor->SetZTitle ("Z");
	landmarkActor->SetXAxisLabelVisibility (1);
	landmarkActor->SetYAxisLabelVisibility (1);
	landmarkActor->SetZAxisLabelVisibility (1);
	landmarkActor->SetXLabelFormat ("%g");
	landmarkActor->SetYLabelFormat ("%g");
	landmarkActor->SetZLabelFormat ("%g");
	landmarkActor->SetXAxisTickVisibility (1);
	landmarkActor->SetYAxisTickVisibility (1);
	landmarkActor->SetZAxisTickVisibility (1);
	landmarkActor->SetXAxisMinorTickVisibility (1);
	landmarkActor->SetYAxisMinorTickVisibility (1);
	landmarkActor->SetZAxisMinorTickVisibility (1);
	landmarkActor->SetTickLocationToOutside ( );
	landmarkActor->SetCamera (renderer->GetActiveCamera ( ));
	landmarkActor->SetDrawXGridlines (1);
	landmarkActor->SetDrawYGridlines (1);
	landmarkActor->SetDrawZGridlines (1);
	landmarkActor->SetCornerOffset (0.);
	landmarkActor->PickableOff ( );
	renderer->AddActor (landmarkActor);

	interactor->Initialize ( );
	window->Render ( );
	renderer->SetBackground(0.1, 0.2, 0.4);
	window->SetSize(300, 300);
	renderer->ResetCamera ( );
	renderer->ResetCameraClippingRange ( );
	window->Render ( );

	interactor->Start ( );

	return EXIT_SUCCESS;

}

