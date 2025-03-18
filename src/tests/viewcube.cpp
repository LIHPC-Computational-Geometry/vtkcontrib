//
// Petit programme test des viewcube de cette bibliothèque.
//


#include <VtkContrib/vtkViewCubeActor.h>
#include <VtkContrib/vtkTrihedronCommand.h>

#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCylinderSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>

#include <assert.h>
#include <iostream>
using namespace std;


int main ( int argc, char *argv[] )
{
	vtkSmartPointer<vtkRenderer>		theaterRenderer		= vtkSmartPointer<vtkRenderer>::New ( );
	vtkSmartPointer<vtkRenderer>		viewCubeRenderer	= vtkSmartPointer<vtkRenderer>::New ( );
	viewCubeRenderer->InteractiveOff ( );
	viewCubeRenderer->SetViewport (0, 0, 0.2, 0.2);
	viewCubeRenderer->SetLayer (1);
	assert (0 != viewCubeRenderer->GetActiveCamera ( ));
	viewCubeRenderer->GetActiveCamera ( )->ParallelProjectionOn ( );
	vtkTrihedronCommand*	trihedronCommand	= vtkTrihedronCommand::New ( );
	trihedronCommand->SetRenderer (viewCubeRenderer);
	unsigned long	trihedronCommandTag	= theaterRenderer->AddObserver (vtkCommand::StartEvent, trihedronCommand);
	vtkSmartPointer<vtkRenderWindow>        renWin   =vtkSmartPointer<vtkRenderWindow>::New ( );
	renWin->SetNumberOfLayers (2);
	renWin->AddRenderer (theaterRenderer);
	renWin->AddRenderer (viewCubeRenderer);

	vtkSmartPointer<vtkRenderWindowInteractor>	iren = vtkSmartPointer<vtkRenderWindowInteractor>::New ( );
	iren->Initialize ( );
	iren->SetRenderWindow(renWin);

	// Quelque chose à afficher dans la scène :
	double	center [3]	= { 1.5, -2., 1. };
	double	radius		= 0.5, height	= 2.;
	vtkCylinderSource*	cylinder	= vtkCylinderSource::New ( );
	cylinder->SetResolution (36);
	cylinder->SetCenter (center);
	cylinder->SetRadius (radius);
	cylinder->SetHeight (height);

	vtkPolyDataMapper*	mapper	= vtkPolyDataMapper::New ( );
	mapper->SetInputConnection (cylinder->GetOutputPort ( ));
	mapper->ScalarVisibilityOff ( );
#if defined(VTK5) || defined(VTK_7)
	mapper->SetImmediateModeRendering (false);
#endif	
	vtkActor*				actor	= vtkActor::New ( );
	actor->RotateZ (90.);	// Le long de X (Z en axisymétrique)
	actor->GetProperty ( )->SetColor (0., 1/170., 1.);
	actor->SetMapper (mapper);
	theaterRenderer->AddActor (actor);

	// Les trièdres :
	vtkViewCubeActor*	viewCube	= vtkViewCubeActor::New ( );
	viewCube->PickableOn ( );
	vtkTransform*	transform	= vtkTransform::New ( );
	transform->PostMultiply ( );
	transform->Translate (-0.5, -0.5, -0.5);
	viewCube->SetTransform (transform);
	transform->Delete ( );	transform	= 0;
	viewCubeRenderer->AddActor (viewCube);
	viewCube->SetRenderers (viewCubeRenderer, theaterRenderer);

	// Affichage et main loop :
	renWin->Render ( );
	theaterRenderer->SetBackground (1., 1., 1.);
	renWin->SetSize (800, 800);
	theaterRenderer->ResetCamera ( );
	theaterRenderer->ResetCameraClippingRange ( );
	vtkCamera*	camera	= theaterRenderer->GetActiveCamera ( );
	trihedronCommand->SynchronizeViews (camera);
	viewCubeRenderer->ResetCamera ( );
	viewCubeRenderer->ResetCameraClippingRange ( );
	renWin->Render ( );
	iren->Start ( );

	return EXIT_SUCCESS;

}

