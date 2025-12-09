#include <vtkCubeSource.h>
#include "VtkContrib/vtkEllipsoidWidget.h"
#include "VtkContrib/vtkLandmarkActor.h"

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>


#include <assert.h>
#include <values.h>
#include <iostream>
using namespace std;



int main ( int argc, char *argv[] )
{
  // Create the RenderWindow, Renderer and both Actors
  //
  vtkSmartPointer<vtkRenderer>		ren1	= vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow>	renWin = vtkSmartPointer<vtkRenderWindow>::New();
  renWin->AddRenderer(ren1);

  vtkSmartPointer<vtkRenderWindowInteractor>	iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  iren->SetRenderWindow(renWin);

	double	bounds [6]	= { -1., 2., -1., 1., -1., 1. };
	
	vtkSmartPointer<vtkEllipsoidWidget>	ellipsoidWidget	= vtkSmartPointer<vtkEllipsoidWidget>::New ( );
	ellipsoidWidget->SetPlaceFactor (1.);
	ellipsoidWidget->SetEllipsoidOpacity (0.2);
	ellipsoidWidget->SetEllipsoidColor (1., 0., 0.);
	ellipsoidWidget->SetDiscretisation (50, 50, 50);
	ellipsoidWidget->PlaceWidget (bounds);
	ellipsoidWidget->SetInteractor (iren);
	ellipsoidWidget->On ( );

  vtkLandmarkActor*	landmarkActor	= vtkLandmarkActor::New ( );
  landmarkActor->SetBounds (bounds);
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
  landmarkActor->SetCamera (ren1->GetActiveCamera ( ));
  landmarkActor->SetDrawXGridlines (1);
  landmarkActor->SetDrawYGridlines (1);
  landmarkActor->SetDrawZGridlines (1);
  landmarkActor->SetCornerOffset (0.);
  landmarkActor->PickableOff ( );
  ren1->AddActor (landmarkActor);

  iren->Initialize();
  renWin->Render();
//  handleWidget->EnabledOn();
  ren1->SetBackground(0.1, 0.2, 0.4);
  renWin->SetSize(1800, 1200);
  ren1->ResetCamera();
  ren1->ResetCameraClippingRange();
  renWin->Render();

  iren->Start();

  return EXIT_SUCCESS;

}

