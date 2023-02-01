#include <VtkContrib/vtk2dAxiDihedron.h>
#include <VtkContrib/vtkTrihedron.h>
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
	vtkSmartPointer<vtkRenderer>		theaterRenderer	=
									vtkSmartPointer<vtkRenderer>::New ( );
	vtkSmartPointer<vtkRenderer>		trihedronRenderer	=
									vtkSmartPointer<vtkRenderer>::New ( );
	trihedronRenderer->InteractiveOff ( );
	trihedronRenderer->SetViewport (0, 0, 0.2, 0.2);
	trihedronRenderer->SetLayer (1);
	assert (0 != trihedronRenderer->GetActiveCamera ( ));
	trihedronRenderer->GetActiveCamera ( )->ParallelProjectionOn ( );
	vtkTrihedronCommand*	trihedronCommand	= vtkTrihedronCommand::New ( );
	trihedronCommand->SetRenderer (trihedronRenderer);
	unsigned long	trihedronCommandTag	=
		theaterRenderer->AddObserver (vtkCommand::StartEvent, trihedronCommand);
	vtkSmartPointer<vtkRenderer>		dihedronRenderer	=
									vtkSmartPointer<vtkRenderer>::New ( );
	dihedronRenderer->InteractiveOff ( );
	dihedronRenderer->SetViewport (0.8, 0, 1.0, 0.2);
	dihedronRenderer->SetLayer (2);
	assert (0 != dihedronRenderer->GetActiveCamera ( ));
	dihedronRenderer->GetActiveCamera ( )->ParallelProjectionOn ( );
	vtkTrihedronCommand*	dihedronCommand	= vtkTrihedronCommand::New ( );
	dihedronCommand->SetRenderer (dihedronRenderer);
	unsigned long	dihedronCommandTag	=
		theaterRenderer->AddObserver (vtkCommand::StartEvent, dihedronCommand);
	vtkSmartPointer<vtkRenderWindow>	renWin	 =
									vtkSmartPointer<vtkRenderWindow>::New ( );
	renWin->SetNumberOfLayers (3);
	renWin->AddRenderer (theaterRenderer);
	renWin->AddRenderer (trihedronRenderer);
	renWin->AddRenderer (dihedronRenderer);

	vtkSmartPointer<vtkRenderWindowInteractor>	iren =
							vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);

	// Quelque chose à afficher dans la scène :
	double	center [3]	= { 1.5, -2., 1. };
	double	radius		= 0.5, height	= 2.;
	vtkCylinderSource*	cylinder	= vtkCylinderSource::New ( );
	cylinder->SetResolution (36);
cout << "Cylinder center at (" << center [0] << ", " << center [1] << ", " << center [2] << "). Radius = " << radius << " Height = " << height << endl;
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
	vtkTrihedron*	trihedron	= vtkTrihedron::New ( );
	trihedron->PickableOff ( );
	trihedronRenderer->AddActor (trihedron);
	trihedron->SetAbscissaColor (1., 0.67, 0.);	// beige
	trihedron->SetOrdinateColor (0., 0.82, 0.);	// vert
	trihedron->SetElevationColor (0., 0., 1.);	// bleu
	trihedronCommand->SetTrihedron (trihedron);
	vtk2dAxiDihedron*	dihedron	= vtk2dAxiDihedron::New ( );
	dihedron->PickableOff ( );
	dihedronRenderer->AddActor (dihedron);
//	dihedron->SetAbscissaColor (1., 0.67, 0.);	// beige
//	dihedron->SetOrdinateColor (0., 0.82, 0.);	// vert
//	dihedron->SetElevationColor (0., 0., 1.);	// bleu
//	dihedron->SetElevationColor (0., 0., 0.);	// OK, works fine
//	dihedron->SetAxisThicknessScale (4.);		// OK, works

	dihedronCommand->SetTrihedron (dihedron);

	// Affichage et main loop :

	// No start of graphical application during GitHub CI (just link test)
	// iren->Initialize ( );
	// renWin->Render ( );

	theaterRenderer->SetBackground (1., 1., 1.);
	renWin->SetSize (800, 800);
	theaterRenderer->ResetCamera ( );
	theaterRenderer->ResetCameraClippingRange ( );
	vtkCamera*	camera	= theaterRenderer->GetActiveCamera ( );
	trihedronCommand->SynchronizeViews (camera);
	trihedronRenderer->ResetCamera ( );
	trihedronRenderer->ResetCameraClippingRange ( );
	dihedronCommand->SynchronizeViews (camera);
	dihedronRenderer->ResetCamera ( );
	dihedronRenderer->ResetCameraClippingRange ( );

	// No start of graphical application during GitHub CI (just link test)
	// renWin->Render ( );

    // No start of graphical application during GitHub CI (just link test)
	// iren->Start ( );

	return EXIT_SUCCESS;
}

