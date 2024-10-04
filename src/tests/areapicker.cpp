#include "VtkContrib/vtkConstrainedPointWidget.h"

#include <vtkActor.h>
#include <vtkAreaPicker.h>
#include <vtkCallbackCommand.h>
#include <vtkCellArray.h>
#include <vtkInteractorStyleRubberBandPick.h>
#include <vtkInteractorStyleTrackball.h>
#include <vtkNamedColors.h>
#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProp3DCollection.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderedAreaPicker.h>

#ifdef VTK_9
#include "VtkContrib/vtkLandmarkActor.h"
#endif	// VTK_9


using namespace std;


static void PickCallbackFunction (vtkObject* caller, long unsigned int vtkNotUsed(eventId), void* vtkNotUsed(clientData), void* vtkNotUsed(callData))
{
	cout << "Pick." << endl;
	vtkAreaPicker* areaPicker = static_cast<vtkAreaPicker*>(caller);
	vtkProp3DCollection* props = areaPicker->GetProp3Ds();
	props->InitTraversal();

	for (vtkIdType i = 0; i < props->GetNumberOfItems(); i++)
	{
		vtkProp3D const* prop = props->GetNextProp3D();
		cout << "Picked prop: " << prop << endl;
	}
}	// PickCallbackFunction



int main (int argc, char* argv [], char* envp [])
{
	vtkSmartPointer<vtkRenderer>		renderer		= vtkSmartPointer<vtkRenderer>::New ( );
	vtkSmartPointer<vtkRenderWindow>	renderWindow	= vtkSmartPointer<vtkRenderWindow>::New ( );
	renderWindow->AddRenderer (renderer);
	renderWindow->SetWindowName ("AreaPicking");
#ifdef VTK_9
vtkLandmarkActor*	landmarkActor	= vtkLandmarkActor::New ( );	// WHY ??? Empêche la main loop de tomber ...
#endif	// VTK_9

	vtkSmartPointer<vtkAreaPicker>		areaPicker		= vtkSmartPointer<vtkAreaPicker>::New ( );
	vtkSmartPointer<vtkRenderWindowInteractor>	renderWindowInteractor	= vtkSmartPointer<vtkRenderWindowInteractor>::New ( );
	renderWindowInteractor->SetRenderWindow (renderWindow);
	renderWindowInteractor->SetPicker (areaPicker);

	// Create sets of set of points :
	const int	number	= 5;
	for (int i = 1;i <= number; i++)
	{
		vtkSmartPointer<vtkPoints>		points		= vtkSmartPointer<vtkPoints>::New ( );
		vtkSmartPointer<vtkCellArray>	vertices	= vtkSmartPointer<vtkCellArray>::New ( );
		vtkIdType						pid [1];
		pid [0]	= points->InsertNextPoint (1.0, i, 0.0);
		vertices->InsertNextCell (1, pid);
		pid [0]	= points->InsertNextPoint (0.0, i, 0.0);
		vertices->InsertNextCell (1, pid);
		pid [0]	= points->InsertNextPoint (0.0, i, 1.0);
		vertices->InsertNextCell (1, pid);

		// Create a polydata
		vtkSmartPointer<vtkPolyData>	polydata	= vtkSmartPointer<vtkPolyData>::New ( );
		polydata->SetPoints(points);
		polydata->SetVerts(vertices);

		// Visualize
		vtkSmartPointer<vtkPolyDataMapper>	mapper	= vtkSmartPointer<vtkPolyDataMapper>::New ( );
		mapper->SetInputData(polydata);
		vtkSmartPointer<vtkActor>			actor	= vtkSmartPointer<vtkActor>::New ( );
		actor->SetMapper (mapper);
		actor->GetProperty ( )->SetPointSize (8);
		actor->GetProperty ( )->SetColor (0.5, 0.5, 0.5);

		renderer->AddActor (actor);
	}	// for (int i = 1;i <= number; i++)

	renderWindowInteractor->Initialize ( );
	renderWindow->Render ( );
	renderWindow->SetSize (1800, 1200);
	renderer->ResetCamera();
	renderer->ResetCameraClippingRange();
	renderWindow->Render();

	// For vtkInteractorStyleRubberBandPick - use 'r' and left-mouse to draw a selection box used to pick.
	vtkSmartPointer<vtkInteractorStyleRubberBandPick>	style	= vtkSmartPointer<vtkInteractorStyleRubberBandPick>::New ( );
	style->SetCurrentRenderer (renderer);
	renderWindowInteractor->SetInteractorStyle (style);

	vtkSmartPointer<vtkCallbackCommand>	pickCallback	= vtkSmartPointer<vtkCallbackCommand>::New ( );
	pickCallback->SetCallback (PickCallbackFunction);

	areaPicker->AddObserver (vtkCommand::EndPickEvent, pickCallback);
	cout << "Positionnez la vue à la souris, puis pressez la touche \'r\' pour sélectionner des acteurs au rectangle élastique." << endl;
#ifdef VTK_9
cout << __FILE__ << ' ' << __LINE__ << endl;
#endif	// VTK_9
	renderWindowInteractor->Start ( );
#ifdef VTK_9
cout << __FILE__ << ' ' << __LINE__ << endl;
#endif	// VTK_9

	return EXIT_SUCCESS;
}	// main

