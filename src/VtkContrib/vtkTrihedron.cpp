#include "VtkContrib/vtkTrihedron.h"
#include "VtkContrib/vtkFloatingPointType.h"

#include <vtkPolyDataMapper.h>
#include <vtkArrowSource.h>
#include <vtkPolyData.h>
#include <vtkProperty.h>
#include <vtkVectorText.h>
#include <vtkObjectFactory.h>
#include <vtkProp3DCollection.h>
#include <vtkTextMapper.h>
#include <vtkTextProperty.h>
#include <vtkProperty2D.h>


#include <assert.h>


double	vtkTrihedron::xColor [3]	= {1., 0., 0.};
double	vtkTrihedron::yColor [3]	= {0., 1., 0.};
double	vtkTrihedron::zColor [3]	= {0., 0., 1.};


vtkTrihedron::vtkTrihedron ( )
	: vtkPropAssembly ( ), 
	  m_xAxisArrowSource (0), m_yAxisArrowSource (0), m_zAxisArrowSource (0),
	  m_xVectorText (0), m_yVectorText (0), m_zVectorText (0),
	  m_xAxisActor (0), m_yAxisActor (0), m_zAxisActor (0),
	  m_xLabelActor (0), m_yLabelActor (0), m_zLabelActor (0),
	  m_xLabelActor2D (0), m_yLabelActor2D (0), m_zLabelActor2D (0),
	  m_xTextMapper (0), m_yTextMapper (0), m_zTextMapper (0),
	  m_transform (0),
	  m_shaftRadius (0.03), m_tipRadius (0.35), m_tipLength (0.1)
{
	int ArrowResolution = 8;
	int TipResolution   = 12;
	
	m_xAxisArrowSource	= vtkArrowSource::New();
	m_yAxisArrowSource	= vtkArrowSource::New();
	m_zAxisArrowSource	= vtkArrowSource::New();
	m_shaftRadius		= m_xAxisArrowSource->GetShaftRadius ( );
	m_tipRadius			= m_xAxisArrowSource->GetTipRadius ( );
	m_tipLength			= m_xAxisArrowSource->GetTipLength ( );
	
	vtkPolyDataMapper *xAxisMapper  = vtkPolyDataMapper::New();
	vtkPolyDataMapper *yAxisMapper  = vtkPolyDataMapper::New();
	vtkPolyDataMapper *zAxisMapper  = vtkPolyDataMapper::New();
	
	m_xAxisActor	= vtkActor::New();
	m_yAxisActor	= vtkActor::New();
	m_zAxisActor	= vtkActor::New();
	
	m_xVectorText = vtkVectorText::New();
	m_yVectorText = vtkVectorText::New();
	m_zVectorText = vtkVectorText::New();
	
	vtkPolyDataMapper *xLabelMapper = vtkPolyDataMapper::New();
	vtkPolyDataMapper *yLabelMapper = vtkPolyDataMapper::New();
	vtkPolyDataMapper *zLabelMapper = vtkPolyDataMapper::New();
	m_xTextMapper	= vtkTextMapper::New ( );
	m_yTextMapper	= vtkTextMapper::New ( );
	m_zTextMapper	= vtkTextMapper::New ( );
	
	m_xLabelActor	= vtkActor::New();
	m_yLabelActor	= vtkActor::New();
	m_zLabelActor	= vtkActor::New();
	m_xLabelActor2D	= vtkActor2D::New();
	m_yLabelActor2D	= vtkActor2D::New();
	m_zLabelActor2D	= vtkActor2D::New();
	
	// Création de l'axe X
	m_xAxisArrowSource->SetShaftResolution(ArrowResolution);
	m_xAxisArrowSource->SetTipResolution(TipResolution);
#ifndef VTK_5
	xAxisMapper->SetInputConnection (m_xAxisArrowSource->GetOutputPort ( ));
#else	// VTK_5
	xAxisMapper->SetInput (m_xAxisArrowSource->GetOutput ( ));
#endif	// VTK_5
	xAxisMapper->ScalarVisibilityOff();
	m_xAxisActor->SetMapper(xAxisMapper);
	m_xVectorText->SetText("x");
#ifndef VTK_5
	xLabelMapper->SetInputConnection(m_xVectorText->GetOutputPort());
#else	// VTK_5
	xLabelMapper->SetInput(m_xVectorText->GetOutput());
#endif	// VTK_5
	m_xLabelActor->SetMapper(xLabelMapper);
	m_xLabelActor->SetScale(0.4,0.4,0.4);
	m_xLabelActor->AddPosition(1.1,0.0,0.0);
	m_xTextMapper->SetInput ("x");
	m_xTextMapper->GetTextProperty ( )->SetFontFamilyToArial ( );
	m_xTextMapper->GetTextProperty ( )->SetFontSize (24);
	m_xTextMapper->GetTextProperty ( )->BoldOn ( );
	m_xTextMapper->GetTextProperty ( )->ShadowOn ( );
	m_xLabelActor2D->VisibilityOn ( );
	m_xLabelActor2D->PickableOff ( );
	m_xLabelActor2D->SetMapper (m_xTextMapper);
	m_xLabelActor2D->GetPositionCoordinate ( )->SetCoordinateSystemToWorld  ( );
	// Pb si on met (1.1, 0., 0.) : clipé en Y. Pourquoi ???
	m_xLabelActor2D->GetPositionCoordinate ( )->SetValue (0.8, 0.1, 0.);

	// Création de l'axe Y
	m_yAxisArrowSource->SetShaftResolution(ArrowResolution);
	m_yAxisArrowSource->SetTipResolution(TipResolution);
#ifndef VTK_5
	yAxisMapper->SetInputConnection (m_yAxisArrowSource->GetOutputPort ( ));
#else	// VTK_5
	yAxisMapper->SetInput(m_yAxisArrowSource->GetOutput());
#endif	// VTK_5
	yAxisMapper->ScalarVisibilityOff();
	m_yAxisActor->SetMapper(yAxisMapper);
	m_yAxisActor->RotateZ(90);
	m_yVectorText->SetText("y");
#ifndef VTK_5
	yLabelMapper->SetInputConnection(m_yVectorText->GetOutputPort());
#else	// VTK_5
	yLabelMapper->SetInput(m_yVectorText->GetOutput());
#endif	// VTK_5
	m_yLabelActor->SetMapper(yLabelMapper);
	m_yLabelActor->SetScale(0.4,0.4,0.4);
	m_yLabelActor->AddPosition(0.0,1.1,0.0);
	m_yLabelActor->RotateZ (90);
	m_yTextMapper->SetInput ("y");
	m_yTextMapper->GetTextProperty ( )->SetFontFamilyToArial ( );
	m_yTextMapper->GetTextProperty ( )->SetFontSize (24);
	m_yTextMapper->GetTextProperty ( )->BoldOn ( );
	m_yTextMapper->GetTextProperty ( )->ShadowOn ( );
	m_yLabelActor2D->VisibilityOn ( );
	m_yLabelActor2D->PickableOff ( );
	m_yLabelActor2D->SetMapper (m_yTextMapper);
	m_yLabelActor2D->GetPositionCoordinate ( )->SetCoordinateSystemToWorld  ( );
	m_yLabelActor2D->GetPositionCoordinate ( )->SetValue (0.1, 0.8, 0.);

	// Création de l'axe Z
	m_zAxisArrowSource->SetShaftResolution(ArrowResolution);
	m_zAxisArrowSource->SetTipResolution(TipResolution);
#ifndef VTK_5
	zAxisMapper->SetInputConnection (m_zAxisArrowSource->GetOutputPort ( ));
#else	// VTK_5
	zAxisMapper->SetInput(m_zAxisArrowSource->GetOutput());
#endif	// VTK_5
	zAxisMapper->ScalarVisibilityOff();
	m_zAxisActor->SetMapper(zAxisMapper);
	m_zAxisActor->RotateY(-90);
	m_zVectorText->SetText("z");
#ifndef VTK_5
	zLabelMapper->SetInputConnection(m_zVectorText->GetOutputPort());
#else	// VTK_5
	zLabelMapper->SetInput(m_zVectorText->GetOutput());
#endif	// VTK_5
	m_zLabelActor->SetMapper(zLabelMapper);
	m_zLabelActor->SetScale(0.4,0.4,0.4);
	m_zLabelActor->AddPosition(0.0,0.0,1.1);
	m_zLabelActor->RotateY (-90);
	m_zTextMapper->SetInput ("z");
	m_zTextMapper->GetTextProperty ( )->SetFontFamilyToArial ( );
	m_zTextMapper->GetTextProperty ( )->SetFontSize (24);
	m_zTextMapper->GetTextProperty ( )->BoldOn ( );
	m_zTextMapper->GetTextProperty ( )->ShadowOn ( );
	m_zLabelActor2D->VisibilityOn ( );
	m_zLabelActor2D->PickableOff ( );
	m_zLabelActor2D->SetMapper (m_zTextMapper);
	m_zLabelActor2D->GetPositionCoordinate ( )->SetCoordinateSystemToWorld  ( );
	m_zLabelActor2D->GetPositionCoordinate ( )->SetValue (0., 0.1, 0.8);

	AddPart (m_xAxisActor);
	AddPart (m_yAxisActor);
	AddPart (m_zAxisActor);
	AddPart (m_xLabelActor);
	AddPart (m_yLabelActor);
	AddPart (m_zLabelActor);
	
	// nettoyage mémoire
	xAxisMapper->Delete();
	yAxisMapper->Delete();
	zAxisMapper->Delete();
	xLabelMapper->Delete();
	yLabelMapper->Delete();
	zLabelMapper->Delete();

	SetAbscissaColor (xColor [0], xColor [1], xColor [2]);
	SetOrdinateColor (yColor [0], yColor [1], yColor [2]);
	SetElevationColor (zColor [0], zColor [1], zColor [2]);
}	// vtkTrihedron::vtkTrihedron


vtkTrihedron::vtkTrihedron (const vtkTrihedron&)
{
	assert (0 && "vtkTrihedron copy constructor is not allowed.");
}	// vtkTrihedron::vtkTrihedron (const vtkTrihedron&)


vtkTrihedron& vtkTrihedron::operator = (const vtkTrihedron&)
{
	assert (0 && "vtkTrihedron operator = is not allowed.");
	return *this;
}	// vtkTrihedron::operator =


vtkTrihedron::~vtkTrihedron ( )
{
	if (0 != m_transform)
		m_transform->Delete ( );
	m_transform	= 0;

	if (0 != m_xVectorText)
		m_xVectorText->Delete ( );
	m_xVectorText	= 0;
	if (0 != m_yVectorText)
		m_yVectorText->Delete ( );
	m_yVectorText	= 0;
	if (0 != m_zVectorText)
		m_zVectorText->Delete ( );
	m_zVectorText	= 0;
	if (0 != m_xAxisActor)
	{
		RemovePart (m_xAxisActor);
		m_xAxisActor->Delete ( );
		m_xAxisActor	= 0;
	}
	if (0 != m_yAxisActor)
	{
		RemovePart (m_yAxisActor);
		m_yAxisActor->Delete ( );
		m_yAxisActor	= 0;
	}
	if (0 != m_zAxisActor)
	{
		RemovePart (m_zAxisActor);
		m_zAxisActor->Delete ( );
		m_zAxisActor	= 0;
	}
	if (0 != m_xLabelActor)
	{
		RemovePart (m_xLabelActor);
		m_xLabelActor->Delete ( );
		m_xLabelActor	= 0;
	}
	if (0 != m_yLabelActor)
	{
		RemovePart (m_yLabelActor);
		m_yLabelActor->Delete ( );
		m_yLabelActor	= 0;
	}
	if (0 != m_zLabelActor)
	{
		RemovePart (m_zLabelActor);
		m_zLabelActor->Delete ( );
		m_zLabelActor	= 0;
	}
	if (0 != m_xLabelActor2D)
	{
		RemovePart (m_xLabelActor2D);
		m_xLabelActor2D->Delete ( );
		m_xTextMapper->Delete ( );
		m_xLabelActor2D	= 0;
		m_xTextMapper	= 0;
	}
	if (0 != m_yLabelActor2D)
	{
		RemovePart (m_yLabelActor2D);
		m_yLabelActor2D->Delete ( );
		m_yTextMapper->Delete ( );
		m_yLabelActor2D	= 0;
		m_yTextMapper	= 0;
	}
	if (0 != m_zLabelActor2D)
	{
		RemovePart (m_zLabelActor2D);
		m_zLabelActor2D->Delete ( );
		m_zTextMapper->Delete ( );
		m_zLabelActor2D	= 0;
		m_zTextMapper	= 0;
	}
	if (0 != m_xAxisArrowSource)
		m_xAxisArrowSource->Delete ( );
	m_xAxisArrowSource	= 0;
	if (0 != m_yAxisArrowSource)
		m_yAxisArrowSource->Delete ( );
	m_yAxisArrowSource	= 0;
	if (0 != m_zAxisArrowSource)
		m_zAxisArrowSource->Delete ( );
	m_zAxisArrowSource	= 0;
}	// vtkTrihedron::~vtkTrihedron


vtkTrihedron* vtkTrihedron::New ( )
{
	vtkObject*	object	= vtkObjectFactory::CreateInstance ("vtkTrihedron");
	if (0 != object)
		return (vtkTrihedron*)object;

    return new vtkTrihedron;
}	// vtkTrihedron::New


vtkTransform* vtkTrihedron::GetTransform ( )
{
	return m_transform;
}	// vtkTrihedron::GetTransform


void vtkTrihedron::SetTransform (vtkTransform* transform)
{
	if (transform == m_transform)
		return;

	if (0 != m_transform)
		m_transform->UnRegister (0);
	m_transform	= transform;
	if (0 != m_transform)
		m_transform->Register (0);

	if (0 != m_xAxisActor)
		m_xAxisActor->SetUserTransform (m_transform);
	if (0 != m_yAxisActor)
		m_yAxisActor->SetUserTransform (m_transform);
	if (0 != m_zAxisActor)
		m_zAxisActor->SetUserTransform (m_transform);
	if (0 != m_xLabelActor)
		m_xLabelActor->SetUserTransform (m_transform);
	if (0 != m_yLabelActor)
		m_yLabelActor->SetUserTransform (m_transform);
	if (0 != m_zLabelActor)
		m_zLabelActor->SetUserTransform (m_transform);
	double	center [3]	= { 0., 0., 0. };
	if (0 != m_transform)
		m_transform->GetPosition (center);
	if (0 != m_xLabelActor2D)
	{
		double	coords [3]	= { 0.8, 0.1, 0. };
		if (0 != m_transform)
			m_transform->MultiplyPoint (coords, coords);
		m_xLabelActor2D->GetPositionCoordinate( )->SetCoordinateSystemToWorld();
		m_xLabelActor2D->GetPositionCoordinate( )->SetValue (
						coords [0] + center [0], coords [1] + center [1],
						coords [2] + center [2]);
	}	// if (0 != m_xLabelActor2D)
	if (0 != m_yLabelActor2D)
	{
		double	coords [3]	= { 0.1, 0.8, 0. };
		if (0 != m_transform)
			m_transform->MultiplyPoint (coords, coords);
		m_yLabelActor2D->GetPositionCoordinate( )->SetCoordinateSystemToWorld();
		// vtkActor2D : ne pas prendre en compte la translation en Y. Pourquoi ?
		m_yLabelActor2D->GetPositionCoordinate( )->SetValue (
//						coords [0] + center [0], coords [1] + center [1],
//						coords [2] + center [2]);
						coords [0], coords [1], coords [2]);
	}	// if (0 != m_yLabelActor2D)
	if (0 != m_zLabelActor2D)
	{
		double	coords [3]	= { 0., 0.1, 0.8 };
		if (0 != m_transform)
			m_transform->MultiplyPoint (coords, coords);
		m_zLabelActor2D->GetPositionCoordinate( )->SetCoordinateSystemToWorld();
		m_zLabelActor2D->GetPositionCoordinate( )->SetValue (
						coords [0] + center [0], coords [1] + center [1],
						coords [2] + center [2]);
	}	// if (0 != m_zLabelActor2D)
}	// vtkTrihedron::SetTransform


void vtkTrihedron::SetAxisLabels (const char* x, const char* y, const char* z)
{
	if (0 != m_xVectorText)
		m_xVectorText->SetText (x);
	if (0 != m_xTextMapper)
		m_xTextMapper->SetInput (x);
	if (0 != m_yVectorText)
		m_yVectorText->SetText (y);
	if (0 != m_yTextMapper)
		m_yTextMapper->SetInput (y);
	if (0 != m_zVectorText)
		m_zVectorText->SetText (z);
	if (0 != m_zTextMapper)
		m_zTextMapper->SetInput (z);
}	// vtkTrihedron::SetAxisLabels


void vtkTrihedron::SetLabelsOffsets (
							double xOffset, double yOffset, double zOffset)
{
	if (0 != m_xLabelActor)
		m_xLabelActor->AddPosition (0., xOffset, 0.);
	if (0 != m_yLabelActor)
		m_yLabelActor->AddPosition (yOffset, 0., 0.);
	if (0 != m_zLabelActor)
		m_zLabelActor->AddPosition (0., zOffset, 0.);
	// Rem CP : SetLineOffset => offset vertical, mais sans effet apparent.
	if ((0 != m_xTextMapper) && (0 != m_xTextMapper->GetTextProperty ( )))
	{
		m_xTextMapper->GetTextProperty ( )->SetVerticalJustificationToBottom( );
		m_xTextMapper->GetTextProperty ( )->SetLineOffset (xOffset);
	}	// if ((0 != m_xTextMapper) && ...
	if ((0 != m_yTextMapper) && (0 != m_yTextMapper->GetTextProperty ( )))
	{
		m_yTextMapper->GetTextProperty ( )->SetVerticalJustificationToBottom( );
		m_yTextMapper->GetTextProperty ( )->SetLineOffset (yOffset);
	}	// if ((0 != m_yTextMapper) && ...
	if ((0 != m_zTextMapper) && (0 != m_zTextMapper->GetTextProperty ( )))
	{
		m_zTextMapper->GetTextProperty ( )->SetVerticalJustificationToBottom( );
		m_zTextMapper->GetTextProperty ( )->SetLineOffset (zOffset);
	}	// if ((0 != m_zTextMapper) && ...
}	// vtkTrihedron::SetLabelsOffsets


void vtkTrihedron::SetAxisThicknessScale (double scale)
{
	GetXAxisArrowSource ( ).SetShaftRadius (m_shaftRadius * scale);
	GetXAxisArrowSource ( ).SetTipRadius (m_tipRadius * scale);
	GetXAxisArrowSource ( ).SetTipLength (m_tipLength * scale);
	GetYAxisArrowSource ( ).SetShaftRadius (m_shaftRadius * scale);
	GetYAxisArrowSource ( ).SetTipRadius (m_tipRadius * scale);
	GetYAxisArrowSource ( ).SetTipLength (m_tipLength * scale);
	GetZAxisArrowSource ( ).SetShaftRadius (m_shaftRadius * scale);
	GetZAxisArrowSource ( ).SetTipRadius (m_tipRadius * scale);
	GetZAxisArrowSource ( ).SetTipLength (m_tipLength * scale);
}	// vtkTrihedron::SetAxisThicknessScale


void vtkTrihedron::GetAbscissaColor (double& r, double& g, double& b)
{
	vtkFloatingPointType	color [3]	= {0., 0., 0.};

	if (0 != m_xAxisActor)
		m_xAxisActor->GetProperty ( )->GetColor (color);

	r	= color [0];
	g	= color [1];
	b	= color [2];
}	// vtkTrihedron::GetAbscissaColor


void vtkTrihedron::SetAbscissaColor (double r, double g, double b)
{
	if (0 != m_xAxisActor)
		m_xAxisActor->GetProperty ( )->SetColor (r, g, b);
	if (0 != m_xLabelActor)
		m_xLabelActor->GetProperty ( )->SetColor (r, g, b);
	if (0 != m_xTextMapper)
		m_xTextMapper->GetTextProperty ( )->SetColor (r, g, b);
}	// vtkTrihedron::SetAbscissaColor


void vtkTrihedron::GetOrdinateColor (double& r, double& g, double& b)
{
	vtkFloatingPointType	color [3]	= {0., 0., 0.};

	if (0 != m_yAxisActor)
		m_yAxisActor->GetProperty ( )->GetColor (color);

	r	= color [0];
	g	= color [1];
	b	= color [2];
}	// vtkTrihedron::GetOrdinateColor


void vtkTrihedron::SetOrdinateColor (double r, double g, double b)
{
	if (0 != m_yAxisActor)
		m_yAxisActor->GetProperty ( )->SetColor (r, g, b);
	if (0 != m_yLabelActor)
		m_yLabelActor->GetProperty ( )->SetColor (r, g, b);
	if (0 != m_yTextMapper)
		m_yTextMapper->GetTextProperty ( )->SetColor (r, g, b);
}	// vtkTrihedron::SetOrdinateColor


void vtkTrihedron::GetElevationColor (double& r, double& g, double& b)
{
	vtkFloatingPointType	color [3]	= {0., 0., 0.};

	if (0 != m_zAxisActor)
		m_zAxisActor->GetProperty ( )->GetColor (color);

	r	= color [0];
	g	= color [1];
	b	= color [2];
}	// vtkTrihedron::GetElevationColor


void vtkTrihedron::SetElevationColor (double r, double g, double b)
{
	if (0 != m_zAxisActor)
		m_zAxisActor->GetProperty ( )->SetColor (r, g, b);
	if (0 != m_zLabelActor)
		m_zLabelActor->GetProperty ( )->SetColor (r, g, b);
	if (0 != m_zLabelActor2D)
		m_zLabelActor2D->GetProperty ( )->SetColor (r, g, b);
	if (0 != m_zTextMapper)
		m_zTextMapper->GetTextProperty ( )->SetColor (r, g, b);
}	// vtkTrihedron::SetElevationColor


void vtkTrihedron::SetLabel2D (bool on)
{
	if (true == on)
		Label2DOn ( );
	else
		Label2DOff ( );
}	// vtkTrihedron::SetLabel2D


void vtkTrihedron::Label2DOn ( )
{
	if (0 != m_xLabelActor)
		RemovePart (m_xLabelActor);
	if (0 != m_yLabelActor)
		RemovePart (m_yLabelActor);
	if (0 != m_zLabelActor)
		RemovePart (m_zLabelActor);
	if (0 != m_xLabelActor2D)
		AddPart (m_xLabelActor2D);
	if (0 != m_yLabelActor2D)
		AddPart (m_yLabelActor2D);
	if (0 != m_zLabelActor2D)
		AddPart (m_zLabelActor2D);
}	// vtkTrihedron::Label2DOn


void vtkTrihedron::Label2DOff ( )
{
	if (0 != m_xLabelActor)
		AddPart (m_xLabelActor);
	if (0 != m_yLabelActor)
		AddPart (m_yLabelActor);
	if (0 != m_zLabelActor)
		AddPart (m_zLabelActor);
	if (0 != m_xLabelActor2D)
		RemovePart (m_xLabelActor2D);
	if (0 != m_yLabelActor2D)
		RemovePart (m_yLabelActor2D);
	if (0 != m_zLabelActor2D)
		RemovePart (m_zLabelActor2D);
}	// vtkTrihedron::Label2DOff


vtkArrowSource& vtkTrihedron::GetXAxisArrowSource ( )
{
	assert (0 != m_xAxisArrowSource);
	return *m_xAxisArrowSource;
};	// vtkTrihedron::GetXAxisArrowSource


vtkArrowSource& vtkTrihedron::GetYAxisArrowSource ( )
{
	assert (0 != m_yAxisArrowSource);
	return *m_yAxisArrowSource;
};	// vtkTrihedron::GetYAxisArrowSource


vtkArrowSource& vtkTrihedron::GetZAxisArrowSource ( )
{
	assert (0 != m_zAxisArrowSource);
	return *m_zAxisArrowSource;
};	// vtkTrihedron::GetZAxisArrowSource


vtkActor& vtkTrihedron::GetXAxisArrowActor ( )
{
	assert (0 != m_xAxisActor);
	return *m_xAxisActor;
};	// vtkTrihedron::GetXAxisArrowActor


vtkActor& vtkTrihedron::GetYAxisArrowActor ( )
{
	assert (0 != m_yAxisActor);
	return *m_yAxisActor;
};	// vtkTrihedron::GetYAxisArrowActor


vtkActor& vtkTrihedron::GetZAxisArrowActor ( )
{
	assert (0 != m_zAxisActor);
	return *m_zAxisActor;
}	// vtkTrihedron::GetZAxisArrowActor


vtkActor* vtkTrihedron::GetXAxisLabelActor ( )
{
	return m_xLabelActor;
};	// vtkTrihedron::GetXAxisLabelActor


vtkActor* vtkTrihedron::GetYAxisLabelActor ( )
{
	return m_yLabelActor;
};	// vtkTrihedron::GetYAxisLabelActor


vtkActor* vtkTrihedron::GetZAxisLabelActor ( )
{
	return m_zLabelActor;
};	// vtkTrihedron::GetZAxisLabelActor



