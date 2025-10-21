#include <vtkCubeSource.h>
#include "VtkContrib/vtkFrustumWidget.h"
#include "VtkContrib/vtkLandmarkActor.h"

#include <vtkActor.h>
#include <vtkDoubleArray.h>
#include <vtkMapper.h>
#include <vtkCubeSource.h>
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
	
	vtkSmartPointer<vtkCubeSource>	cubeSource	= vtkSmartPointer<vtkCubeSource>::New ( );
	cubeSource->SetBounds (bounds);
	cubeSource->Update ( );

	vtkPolyDataMapper*	cubeMapper	= vtkPolyDataMapper::New ( );
	cubeMapper->SetInputConnection (cubeSource->GetOutputPort ( ));
	vtkActor*	cubeActor	= vtkActor::New ( );
	cubeActor->SetMapper (cubeMapper);
	cubeActor->GetProperty ( )->SetColor (0, 0, 1);
//	ren1->AddActor (cubeActor);

	vtkSmartPointer<vtkFrustumWidget>	frustumWidget	= vtkSmartPointer<vtkFrustumWidget>::New ( );
	frustumWidget->SetPlaceFactor (1.);
	frustumWidget->PlaceWidget (bounds);
	frustumWidget->SetInteractor (iren);
//	frustumWidget->AddParallelPlanes (vtkFrustumWidget::LEFT, vtkFrustumWidget::RIGHT);
	frustumWidget->AddParallelPlanes (vtkFrustumWidget::LEFT);
//	frustumWidget->RemoveParallelPlanes (vtkFrustumWidget::BACK, vtkFrustumWidget::FRONT);
	frustumWidget->On ( );
	frustumWidget->SetInteractorEnabled (vtkFrustumWidget::TOP, false);

	double	origin [3]	= { -0.5, 0.5, 0.8 };
	double	normal [3]	= { 0., 1., 1. };
	frustumWidget->SetPlane (vtkFrustumWidget::FRONT, origin, normal);
	vtkSmartPointer<vtkPlanes>	planes	= vtkSmartPointer<vtkPlanes>::New ( );
	vtkPoints*		points	= vtkPoints::New ( );
	vtkDoubleArray*	normals	= vtkDoubleArray::New ( );
	normals->SetNumberOfComponents (3);
	points->InsertNextPoint (-0.8, 0., 0.);
	points->InsertNextPoint (0.8, 0., 0.);
	points->InsertNextPoint (0., -0.8, 0.);
	points->InsertNextPoint (0., 0.8, 0.);
	points->InsertNextPoint (0., 0., -0.8);
	points->InsertNextPoint (0., 0., 0.8);
	normals->InsertNextTuple3 (-0.7, 0.3, 0.);
	normals->InsertNextTuple3 (0.7, 0.3, 0.);
	normals->InsertNextTuple3 (0., -1., 0.);
	normals->InsertNextTuple3 (0., 1, 0.);
	normals->InsertNextTuple3 (0., 0.3, -0.7);
	normals->InsertNextTuple3 (0., 0.3, 0.7);
	planes->SetPoints (points);
	planes->SetNormals (normals);
	frustumWidget->SetPlanes (*(planes.Get ( )));

// Test :
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

