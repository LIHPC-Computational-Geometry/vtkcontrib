#include "VtkContrib/vtkConstrainedPointWidget.h"
#include "VtkContrib/vtkLandmarkActor.h"

#include "vtkSmartPointer.h"

#include <vtkActor.h>
#include <vtkCellArray.h>
#include <vtkCellPicker.h>
#include <vtkCommand.h>
#include <vtkDataSetMapper.h>
#include <vtkIdTypeArray.h>
#include <vtkMapper.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>


#include <assert.h>
#include <values.h>
#include <iostream>
using namespace std;


class vtkConstrainedPointWidgetCallback : public vtkCommand
{
	public :

	static vtkConstrainedPointWidgetCallback* New ( )
	{ return new vtkConstrainedPointWidgetCallback ( ); }

	virtual void SetWidget (vtkConstrainedPointWidget* w)
	{ Widget	= w; }

	virtual void Execute (vtkObject* caller, unsigned long event, void* data);


	private :

	vtkConstrainedPointWidgetCallback ( )
		: vtkCommand ( ), Widget (0)
	{ }
	vtkConstrainedPointWidgetCallback (const vtkConstrainedPointWidgetCallback&)
		: vtkCommand ( ), Widget (0)
	{ }
	vtkConstrainedPointWidgetCallback& operator = (
									const vtkConstrainedPointWidgetCallback&)
	{ return *this; }
	virtual ~vtkConstrainedPointWidgetCallback ( )
	{ }

	vtkConstrainedPointWidget*	Widget;
};	// class vtkConstrainedPointWidgetCallback


void vtkConstrainedPointWidgetCallback::Execute (
							vtkObject* caller, unsigned long event, void* data)
{
	assert (0 != Widget);
	vtkConstrainedPointHandleRepresentation*	rep	=
								Widget->GetConstrainedPointRepresentation ( );
	assert (0 != rep);
	double	pos [3]	= { 0., 0., 0. };
	rep->GetWorldPosition (pos);
	cout << "vtkConstrainedPointWidgetCallback::Execute. New pos ("
	     << pos [0] << ", " << pos [1] << ", " << pos [2] << ")" << endl;
}	// vtkConstrainedPointWidgetCallback::Execute


int main ( int argc, char *argv[] )
{
  // Create the RenderWindow, Renderer and both Actors
  //
  vtkSmartPointer<vtkRenderer>		ren1	=
										vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow>	renWin =
										vtkSmartPointer<vtkRenderWindow>::New();
  renWin->AddRenderer(ren1);

  vtkSmartPointer<vtkRenderWindowInteractor>	iren =
							vtkSmartPointer<vtkRenderWindowInteractor>::New();
  iren->SetRenderWindow(renWin);

	const size_t	discretization	= 360;	// Nb points / cercle
	const size_t	loopNum			= 10;	// Nb cercles
	const size_t	pointsNum		= loopNum * discretization;
	const double	loopLength		= 1.;

	double	bounds [6]	= { 0., 2., -1., 1., -1., 1. };
	vtkSmartPointer<vtkConstrainedPointWidget> handleWidget =
							vtkSmartPointer<vtkConstrainedPointWidget>::New();
	handleWidget->GetRepresentation ( );
	handleWidget->SetInteractor(iren);
	handleWidget->GetRepresentation ( )->SetPlaceFactor (2.5);
	handleWidget->GetRepresentation ( )->PlaceWidget (bounds);
	handleWidget->GetRepresentation ( )->SetHandleSize (30);
	vtkSmartPointer<vtkConstrainedPointWidgetCallback>	posCallback	=
					vtkSmartPointer<vtkConstrainedPointWidgetCallback>::New ( );
	posCallback->SetWidget (handleWidget);
	handleWidget->AddObserver (vtkCommand::InteractionEvent, posCallback);

  // Add the actors to the renderer, set the background and size
  //

	vtkUnstructuredGrid*	grid	= vtkUnstructuredGrid::New ( );
	vtkDataSetMapper*		mapper	= vtkDataSetMapper::New ( );
#ifndef VTK_5
	mapper->SetInputData (grid);
#else	// VTK_5
	mapper->SetInput (grid);
#endif	// VTK_5
	mapper->ScalarVisibilityOff ( );
#if defined(VTK5) || defined(VTK_7)	
	mapper->SetImmediateModeRendering (false);
#endif	
	vtkActor*				actor	= vtkActor::New ( );
	actor->GetProperty ( )->SetColor (1., 0., 0.);
	actor->GetProperty ( )->ShadingOff ( );
	actor->GetProperty ( )->SetInterpolationToGouraud ( );
	actor->GetProperty ( )->SetRepresentationToPoints ( );
	actor->GetProperty ( )->SetLineWidth (5.);
	actor->GetProperty ( )->SetPointSize (10.);
	actor->SetMapper (mapper);
	vtkPoints*	points	= vtkPoints::New ( );
	grid->Initialize ( );
	points->SetDataTypeToDouble ( );
	points->SetNumberOfPoints (pointsNum);
	grid->Allocate (pointsNum - 1, pointsNum);
	grid->SetPoints (points);
	for (size_t i = 0; i < pointsNum; i++)
	{
		const double	x	= cos (i * 2 * M_PI / (double)discretization);
		const double	y	= sin (i * 2 * M_PI / (double)discretization);
		const double	z	= (i / discretization) * loopLength + (i % discretization) * loopLength / discretization;
		 points->SetPoint ((vtkIdType)i, x, y, z);
	}	// for (size_t i = 0; i < pointsNum; i++)
	vtkCellArray*	cellArray	= vtkCellArray::New ( );
	vtkIdTypeArray*	idsArray	= vtkIdTypeArray::New ( );
	idsArray->SetNumberOfValues (3 * (pointsNum - 1));
	vtkIdType*		cellsPtr	= idsArray->GetPointer (0);
	for (size_t i = 0; i < pointsNum - 1; i++)
	{
		size_t	pos			= 3 * i;
		cellsPtr [pos++]	= 2;
		cellsPtr [pos++]	= i;
		cellsPtr [pos]		= i + 1;
	}	// for (size_t i = 0; i < pointsNum - 1; i++)
	idsArray->Squeeze ( );
	cellArray->SetCells (pointsNum - 1, idsArray);
	cellArray->Squeeze ( );
	grid->SetCells (VTK_LINE, cellArray);
	ren1->AddActor (actor);

//	vtkPolyDataPointPlacer*	pointPlacer	= vtkPolyDataPointPlacer::New ( );
	handleWidget->GetConstrainedPointRepresentation(
									)->GetUGridPointPlacer ( )->SetGrid (grid);
	handleWidget->GetConstrainedPointRepresentation( )->GetUGridPointPlacer ( )->AddProp (actor);

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
  handleWidget->EnabledOn();
  ren1->SetBackground(0.1, 0.2, 0.4);
  renWin->SetSize(300, 300);
  ren1->ResetCamera();
  ren1->ResetCameraClippingRange();
  renWin->Render();

  iren->Start();

  return EXIT_SUCCESS;

}

