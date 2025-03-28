#include "VtkContrib/vtkViewCubeActor.h"
#include "VtkContrib/vtkFloatingPointType.h"
#include "VtkContrib/vtkTrihedron.h"

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
			{
				ViewCube->PickCallback (x, y);
			}
            // Add your custom handling code here
	}	// if ((0 != interactor) && (0 != ViewCube))
}	// vtkViewCubeActorPickCallback::Execute


// ====================================== LA CLASSE vtkViewCubeActor ====================================== 

vtkViewCubeActor::vtkViewCubeActor ( )
	: vtkPropAssembly ( ), Renderer (0), DrivenRenderer (0), CubePolyData ( ), CubeActor ( ), CubePolyDataMapper ( ), CellPicker ( ),
	  XPlusVectorText ( ), XMinusVectorText ( ), YPlusVectorText ( ), YMinusVectorText ( ), ZPlusVectorText ( ), ZMinusVectorText ( ), 
	  XPlusActor ( ), XMinusActor ( ), YPlusActor ( ), YMinusActor ( ), ZPlusActor ( ), ZMinusActor ( ),
	  Transform (0)
{
	CubePolyData		= vtkSmartPointer<vtkPolyData>::New ( );
	CubeActor			= vtkSmartPointer<vtkActor>::New ( );
	CubePolyDataMapper	= vtkSmartPointer<vtkPolyDataMapper>::New ( );
	assert (0 != CubePolyData.Get ( ));
	assert (0 != CubeActor.Get ( ));
	assert (0 != CubePolyDataMapper.Get ( ));
	CubePolyData->Initialize ( );
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

	vtkCellArray*	cellArray	= vtkCellArray::New ( );
	vtkIdTypeArray*	idsArray	= vtkIdTypeArray::New ( );
	assert (0 != cellArray);
	assert (0 != idsArray);
	cellArray->Initialize ( );
	idsArray->Initialize ( );
	idsArray->SetNumberOfValues (5 * 6 + 5 * 12 + 4 * 8);	// Faces + jonctions quads + triangles
	vtkIdType*	cellsPtr	= idsArray->GetPointer (0);
	size_t		pos			= 0;
	cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 0;	cellsPtr [pos++]	= 1; cellsPtr [pos++]	= 2; cellsPtr [pos++]	= 3;	// Bas
	cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 5; cellsPtr [pos++]	= 6; cellsPtr [pos++]	= 7;	// Droite
	cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 8;	cellsPtr [pos++]	= 9; cellsPtr [pos++]	= 10; cellsPtr [pos++]	= 11;	// Haut
	cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 15;	cellsPtr [pos++]	= 14; cellsPtr [pos++]	= 13; cellsPtr [pos++]	= 12;	// Gauche
	cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 19;	cellsPtr [pos++]	= 18; cellsPtr [pos++]	= 17; cellsPtr [pos++]	= 16;	// Arrière
	cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 20;	cellsPtr [pos++]	= 21; cellsPtr [pos++]	= 22; cellsPtr [pos++]	= 23;	// Avant
//	cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 0;	cellsPtr [pos++]	= 1; cellsPtr [pos++]	= 17; cellsPtr [pos++]	= 16;	// Bas-Ar
cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 16;	cellsPtr [pos++]	= 17; cellsPtr [pos++]	= 1; cellsPtr [pos++]	= 0;	// Bas-Ar
	cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 1;	cellsPtr [pos++]	= 4; cellsPtr [pos++]	= 7; cellsPtr [pos++]	= 2;	// Bas-D
	cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 3;	cellsPtr [pos++]	= 2; cellsPtr [pos++]	= 21; cellsPtr [pos++]	= 20;	// Bas-Av
	cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 12;	cellsPtr [pos++]	= 0; cellsPtr [pos++]	= 3; cellsPtr [pos++]	= 15;	// Bas-G
//	cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 8;	cellsPtr [pos++]	= 9; cellsPtr [pos++]	= 19; cellsPtr [pos++]	= 18;	// Haut-Ar
cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 18;	cellsPtr [pos++]	= 19; cellsPtr [pos++]	= 9; cellsPtr [pos++]	= 8;	// Haut-Ar
//	cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 6;	cellsPtr [pos++]	= 11; cellsPtr [pos++]	= 8; cellsPtr [pos++]	= 5;	// Haut-D
cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 5;	cellsPtr [pos++]	= 8; cellsPtr [pos++]	= 11; cellsPtr [pos++]	= 6;	// Haut-D
	cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 11;	cellsPtr [pos++]	= 10; cellsPtr [pos++]	= 23; cellsPtr [pos++]	= 22;	// Haut-Av
//	cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 10;	cellsPtr [pos++]	= 14; cellsPtr [pos++]	= 13; cellsPtr [pos++]	= 9;	// Haut-G
cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 9;	cellsPtr [pos++]	= 13; cellsPtr [pos++]	= 14; cellsPtr [pos++]	= 10;	// Haut-G
//	cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 17;	cellsPtr [pos++]	= 4; cellsPtr [pos++]	= 5; cellsPtr [pos++]	= 18;	// Ar-D
cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 18;	cellsPtr [pos++]	= 5; cellsPtr [pos++]	= 4; cellsPtr [pos++]	= 17;	// Ar-D
	cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 7;	cellsPtr [pos++]	= 6; cellsPtr [pos++]	= 22; cellsPtr [pos++]	= 21;	// Av-D
//	cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 19;	cellsPtr [pos++]	= 13; cellsPtr [pos++]	= 12; cellsPtr [pos++]	= 16;	// Ar-G
cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 16;	cellsPtr [pos++]	= 12; cellsPtr [pos++]	= 13; cellsPtr [pos++]	= 19;	// Ar-G
	cellsPtr [pos++]	= 4;	cellsPtr [pos++]	= 14;	cellsPtr [pos++]	= 15; cellsPtr [pos++]	= 20; cellsPtr [pos++]	= 23;	// Av-G
	cellsPtr [pos++]	= 3;	cellsPtr [pos++]	= 10;	cellsPtr [pos++]	= 14; cellsPtr [pos++]	= 23;							// Av-H-G
	cellsPtr [pos++]	= 3;	cellsPtr [pos++]	= 22;	cellsPtr [pos++]	= 6; cellsPtr [pos++]	= 11;							// Av-H-D
	cellsPtr [pos++]	= 3;	cellsPtr [pos++]	= 3;	cellsPtr [pos++]	= 20; cellsPtr [pos++]	= 15;							// Av-B-G
	cellsPtr [pos++]	= 3;	cellsPtr [pos++]	= 2;	cellsPtr [pos++]	= 7; cellsPtr [pos++]	= 21;							// Av-B-D
//	cellsPtr [pos++]	= 3;	cellsPtr [pos++]	= 9;	cellsPtr [pos++]	= 13; cellsPtr [pos++]	= 19;							// Ar-H-G
cellsPtr [pos++]	= 3;	cellsPtr [pos++]	= 19;	cellsPtr [pos++]	= 13; cellsPtr [pos++]	= 9;							// Ar-H-G
//	cellsPtr [pos++]	= 3;	cellsPtr [pos++]	= 18;	cellsPtr [pos++]	= 5; cellsPtr [pos++]	= 8;							// Ar-H-D
cellsPtr [pos++]	= 3;	cellsPtr [pos++]	= 8;	cellsPtr [pos++]	= 5; cellsPtr [pos++]	= 18;							// Ar-H-D
//	cellsPtr [pos++]	= 3;	cellsPtr [pos++]	= 0;	cellsPtr [pos++]	= 16; cellsPtr [pos++]	= 12;							// Ar-B-G
cellsPtr [pos++]	= 3;	cellsPtr [pos++]	= 12;	cellsPtr [pos++]	= 16; cellsPtr [pos++]	= 0;							// Ar-B-G
//	cellsPtr [pos++]	= 3;	cellsPtr [pos++]	= 1;	cellsPtr [pos++]	= 4; cellsPtr [pos++]	= 17;							// Ar-B-D
cellsPtr [pos++]	= 3;	cellsPtr [pos++]	= 17;	cellsPtr [pos++]	= 4; cellsPtr [pos++]	= 7;							// Ar-B-D
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
	XPlusActor->AddPosition (0.7, 0., textWidthScaled);
	XMinusActor->AddPosition (-0.7, 0., -textWidthScaled);
	YPlusActor->AddPosition (-textWidthScaled, 0.7, 0.);
	YMinusActor->AddPosition (-textWidthScaled, -0.7, 0.);
	ZPlusActor->AddPosition (-textWidthScaled, 0., 0.7);
	ZMinusActor->SetPosition (textWidthScaled, 0., -0.7);
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
/*	xPlusMapper->Delete ( );	xPlusMapper		= 0;
	xMinusMapper->Delete ( );	xMinusMapper	= 0;
	yPlusMapper->Delete ( );	yPlusMapper		= 0;
	yMinusMapper->Delete ( );	yMinusMapper	= 0;
	zPlusMapper->Delete ( );	zPlusMapper		= 0;
	zMinusMapper->Delete ( );	zMinusMapper	= 0;*/

	// Le picking sur les faces du cube :
	CellPicker	= vtkSmartPointer<vtkCellPicker>::New ( );
	CellPicker->SetTolerance (0.005);
}	// vtkViewCubeActor::vtkViewCubeActor


vtkViewCubeActor::vtkViewCubeActor (const vtkViewCubeActor&)
	: vtkPropAssembly ( ), Renderer (0), DrivenRenderer (0), CubePolyData ( ), CubeActor ( ), CubePolyDataMapper ( ), CellPicker ( ),
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
/*	if (0 != CellPicker)
		CellPicker->Delete ( );
	CellPicker	= 0;
	if (0 != CubeActor)
	{
		RemovePart (CubeActor);
		CubeActor->Delete ( );
	}
	CubeActor	= 0;
	if (0 != XPlusActor)
	{
		RemovePart (XPlusActor);
		XPlusActor->Delete ( );
	}
	XPlusActor	= 0;
	if (0 != XMinusActor)
	{
		RemovePart (XMinusActor);
		XMinusActor->Delete ( );
	}
	XMinusActor	= 0;
	if (0 != YPlusActor)
	{
		RemovePart (YPlusActor);
		YPlusActor->Delete ( );
	}
	YPlusActor	= 0;
	if (0 != YMinusActor)
	{
		RemovePart (YMinusActor);
		YMinusActor->Delete ( );
	}
	YMinusActor	= 0;
	if (0 != ZPlusActor)
	{
		RemovePart (ZPlusActor);
		ZPlusActor->Delete ( );
	}
	ZPlusActor	= 0;
	if (0 != ZMinusActor)
	{
		RemovePart (ZMinusActor);
		ZMinusActor->Delete ( );
	}
	ZMinusActor	= 0;
	if (0 != XPlusVectorText)
		XPlusVectorText->Delete ( );
	XPlusVectorText	= 0;
	if (0 != XMinusVectorText)
		XMinusVectorText->Delete ( );
	XMinusVectorText	= 0;
	if (0 != YPlusVectorText)
		YPlusVectorText->Delete ( );
	YPlusVectorText	= 0;
	if (0 != YMinusVectorText)
		YMinusVectorText->Delete ( );
	YMinusVectorText	= 0;
	if (0 != ZPlusVectorText)
		ZPlusVectorText->Delete ( );
	ZPlusVectorText	= 0;
	if (0 != ZMinusVectorText)
		ZMinusVectorText->Delete ( );
	ZMinusVectorText	= 0;
	if (0 != CubePolyDataMapper)
		CubePolyDataMapper->Delete ( );
	CubePolyDataMapper	= 0;
	if (0 != CubePolyData)
		CubePolyData->Delete ( );
	CubePolyData	= 0;	*/
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
		if (0 != actor)
		{
			actor->SetUserTransform (Transform);
		}	// if (0 != actor)
	}	// while (0 != (property = collection->GetNextProp ( )))
/*	if (0 != CubeActor)
		CubeActor->SetUserTransform (Transform);
	if (0 != XPlusActor)
		XPlusActor->SetUserTransform (Transform);
	if (0 != XMinusActor)
		XMinusActor->SetUserTransform (Transform);
	if (0 != YPlusActor)
		YPlusActor->SetUserTransform (Transform);
	if (0 != YMinusActor)
		YMinusActor->SetUserTransform (Transform);
	if (0 != ZPlusActor)
		ZPlusActor->SetUserTransform (Transform);
	if (0 != ZMinusActor)
		ZMinusActor->SetUserTransform (Transform);*/
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
			camera->ComputeViewPlaneNormal ( );
			DrivenRenderer->ResetCameraClippingRange ( );
			if (0 != DrivenRenderer->GetRenderWindow ( ))
				DrivenRenderer->GetRenderWindow ( )->Render ( );
		}	// if (0 != camera)
	}	// if (0 != CellPicker->Pick (x, y, 0, Renderer))
}	// vtkViewCubeActor::PickCallback

