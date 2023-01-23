/**
 * @file    vtk2dAxiDihedron.cpp
 *  @author  C. PIGNEROL
 *  @date    18/09/2019
 */

#ifndef VTK_5

#include "VtkContrib/vtk2dAxiDihedron.h"

#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>

#include <assert.h>


vtk2dAxiDihedron::vtk2dAxiDihedron ( )
	: vtkTrihedron ( ), m_zShaftActor (0), m_zTipActor (0),
	  m_zShaftCurveSource (0), m_zTipCurveSource (0)
{
	SetAxisLabels ("Z", "X", "Y");
	GetZAxisArrowSource ( ).SetTipLength (0.5);
	m_zShaftCurveSource	= vtkPolyLineSource::New ( );
	const int	pointsNum	= 271;
	m_zShaftCurveSource->SetNumberOfPoints (pointsNum);
	const double	radius	= 0.5;
	for (int i = 0; i < pointsNum; i++)
	{
		const double theta	= -M_PI/2. + M_PI / 180. * (double)i;
		const double	x	= 0.;
		const double	y	= radius * cos (theta);
		const double	z	= radius * sin (theta);
		m_zShaftCurveSource->SetPoint (i, x, y, z);
	}	// for (int i = 0; i < pointsNum; i++)
//	m_zShaftCurveSource->SetPoint (
//		pointsNum, 0., -radius, GetZAxisArrowSource ( ).GetTipLength ( ) - 1.);
	m_zShaftCurveSource->ClosedOff ( );
	vtkPolyDataMapper*	zShaftCurveMapper	= vtkPolyDataMapper::New ( );
	m_zShaftActor							= vtkActor::New ( );
	m_zShaftActor->GetProperty ( )->RenderLinesAsTubesOn ( );
	m_zShaftActor->GetProperty ( )->SetLineWidth (4);
	m_zShaftActor->GetProperty ( )->SetColor (vtkTrihedron::zColor);
	m_zShaftActor->AddPosition (1., 0., 0.);
	GetZAxisArrowActor ( ).AddPosition (1., -radius, -1.);
	GetZAxisArrowSource ( ).InvertOn ( );
	if (0 != GetZAxisLabelActor ( ))
		GetZAxisLabelActor ( )->AddPosition (1., -radius, -2.1);
#ifndef VTK_5
	zShaftCurveMapper->SetInputConnection (m_zShaftCurveSource->GetOutputPort ( ));
#else	// VTK_5
	zShaftCurveMapper->SetInput (m_zShaftCurveSource->GetOutput ( ));
#endif	// VTK_5
	zShaftCurveMapper->ScalarVisibilityOff ( );
	m_zShaftActor->SetMapper (zShaftCurveMapper);
	AddPart (m_zShaftActor);
	zShaftCurveMapper->Delete ( );	zShaftCurveMapper	= 0;

	m_zTipCurveSource	= vtkConeSource::New ( );
	m_zTipCurveSource->SetRadius (GetZAxisArrowSource ( ).GetTipRadius ( ));
	m_zTipCurveSource->SetHeight (GetZAxisArrowSource ( ).GetTipLength ( ));
	m_zTipCurveSource->SetResolution (18);
	m_zTipCurveSource->SetDirection (0., 0., -1.);
	m_zTipCurveSource->SetCenter (
				1., -radius, -GetZAxisArrowSource ( ).GetTipLength ( ) / 2.);
	vtkPolyDataMapper*	zTipConeMapper	= vtkPolyDataMapper::New ( );
	zTipConeMapper->ScalarVisibilityOff ( );
#ifndef VTK_5
	zTipConeMapper->SetInputConnection (m_zTipCurveSource->GetOutputPort ( ));
#else	// VTK_5
	zTipConeMapper->SetInput (m_zTipCurveSource->GetOutput ( ));
#endif	// VTK_5
	m_zTipActor	= vtkActor::New ( );
	m_zTipActor->SetMapper (zTipConeMapper);
	m_zTipActor->GetProperty ( )->SetColor (vtkTrihedron::zColor);
	AddPart (m_zTipActor);
	zTipConeMapper->Delete ( );		zTipConeMapper	= 0;

	// On désactive la flèche Z héritée :
	GetZAxisArrowActor ( ).VisibilityOff ( );
}	// vtk2dAxiDihedron::vtk2dAxiDihedron


vtk2dAxiDihedron::vtk2dAxiDihedron (const vtk2dAxiDihedron&)
//	: vtkTrihedron ( )
	: m_zShaftActor (0), m_zTipActor (0),
	  m_zShaftCurveSource (0), m_zTipCurveSource (0)
{
	assert (0 && "vtk2dAxiDihedron copy constructor is not allowed.");
}	// vtk2dAxiDihedron::vtk2dAxiDihedron


vtk2dAxiDihedron& vtk2dAxiDihedron::operator = (const vtk2dAxiDihedron&)
{
	assert (0 && "vtk2dAxiDihedron assignment operator is not allowed.");
	return *this;
}	// vtk2dAxiDihedron::operator =


vtk2dAxiDihedron::~vtk2dAxiDihedron ( )
{
	if (0 != m_zShaftActor)
	{
		RemovePart (m_zShaftActor);
		m_zShaftActor->Delete ( );
		m_zShaftActor	= 0;
	}	// if (0 != m_zShaftActor)
	if (0 != m_zShaftCurveSource)
	{
		m_zShaftCurveSource->Delete ( );
		m_zShaftCurveSource	= 0;
	}	// if (0 != m_zShaftCurveSource)
	if (0 != m_zTipActor)
	{
		RemovePart (m_zTipActor);
		m_zTipActor->Delete ( );
		m_zTipActor	= 0;
	}	// if (0 != m_zTipActor)
	if (0 != m_zTipCurveSource)
	{
		m_zTipCurveSource->Delete ( );
		m_zTipCurveSource	= 0;
	}	// if (0 != m_zTipCurveSource)
}	// vtk2dAxiDihedron::~vtk2dAxiDihedron


vtk2dAxiDihedron* vtk2dAxiDihedron::New ( )
{
	vtkObject*	object	= vtkObjectFactory::CreateInstance ("vtk2dAxiDihedron");
	if (0 != object)
		return (vtk2dAxiDihedron*)object;

	return new vtk2dAxiDihedron;
}	// vtk2dAxiDihedron::New


void vtk2dAxiDihedron::SetTransform (vtkTransform* transform)
{
	if (transform == GetTransform ( ))
		return;

	vtkTrihedron::SetTransform (transform);

	if (0 != m_zShaftActor)
		m_zShaftActor->SetUserTransform (transform);
	if (0 != m_zTipActor)
		m_zTipActor->SetUserTransform (transform);
}	// vtk2dAxiDihedron::SetTransform


void vtk2dAxiDihedron::SetAxisThicknessScale (double scale)
{
	vtkTrihedron::SetAxisThicknessScale (scale);

	if (0 != m_zShaftActor)
		m_zShaftActor->GetProperty ( )->SetLineWidth (4. * scale);
	if (0 != m_zTipCurveSource)
		m_zTipCurveSource->SetRadius (GetZAxisArrowSource ( ).GetTipRadius ( ));
}	// vtk2dAxiDihedron::SetAxisThicknessScale


void vtk2dAxiDihedron::SetElevationColor (double r, double g, double b)
{
	vtkTrihedron::SetElevationColor (r, g, b);

	if (0 != m_zShaftActor)
		m_zShaftActor->GetProperty ( )->SetColor (r, g, b);
	if (0 != m_zTipActor)
		m_zTipActor->GetProperty ( )->SetColor (r, g, b);
}	// vtk2dAxiDihedron::SetElevationColor

#endif	// VTK_5
