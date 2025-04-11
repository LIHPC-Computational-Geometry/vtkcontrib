#include "VtkContrib/vtkViewCubeActor.h"
#include "VtkContrib/vtkFloatingPointType.h"
#include "VtkContrib/vtkTrihedron.h"

#include <vtkCellData.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyData.h>
#include <vtkProperty.h>
#include <vtkObjectFactory.h>
#include <vtkProp3DCollection.h>
#include <vtkTextMapper.h>
#include <vtkTextProperty.h>
#include <vtkCommand.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorObserver.h>
#include <vtkInteractorStyle.h>
#include <vtkIndent.h>
#include <vtkCamera.h>
#include <vtkQuad.h>

#include <assert.h>


// ====================================== LA CLASSE vtkViewCubeActorPickCallback ====================================== 

class vtkViewCubeActorPickCallback : public vtkCommand
{
	public :

	static vtkViewCubeActorPickCallback* New ( )
	{
		return new vtkViewCubeActorPickCallback ( );
	}

	virtual void Execute (vtkObject* caller, unsigned long eventId, void* callData);
	virtual void SetViewCube (vtkViewCubeActor* viewCube)
	{ ViewCube	= viewCube; }
	
	
	protected :
	
	vtkViewCubeActorPickCallback ( );
	
	virtual ~vtkViewCubeActorPickCallback ( );
	
	
	private :
	
	vtkViewCubeActorPickCallback (const vtkViewCubeActorPickCallback&);
	vtkViewCubeActorPickCallback& operator = (const vtkViewCubeActorPickCallback&);
	vtkViewCubeActor*		ViewCube;
};	// class vtkViewCubeActorPickCallback


vtkViewCubeActorPickCallback::vtkViewCubeActorPickCallback ( )
	: vtkCommand ( ), ViewCube (0)
{
	PassiveObserverOn ( );	// Indispensable pour que Execute soit appelé.
}	// vtkViewCubeActorPickCallback::vtkViewCubeActorPickCallback


vtkViewCubeActorPickCallback::vtkViewCubeActorPickCallback (const vtkViewCubeActorPickCallback&)
	: vtkCommand ( ), ViewCube (0)
{
	assert (0 && "vtkViewCubeActorPickCallback copy constructor is not allowed.");
}	// vtkViewCubeActorPickCallback::vtkViewCubeActorPickCallback


vtkViewCubeActorPickCallback& vtkViewCubeActorPickCallback::operator = (const vtkViewCubeActorPickCallback&)
{
	assert (0 && "vtkViewCubeActorPickCallback assignment operator is not allowed.");
	return *this;
}	// vtkViewCubeActorPickCallback::operator =


vtkViewCubeActorPickCallback::~vtkViewCubeActorPickCallback ( )
{
}	// vtkViewCubeActorPickCallback::~vtkViewCubeActorPickCallback


void vtkViewCubeActorPickCallback::Execute (vtkObject* caller, unsigned long eventId, void* callData)
{
	vtkRenderWindowInteractor*	interactor	= reinterpret_cast<vtkRenderWindowInteractor*>(caller);

	if ((0 != interactor) && (0 != ViewCube))
	{
			int	x	= interactor->GetEventPosition ( )[0];
			int	y	= interactor->GetEventPosition ( )[1];
			double vp [4];
			ViewCube->GetRenderer ( )->GetViewport (vp);
			if (ViewCube->GetRenderer ( )->IsInViewport  (x, y))
				ViewCube->PickCallback (x, y);
	}	// if ((0 != interactor) && (0 != ViewCube))
}	// vtkViewCubeActorPickCallback::Execute


// ====================================== LA CLASSE vtkViewCubeActorHighlightCallback ====================================== 

class vtkViewCubeActorHighlightCallback : public vtkCommand
{
	public :

	static vtkViewCubeActorHighlightCallback* New ( )
	{
		return new vtkViewCubeActorHighlightCallback ( );
	}

	virtual void Execute (vtkObject* caller, unsigned long eventId, void* callData);
	virtual void SetViewCube (vtkViewCubeActor* viewCube)
	{ ViewCube	= viewCube; }
	
	
	protected :
	
	vtkViewCubeActorHighlightCallback ( );
	
	virtual ~vtkViewCubeActorHighlightCallback ( );
	
	
	private :
	
	vtkViewCubeActorHighlightCallback (const vtkViewCubeActorHighlightCallback&);
	vtkViewCubeActorHighlightCallback& operator = (const vtkViewCubeActorHighlightCallback&);
	vtkViewCubeActor*		ViewCube;
};	// class vtkViewCubeActorHighlightCallback


vtkViewCubeActorHighlightCallback::vtkViewCubeActorHighlightCallback ( )
	: vtkCommand ( ), ViewCube (0)
{
	PassiveObserverOn ( );	// Indispensable pour que Execute soit appelé.
}	// vtkViewCubeActorHighlightCallback::vtkViewCubeActorHighlightCallback


vtkViewCubeActorHighlightCallback::vtkViewCubeActorHighlightCallback (const vtkViewCubeActorHighlightCallback&)
	: vtkCommand ( ), ViewCube (0)
{
	assert (0 && "vtkViewCubeActorHighlightCallback copy constructor is not allowed.");
}	// vtkViewCubeActorHighlightCallback::vtkViewCubeActorHighlightCallback


vtkViewCubeActorHighlightCallback& vtkViewCubeActorHighlightCallback::operator = (const vtkViewCubeActorHighlightCallback&)
{
	assert (0 && "vtkViewCubeActorHighlightCallback assignment operator is not allowed.");
	return *this;
}	// vtkViewCubeActorHighlightCallback::operator =


vtkViewCubeActorHighlightCallback::~vtkViewCubeActorHighlightCallback ( )
{
}	// vtkViewCubeActorHighlightCallback::~vtkViewCubeActorHighlightCallback


void vtkViewCubeActorHighlightCallback::Execute (vtkObject* caller, unsigned long eventId, void* callData)
{
	vtkRenderWindowInteractor*	interactor	= reinterpret_cast<vtkRenderWindowInteractor*>(caller);

	if ((0 != interactor) && (0 != ViewCube))
	{
			int	x	= interactor->GetEventPosition ( )[0];
			int	y	= interactor->GetEventPosition ( )[1];
			double vp [4];
			ViewCube->GetRenderer ( )->GetViewport (vp);
			if (ViewCube->GetRenderer ( )->IsInViewport  (x, y))
				ViewCube->HighlightCallback (x, y);
	}	// if ((0 != interactor) && (0 != ViewCube))
}	// vtkViewCubeActorHighlightCallback::Execute


// ====================================== LA CLASSE vtkViewCubeActor ====================================== 

vtkViewCubeActor::vtkViewCubeActor ( )
	: vtkPropAssembly ( ), Renderer (0), DrivenRenderer (0), CubePolyData ( ), HighlightPolyData ( ), CubeActor ( ), HighlightActor ( ), CubePolyDataMapper ( ), HighlightPolyDataMapper ( ),
	  CellPicker ( ), LastPickedFace ((unsigned char)-1), HighlightedFace ((unsigned char)-1),
	  XPlusVectorText ( ), XMinusVectorText ( ), YPlusVectorText ( ), YMinusVectorText ( ), ZPlusVectorText ( ), ZMinusVectorText ( ), 
	  XPlusActor ( ), XMinusActor ( ), YPlusActor ( ), YMinusActor ( ), ZPlusActor ( ), ZMinusActor ( ), ViewUpVectors ( ),
	  Transform (0)
{
	CubePolyData			= vtkSmartPointer<vtkPolyData>::New ( );
	CubeActor				= vtkSmartPointer<vtkActor>::New ( );
	CubePolyDataMapper		= vtkSmartPointer<vtkPolyDataMapper>::New ( );
	assert (0 != CubePolyData.Get ( ));
	assert (0 != CubePolyData->GetCellData ( ));
	assert (0 != CubeActor.Get ( ));
	assert (0 != CubePolyDataMapper.Get ( ));
	CubePolyData->Initialize ( );
	HighlightPolyData		= vtkSmartPointer<vtkPolyData>::New ( );
	HighlightActor			= vtkSmartPointer<vtkActor>::New ( );
	HighlightPolyDataMapper	= vtkSmartPointer<vtkPolyDataMapper>::New ( );
	assert (0 != HighlightPolyData.Get ( ));
	assert (0 != HighlightActor.Get ( ));
	assert (0 != HighlightPolyDataMapper.Get ( ));
	HighlightPolyData->Initialize ( );
	ViewUpVectors		= vtkSmartPointer<vtkDoubleArray>::New ( );
	ViewUpVectors->SetName ("ViewUp");
	ViewUpVectors->SetNumberOfComponents (3);
	vtkSmartPointer<vtkPoints>	points	= vtkSmartPointer<vtkPoints>::New ( );
	assert (0 != points.Get ( ));
	points->SetNumberOfPoints (24);

	// Longueur d'une arête :
	double	length	= 1.;
	// Portion d'arête définissant la distance extrémité - noeud intermédiaire :
	double	ratio	= 0.15;
	// Coordonnées des jonctions :
	double	j1		= ratio * length, j2	= length - j1;
	
	// Face bas :
	points->SetPoint (0, j1, 0., j1);
	points->SetPoint (1, j2, 0., j1);
	points->SetPoint (2, j2, 0., j2);
	points->SetPoint (3, j1, 0., j2);
	// Face droite :
	points->SetPoint (4, length, j1, j1);
	points->SetPoint (5, length, j2, j1);
	points->SetPoint (6, length, j2, j2);
	points->SetPoint (7, length, j1, j2);
	// Face haute :
	points->SetPoint (8, j2, length, j1);
	points->SetPoint (9, j1, length, j1);
	points->SetPoint (10, j1, length, j2);
	points->SetPoint (11, j2, length, j2);
	// Face gauche :
	points->SetPoint (12, 0., j1, j1);
	points->SetPoint (13, 0., j2, j1);
	points->SetPoint (14, 0., j2, j2);
	points->SetPoint (15, 0., j1, j2);
	// Face arrière :
	points->SetPoint (16, j1, j1, 0.);
	points->SetPoint (17, j2, j1, 0.);
	points->SetPoint (18, j2, j2, 0.);
	points->SetPoint (19, j1, j2, 0.);
	// Face avant :
	points->SetPoint (20, j1, j1, length);
	points->SetPoint (21, j2, j1, length);
	points->SetPoint (22, j2, j2, length);
	points->SetPoint (23, j1, j2, length);
	CubePolyData->SetPoints (points);
	HighlightPolyData->SetPoints (points);

	vtkCellArray*	cellArray	= vtkCellArray::New ( );
	vtkIdTypeArray*	idsArray	= vtkIdTypeArray::New ( );
	assert (0 != cellArray);
	assert (0 != idsArray);
	cellArray->Initialize ( );
	idsArray->Initialize ( );
	idsArray->SetNumberOfValues (5 * 6 + 5 * 12 + 4 * 8);	// Faces + jonctions quads + triangles
	CubePolyData->GetCellData ( )->SetVectors (ViewUpVectors);
	vtkIdType*	cellsPtr	= idsArray->GetPointer (0);
	size_t		pos			= 0;
	cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 0;	cellsPtr [pos++]	= 1; cellsPtr [pos++]	= 2; cellsPtr [pos++]	= 3;	// Bas
	ViewUpVectors->InsertNextTuple3 (0., 0., 1.);
	cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 5; cellsPtr [pos++]	= 6; cellsPtr [pos++]	= 7;	// Droite
	ViewUpVectors->InsertNextTuple3 (0., 1., 0.);
	cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 8;	cellsPtr [pos++]	= 9; cellsPtr [pos++]	= 10; cellsPtr [pos++]	= 11;	// Haut
	ViewUpVectors->InsertNextTuple3 (0., 0., 1.);
	cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 15;	cellsPtr [pos++]	= 14; cellsPtr [pos++]	= 13; cellsPtr [pos++]	= 12;	// Gauche
	ViewUpVectors->InsertNextTuple3 (0., 1., 0.);
	cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 19;	cellsPtr [pos++]	= 18; cellsPtr [pos++]	= 17; cellsPtr [pos++]	= 16;	// Arrière
	ViewUpVectors->InsertNextTuple3 (0., 1., 0.);
	cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 20;	cellsPtr [pos++]	= 21; cellsPtr [pos++]	= 22; cellsPtr [pos++]	= 23;	// Avant
	ViewUpVectors->InsertNextTuple3 (0., 1., 0.);
	cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 16;	cellsPtr [pos++]	= 17; cellsPtr [pos++]	= 1; cellsPtr [pos++]	= 0;	// Bas-Ar
	ViewUpVectors->InsertNextTuple3 (0., 1., 0.);
	cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 1;	cellsPtr [pos++]	= 4; cellsPtr [pos++]	= 7; cellsPtr [pos++]	= 2;	// Bas-D
	ViewUpVectors->InsertNextTuple3 (0., 1., 0.);
	cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 3;	cellsPtr [pos++]	= 2; cellsPtr [pos++]	= 21; cellsPtr [pos++]	= 20;	// Bas-Av
	ViewUpVectors->InsertNextTuple3 (0., 1., 0.);
	cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 12;	cellsPtr [pos++]	= 0; cellsPtr [pos++]	= 3; cellsPtr [pos++]	= 15;	// Bas-G
	ViewUpVectors->InsertNextTuple3 (0., 1., 0.);
	cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 18;	cellsPtr [pos++]	= 19; cellsPtr [pos++]	= 9; cellsPtr [pos++]	= 8;	// Haut-Ar
	ViewUpVectors->InsertNextTuple3 (0., 1., 0.);
	cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 5;	cellsPtr [pos++]	= 8; cellsPtr [pos++]	= 11; cellsPtr [pos++]	= 6;	// Haut-D
	ViewUpVectors->InsertNextTuple3 (0., 1., 0.);
	cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 11;	cellsPtr [pos++]	= 10; cellsPtr [pos++]	= 23; cellsPtr [pos++]	= 22;	// Haut-Av
	ViewUpVectors->InsertNextTuple3 (0., 1., 0.);
	cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 9;	cellsPtr [pos++]	= 13; cellsPtr [pos++]	= 14; cellsPtr [pos++]	= 10;	// Haut-G
	ViewUpVectors->InsertNextTuple3 (0., 1., 0.);
	cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 18;	cellsPtr [pos++]	= 5; cellsPtr [pos++]	= 4; cellsPtr [pos++]	= 17;	// Ar-D
	ViewUpVectors->InsertNextTuple3 (0., 1., 0.);
	cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 7;	cellsPtr [pos++]	= 6; cellsPtr [pos++]	= 22; cellsPtr [pos++]	= 21;	// Av-D
	ViewUpVectors->InsertNextTuple3 (0., 1., 0.);
	cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 16;	cellsPtr [pos++]	= 12; cellsPtr [pos++]	= 13; cellsPtr [pos++]	= 19;	// Ar-G
	ViewUpVectors->InsertNextTuple3 (0., 1., 0.);
	cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 14;	cellsPtr [pos++]	= 15; cellsPtr [pos++]	= 20; cellsPtr [pos++]	= 23;	// Av-G
	ViewUpVectors->InsertNextTuple3 (0., 1., 0.);
	cellsPtr [pos++]	= 3;	cellsPtr [pos++]	= 10;	cellsPtr [pos++]	= 14; cellsPtr [pos++]	= 23;							// Av-H-G
	ViewUpVectors->InsertNextTuple3 (0., 1., 0.);
	cellsPtr [pos++]	= 3;	cellsPtr [pos++]	= 22;	cellsPtr [pos++]	= 6; cellsPtr [pos++]	= 11;							// Av-H-D
	ViewUpVectors->InsertNextTuple3 (0., 1., 0.);
	cellsPtr [pos++]	= 3;	cellsPtr [pos++]	= 3;	cellsPtr [pos++]	= 20; cellsPtr [pos++]	= 15;							// Av-B-G
	ViewUpVectors->InsertNextTuple3 (0., 1., 0.);
	cellsPtr [pos++]	= 3;	cellsPtr [pos++]	= 2;	cellsPtr [pos++]	= 7; cellsPtr [pos++]	= 21;							// Av-B-D
	ViewUpVectors->InsertNextTuple3 (0., 1., 0.);
	cellsPtr [pos++]	= 3;	cellsPtr [pos++]	= 19;	cellsPtr [pos++]	= 13; cellsPtr [pos++]	= 9;							// Ar-H-G
	ViewUpVectors->InsertNextTuple3 (0., 1., 0.);
	cellsPtr [pos++]	= 3;	cellsPtr [pos++]	= 8;	cellsPtr [pos++]	= 5; cellsPtr [pos++]	= 18;							// Ar-H-D
	ViewUpVectors->InsertNextTuple3 (0., 1., 0.);
	cellsPtr [pos++]	= 3;	cellsPtr [pos++]	= 12;	cellsPtr [pos++]	= 16; cellsPtr [pos++]	= 0;							// Ar-B-G
	ViewUpVectors->InsertNextTuple3 (0., 1., 0.);
	cellsPtr [pos++]	= 3;	cellsPtr [pos++]	= 17;	cellsPtr [pos++]	= 4; cellsPtr [pos++]	= 7;							// Ar-B-D
	ViewUpVectors->InsertNextTuple3 (0., 1., 0.);
	cellArray->SetCells (6 + 12 + 8, idsArray);
	CubePolyData->SetPolys (cellArray);
	idsArray->Delete ( );   idsArray        = 0;
	cellArray->Delete ( );  cellArray       = 0;
//	points->Delete ( );		points			= 0;
	CubePolyDataMapper->SetInputData (CubePolyData);
	CubePolyDataMapper->ScalarVisibilityOff ( );
	CubeActor->SetMapper (CubePolyDataMapper);
	CubeActor->GetProperty ( )->SetColor (.75, .75, .75);
	CubeActor->PickableOn ( );
	AddPart (CubeActor);
	HighlightPolyDataMapper->SetInputData (HighlightPolyData);
	HighlightPolyDataMapper->ScalarVisibilityOff ( );
	HighlightActor->SetMapper (HighlightPolyDataMapper);
	HighlightActor->GetProperty ( )->SetColor (1., 0., 0.);
	HighlightActor->PickableOff ( );

	XPlusVectorText		= vtkSmartPointer<vtkVectorText>::New ( );
	XMinusVectorText	= vtkSmartPointer<vtkVectorText>::New ( );
	YPlusVectorText		= vtkSmartPointer<vtkVectorText>::New ( );
	YMinusVectorText	= vtkSmartPointer<vtkVectorText>::New ( );
	ZPlusVectorText		= vtkSmartPointer<vtkVectorText>::New ( );
	ZMinusVectorText	= vtkSmartPointer<vtkVectorText>::New ( );
	XPlusVectorText->SetText ("X+");
	XMinusVectorText->SetText ("X-");
	YPlusVectorText->SetText ("Y+");
	YMinusVectorText->SetText ("Y-");
	ZPlusVectorText->SetText ("Z+");
	ZMinusVectorText->SetText ("Z-");
	XPlusActor		= vtkSmartPointer<vtkActor>::New ( );
	XMinusActor		= vtkSmartPointer<vtkActor>::New ( );
	YPlusActor		= vtkSmartPointer<vtkActor>::New ( );
	YMinusActor		= vtkSmartPointer<vtkActor>::New ( );
	ZPlusActor		= vtkSmartPointer<vtkActor>::New ( );
	ZMinusActor		= vtkSmartPointer<vtkActor>::New ( );
	XPlusActor->PickableOff ( );
	XMinusActor->PickableOff ( );
	YPlusActor->PickableOff ( );
	YMinusActor->PickableOff ( );
	ZPlusActor->PickableOff ( );
	ZMinusActor->PickableOff ( );
	XPlusVectorText->Update ( );
	double*	bounds			= XPlusVectorText->GetOutput ( )->GetBounds ( );
	double	textWidth		= bounds [1] - bounds [0];
	double	textScale		= 0.5 * 1. / textWidth;
	double	textWidthScaled	= 0.25 * textScale * textWidth;
	XPlusActor->SetScale (textScale);
	XMinusActor->SetScale (textScale);
	YPlusActor->SetScale (textScale);
	YMinusActor->SetScale (textScale);
	ZPlusActor->SetScale (textScale);
	ZMinusActor->SetScale (textScale);
	XPlusActor->SetOrigin (0.5, 0.5, 0.5);
	XMinusActor->SetOrigin (0.5, 0.5, 0.5);
	YPlusActor->SetOrigin (0.5, 0.5, 0.5);
	YMinusActor->SetOrigin (0.5, 0.5, 0.5);
	ZPlusActor->SetOrigin (0.5, 0.5, 0.5);
	ZMinusActor->SetOrigin (0.5, 0.5, 0.5);
	const double position	= 0.651;	// La bonne position pour que le texte soit collé au plus près de la face associée
	XPlusActor->AddPosition (position, 0., textWidthScaled);
	XMinusActor->AddPosition (-position, 0., -textWidthScaled);
	YPlusActor->AddPosition (-textWidthScaled, position, 0.);
	YMinusActor->AddPosition (-textWidthScaled, -position, 0.);
	ZPlusActor->AddPosition (-textWidthScaled, 0., position);
	ZMinusActor->SetPosition (textWidthScaled, 0., -position);
	XPlusActor->SetOrientation (0, 90, 0);
	XMinusActor->SetOrientation (0, -90, 0);
	YPlusActor->SetOrientation (-90, 0, 0);
	YMinusActor->SetOrientation (90, 0, 0);
	ZPlusActor->SetOrientation (0, 0, 0);
	ZMinusActor->SetOrientation (0, 180, 0);
	XPlusActor->GetProperty ( )->SetColor (vtkTrihedron::xColor);
	XMinusActor->GetProperty ( )->SetColor (vtkTrihedron::xColor);
	YPlusActor->GetProperty ( )->SetColor (vtkTrihedron::yColor);
	YMinusActor->GetProperty ( )->SetColor (vtkTrihedron::yColor);
	ZPlusActor->GetProperty ( )->SetColor (vtkTrihedron::zColor);
	ZMinusActor->GetProperty ( )->SetColor (vtkTrihedron::zColor);
	vtkSmartPointer<vtkPolyDataMapper>	xPlusMapper		= vtkSmartPointer<vtkPolyDataMapper>::New ( );
	vtkSmartPointer<vtkPolyDataMapper>	xMinusMapper	= vtkSmartPointer<vtkPolyDataMapper>::New ( );
	vtkSmartPointer<vtkPolyDataMapper>	yPlusMapper		= vtkSmartPointer<vtkPolyDataMapper>::New ( );
	vtkSmartPointer<vtkPolyDataMapper>	yMinusMapper	= vtkSmartPointer<vtkPolyDataMapper>::New ( );
	vtkSmartPointer<vtkPolyDataMapper>	zPlusMapper		= vtkSmartPointer<vtkPolyDataMapper>::New ( );
	vtkSmartPointer<vtkPolyDataMapper>	zMinusMapper	= vtkSmartPointer<vtkPolyDataMapper>::New ( );
	xPlusMapper->SetInputConnection (XPlusVectorText->GetOutputPort ( ));
	xMinusMapper->SetInputConnection (XMinusVectorText->GetOutputPort ( ));
	yPlusMapper->SetInputConnection (YPlusVectorText->GetOutputPort ( ));
	yMinusMapper->SetInputConnection (YMinusVectorText->GetOutputPort ( ));
	zPlusMapper->SetInputConnection (ZPlusVectorText->GetOutputPort ( ));
	zMinusMapper->SetInputConnection (ZMinusVectorText->GetOutputPort ( ));
	XPlusActor->SetMapper (xPlusMapper);
	XMinusActor->SetMapper (xMinusMapper);
	YPlusActor->SetMapper (yPlusMapper);
	YMinusActor->SetMapper (yMinusMapper);
	ZPlusActor->SetMapper (zPlusMapper);
	ZMinusActor->SetMapper (zMinusMapper);
	AddPart (XPlusActor);
	AddPart (XMinusActor);
	AddPart (YPlusActor);
	AddPart (YMinusActor);
	AddPart (ZPlusActor);
	AddPart (ZMinusActor);

	// Le picking sur les faces du cube :
	CellPicker	= vtkSmartPointer<vtkCellPicker>::New ( );
	CellPicker->SetTolerance (0.005);
}	// vtkViewCubeActor::vtkViewCubeActor


vtkViewCubeActor::vtkViewCubeActor (const vtkViewCubeActor&)
	: vtkPropAssembly ( ), Renderer (0), DrivenRenderer (0), CubePolyData ( ), HighlightPolyData ( ), CubeActor ( ), HighlightActor ( ), CubePolyDataMapper ( ), HighlightPolyDataMapper ( ),
	  CellPicker ( ), LastPickedFace ((unsigned char)-1), HighlightedFace ((unsigned char)-1),
	  XPlusVectorText ( ), XMinusVectorText ( ), YPlusVectorText ( ), YMinusVectorText ( ), ZPlusVectorText ( ), ZMinusVectorText ( ), 
	  XPlusActor ( ), XMinusActor ( ), YPlusActor ( ), YMinusActor ( ), ZPlusActor ( ), ZMinusActor ( ),
	  Transform (0)
{
	assert (0 && "vtkViewCubeActor copy constructor is not allowed.");
}	// vtkViewCubeActor::vtkViewCubeActor (const vtkViewCubeActor&)


vtkViewCubeActor& vtkViewCubeActor::operator = (const vtkViewCubeActor&)
{
	assert (0 && "vtkViewCubeActor operator = is not allowed.");
	return *this;
}	// vtkViewCubeActor::operator =


vtkViewCubeActor::~vtkViewCubeActor ( )
{
	if (0 != DrivenRenderer)
		DrivenRenderer->UnRegister (this);
	DrivenRenderer	= 0;
	if (0 != Transform)
		Transform->Delete ( );
	Transform	= 0;
	if (0 != Renderer)
		Renderer->UnRegister (this);
	Renderer	= 0;
}	// vtkViewCubeActor::~vtkViewCubeActor


vtkViewCubeActor* vtkViewCubeActor::New ( )
{
	vtkObject*	object	= vtkObjectFactory::CreateInstance ("vtkViewCubeActor");
	if (0 != object)
		return (vtkViewCubeActor*)object;

    return new vtkViewCubeActor ( );
}	// vtkViewCubeActor::New


void vtkViewCubeActor::SetRenderers (vtkRenderer* renderer, vtkRenderer* drivenRenderer)
{
	if ((renderer != Renderer) && (0 != Renderer))
		Renderer->Delete ( );
	Renderer	= 0;
	if ((drivenRenderer != DrivenRenderer) && (0 != DrivenRenderer))
		DrivenRenderer->Delete ( );
	DrivenRenderer	= 0;
	
	if (0 != renderer)
	{
		Renderer	= renderer;
		Renderer->Register (this);
		renderer->SetInteractive (1);
		// Par défaut les priorités sont à 0. Si on veut être (à peu près) sûr que cette commande passe en premier alors on passe une priorité > 0 :
		vtkViewCubeActorPickCallback*	viewCubeCommand	= vtkViewCubeActorPickCallback::New ( );
		viewCubeCommand->SetViewCube (this);
		renderer->GetRenderWindow ( )->GetInteractor ( )->AddObserver (vtkCommand::LeftButtonReleaseEvent, viewCubeCommand, 10.);
		vtkViewCubeActorHighlightCallback*	viewCubeHighlightCommand	= vtkViewCubeActorHighlightCallback::New ( );
		viewCubeHighlightCommand->SetViewCube (this);
		renderer->GetRenderWindow ( )->GetInteractor ( )->AddObserver (vtkCommand::MouseMoveEvent, viewCubeHighlightCommand, 10.);
	}	// if (0 != renderer)
	if (0 != drivenRenderer)
	{
		DrivenRenderer	= drivenRenderer;
		DrivenRenderer->Register (this);
	}	// if (0 != drivenRenderer)
}	// vtkViewCubeActor::SetRenderer


vtkTransform* vtkViewCubeActor::GetTransform ( )
{
	return Transform;
}	// vtkViewCubeActor::GetTransform


void vtkViewCubeActor::SetTransform (vtkTransform* transform)
{
	if (transform == Transform)
		return;

	if (0 != Transform)
		Transform->UnRegister (0);
	Transform	= transform;
	if (0 != Transform)
		Transform->Register (0);

	vtkPropCollection*	collection	= this->GetParts ( );
	vtkProp*			property	= 0;
	collection->InitTraversal ( );
	while (0 != (property = collection->GetNextProp ( )))
	{
		vtkActor*	actor	= vtkActor::SafeDownCast (property);
		if ((0 != actor) && (actor != HighlightActor))
		{
			actor->SetUserTransform (Transform);
		}	// if (0 != actor)
	}	// while (0 != (property = collection->GetNextProp ( )))
	if (0 != HighlightActor)
		HighlightActor->SetUserTransform (Transform);
}	// vtkViewCubeActor::SetTransform


void vtkViewCubeActor::SetFacesLabels (const char* front, const char* back, const char* left, const char* right, const char* top, const char* bottom)
{
	if (0 != XPlusVectorText.Get ( ))
		XPlusVectorText->SetText (right);
	if (0 != XMinusVectorText.Get ( ))
		XMinusVectorText->SetText (left);
	if (0 != YPlusVectorText.Get ( ))
		YPlusVectorText->SetText (top);
	if (0 != YMinusVectorText.Get ( ))
		YMinusVectorText->SetText (bottom);
	if (0 != ZPlusVectorText.Get ( ))
		ZPlusVectorText->SetText (front);
	if (0 != ZMinusVectorText.Get ( ))
		ZMinusVectorText->SetText (back);
}	// vtkViewCubeActor::SetFacesLabels


void vtkViewCubeActor::SetLabel (bool on)
{
	if (true == on)
		LabelOn ( );
	else
		LabelOff ( );
}	// vtkViewCubeActor::SetLabel2D


void vtkViewCubeActor::LabelOn ( )
{
	AddPart (XPlusActor);
	AddPart (XMinusActor);
	AddPart (YPlusActor);
	AddPart (YMinusActor);
	AddPart (ZPlusActor);
	AddPart (ZMinusActor);
}	// vtkViewCubeActor::LabelOn


void vtkViewCubeActor::LabelOff ( )
{
	RemovePart (XPlusActor);
	RemovePart (XMinusActor);
	RemovePart (YPlusActor);
	RemovePart (YMinusActor);
	RemovePart (ZPlusActor);
	RemovePart (ZMinusActor);
}	// vtkViewCubeActor::LabelOff


void vtkViewCubeActor::SetFacesOpacity (double opacity)
{
	if ((0 != CubeActor.Get ( )) && (0 != CubeActor->GetProperty ( )))
		CubeActor->GetProperty ( )->SetOpacity (opacity);
}	// vtkViewCubeActor::SetFacesOpacity


void vtkViewCubeActor::PickCallback (int x, int y)
{
	if ((0 == CellPicker.Get ( )) || (0 == Renderer) || (0 == DrivenRenderer))
		return;

	if (0 != CellPicker->Pick (x, y, 0, Renderer))
	{
		double*		normal	= CellPicker->GetPickNormal ( );
		vtkMath::Normalize( normal);
		vtkCamera*	camera	= DrivenRenderer->GetActiveCamera ( );
		if (0 != camera)
		{
			const double*	focalPoint	= camera->GetFocalPoint ( );
			const double	distance	= camera->GetDistance ( );
			const double	xpos		= focalPoint [0] + normal [0] * distance;
			const double	ypos		= focalPoint [1] + normal [1] * distance;
			const double	zpos		= focalPoint [2] + normal [2] * distance;
			camera->SetPosition (xpos, ypos, zpos);
			if (CellPicker->GetCellId ( ) == LastPickedFace)
			{
				assert (0 != CubePolyData);
				assert (0 != CubePolyData->GetCellData ( ));
				assert (0 != CubePolyData->GetCellData ( )->GetVectors ( ));
				assert (CellPicker->GetCellId ( ) < CubePolyData->GetCellData ( )->GetVectors ( )->GetNumberOfTuples ( ));
				double*	viewup	= CubePolyData->GetCellData ( )->GetVectors ( )->GetTuple (CellPicker->GetCellId ( ));
				camera->SetViewUp (viewup);
				camera->SetRoll (0.);
			}	// if (CellPicker->GetCellId ( ) == LastPickedFace)
			camera->ComputeViewPlaneNormal ( );
			DrivenRenderer->ResetCameraClippingRange ( );
			if (0 != DrivenRenderer->GetRenderWindow ( ))
				DrivenRenderer->GetRenderWindow ( )->Render ( );
		}	// if (0 != camera)
		LastPickedFace	= CellPicker->GetCellId ( );
	}	// if (0 != CellPicker->Pick (x, y, 0, Renderer))
}	// vtkViewCubeActor::PickCallback


void vtkViewCubeActor::HighlightCallback (int x, int y)
{
	if ((0 == CellPicker.Get ( )) || (0 == Renderer) || (0 == DrivenRenderer))
		return;
	assert (0 != CubePolyData.Get ( ));
	assert (0 != HighlightPolyData.Get ( ));
	assert (0 != HighlightActor.Get ( ));

	if (0 != CellPicker->Pick (x, y, 0, Renderer))
	{
		if (HighlightedFace == CellPicker->GetCellId ( ))
			return;
			
		if ((unsigned char)-1 != HighlightedFace)
		{
			RemovePart (HighlightActor);
		}	// if ((unsigned char)-1 != HighlightedFace)
		HighlightedFace	= CellPicker->GetCellId ( );
		vtkIdType	npts	= 0;
		vtkIdType*	pts		= 0;
		CubePolyData->GetCellPoints (CellPicker->GetCellId ( ), npts, pts);
		if (0 == HighlightPolyData->GetNumberOfCells  ( ))
		{
			HighlightPolyData->Allocate (1);
			HighlightPolyData->InsertNextCell (VTK_POLYGON, npts, pts);
		}	// if (0 == HighlightPolyData->GetNumberOfCells  ( ))
		else
		{
			HighlightPolyData->ReplaceCell (0, npts, pts);
			HighlightPolyData->Modified ( );
		}	// if (0 == HighlightPolyData->GetNumberOfCells  ( ))

		AddPart (HighlightActor);
	}	// if (0 != CellPicker->Pick (x, y, 0, Renderer))
	else
	{
		if ((unsigned char)-1 != HighlightedFace)
		{
			RemovePart (HighlightActor);
		}	// if ((unsigned char)-1 != HighlightedFace)
		HighlightedFace	= (unsigned char)-1;
	}	// else if (0 != CellPicker->Pick (x, y, 0, Renderer))

	if (0 != DrivenRenderer->GetRenderWindow ( ))
		DrivenRenderer->GetRenderWindow ( )->Render ( );
}	// vtkViewCubeActor::HighlightCallback

