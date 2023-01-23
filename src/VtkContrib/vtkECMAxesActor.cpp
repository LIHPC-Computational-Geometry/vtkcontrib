#include "VtkContrib/vtkECMAxesActor.h"
#include "VtkContrib/vtkFloatingPointType.h"

#include <vtkPolyDataMapper.h>
#include <vtkPolyData.h>
#include <vtkProperty.h>
#include <vtkObjectFactory.h>
#include <vtkProp3DCollection.h>


#include <assert.h>


double	vtkECMAxesActor::xColor [3]	= {1., 0., 0.};
double	vtkECMAxesActor::yColor [3]	= {0., 1., 0.};
double	vtkECMAxesActor::zColor [3]	= {0., 0., 1.};


vtkECMAxesActor::vtkECMAxesActor ( )
	: vtkPropAssembly ( ), 
	  _xAxisActor (0), _yAxisActor (0), _zAxisActor (0),
	  _xAxis (0), _yAxis (0), _zAxis (0)
{
	_xAxis = vtkLineSource::New ( );
	_yAxis = vtkLineSource::New ( );
	_zAxis = vtkLineSource::New ( );
	
	vtkPolyDataMapper *xAxisMapper  = vtkPolyDataMapper::New ( );
	vtkPolyDataMapper *yAxisMapper  = vtkPolyDataMapper::New ( );
	vtkPolyDataMapper *zAxisMapper  = vtkPolyDataMapper::New ( );
	
	_xAxisActor	= vtkActor::New();
	_yAxisActor	= vtkActor::New();
	_zAxisActor	= vtkActor::New();
	
	// Création de l'axe X
	_xAxis->SetPoint1 (-1., 0., 0.);
	_xAxis->SetPoint2 (1., 0., 0.);
#ifndef VTK_5
	xAxisMapper->SetInputConnection (_xAxis->GetOutputPort ( ));
#else	// VTK_5
	xAxisMapper->SetInput (_xAxis->GetOutput ( ));
#endif	// VTK_5
	xAxisMapper->ScalarVisibilityOff ( );
	_xAxisActor->SetMapper (xAxisMapper);

	// Création de l'axe Y
	_yAxis->SetPoint1 (0., -1., 0.);
	_yAxis->SetPoint2 (0., 1., 0.);
#ifndef VTK_5
	yAxisMapper->SetInputConnection (_yAxis->GetOutputPort ( ));
#else	// VTK_5
	yAxisMapper->SetInput (_yAxis->GetOutput ( ));
#endif	// VTK_5
	yAxisMapper->ScalarVisibilityOff ( );
	_yAxisActor->SetMapper (yAxisMapper);

	// Création de l'axe Z
	_zAxis->SetPoint1 (0., 0., -1.);
	_zAxis->SetPoint2 (0., 0., 1.);
#ifndef VTK_5
	zAxisMapper->SetInputConnection (_zAxis->GetOutputPort ( ));
#else	// VTK_5
	zAxisMapper->SetInput (_zAxis->GetOutput ( ));
#endif	// VTK_5
	zAxisMapper->ScalarVisibilityOff ();
	_zAxisActor->SetMapper (zAxisMapper);

	AddPart (_xAxisActor);
	AddPart (_yAxisActor);
	AddPart (_zAxisActor);
	
	// nettoyage mémoire
	xAxisMapper->Delete ( );	xAxisMapper	= 0;
	yAxisMapper->Delete ( );	yAxisMapper	= 0;
	zAxisMapper->Delete ( );	zAxisMapper	= 0;

	SetAbscissaColor (xColor [0], xColor [1], xColor [2]);
	SetOrdinateColor (yColor [0], yColor [1], yColor [2]);
	SetElevationColor (zColor [0], zColor [1], zColor [2]);
}	// vtkECMAxesActor::vtkECMAxesActor


vtkECMAxesActor::vtkECMAxesActor (const vtkECMAxesActor&)
	: vtkPropAssembly ( ), 
	  _xAxisActor (0), _yAxisActor (0), _zAxisActor (0),
	  _xAxis (0), _yAxis (0), _zAxis (0)
{
	assert (0 && "vtkECMAxesActor copy constructor is not allowed.");
}	// vtkECMAxesActor::vtkECMAxesActor (const vtkECMAxesActor&)


vtkECMAxesActor& vtkECMAxesActor::operator = (const vtkECMAxesActor&)
{
	assert (0 && "vtkECMAxesActor operator = is not allowed.");
	return *this;
}	// vtkECMAxesActor::operator =


vtkECMAxesActor::~vtkECMAxesActor ( )
{
	if (0 != _xAxis)
		_xAxis->Delete ( );
	_xAxis		= 0;
	if (0 != _yAxis)
		_yAxis->Delete ( );
	_yAxis		= 0;
	if (0 != _zAxis)
		_zAxis->Delete ( );
	_zAxis		= 0;
	if (0 != _xAxisActor)
	{
		RemovePart (_xAxisActor);
		_xAxisActor->Delete ( );
		_xAxisActor	= 0;
	}
	if (0 != _yAxisActor)
	{
		RemovePart (_yAxisActor);
		_yAxisActor->Delete ( );
		_yAxisActor	= 0;
	}
	if (0 != _zAxisActor)
	{
		RemovePart (_zAxisActor);
		_zAxisActor->Delete ( );
		_zAxisActor	= 0;
	}
}	// vtkECMAxesActor::~vtkECMAxesActor


vtkECMAxesActor* vtkECMAxesActor::New ( )
{
	vtkObject*	object	= vtkObjectFactory::CreateInstance ("vtkECMAxesActor");
	if (0 != object)
		return (vtkECMAxesActor*)object;

    return new vtkECMAxesActor;
}	// vtkECMAxesActor::New


void vtkECMAxesActor::GetPosition (
				double& xmin, double& xmax, double& ymin, double& ymax, 
				double& zmin, double& zmax)
{
	double	coords [3]	= {0., 0., 0.};

	if (0 != _xAxis)
	{
		_xAxis->GetPoint1 (coords);
		xmin	= coords [0];
		_xAxis->GetPoint2 (coords);
		xmax	= coords [0];
	}
	else
		cerr << __FILE__ << ' ' << __LINE__ << " vtkECMAxesActor::GetPosition. Null X axis." << endl;
	if (0 != _yAxis)
	{
		_yAxis->GetPoint1 (coords);
		ymin	= coords [1];
		_yAxis->GetPoint2 (coords);
		ymax	= coords [1];
	}
	else
		cerr << __FILE__ << ' ' << __LINE__ << " vtkECMAxesActor::GetPosition. Null Y axis." << endl;
	if (0 != _zAxis)
	{
		_zAxis->GetPoint1 (coords);
		zmin	= coords [2];
		_zAxis->GetPoint2 (coords);
		zmax	= coords [2];
	}
	else
		cerr << __FILE__ << ' ' << __LINE__ << " vtkECMAxesActor::GetPosition. Null Z axis." << endl;
}	// vtkECMAxesActor::GetPosition

void vtkECMAxesActor::SetPosition (
				double xmin, double xmax, double ymin, double ymax, 
				double zmin, double zmax)
{
	const double	xc	= xmin + (xmax - xmin) / 2.;
	const double	yc	= ymin + (ymax - ymin) / 2.;
	const double	zc	= zmin + (zmax - zmin) / 2.;

	if (0 != _xAxis)
	{
		_xAxis->SetPoint1 (xmin, yc, zc);
		_xAxis->SetPoint2 (xmax, yc, zc);
	}
	else
		cerr << __FILE__ << ' ' << __LINE__ << " vtkECMAxesActor::SetPosition. Null X axis." << endl;
	if (0 != _yAxis)
	{
		_yAxis->SetPoint1 (xc, ymin, zc);
		_yAxis->SetPoint2 (xc, ymax, zc);
	}
	else
		cerr << __FILE__ << ' ' << __LINE__ << " vtkECMAxesActor::SetPosition. Null Y axis." << endl;
	if (0 != _zAxis)
	{
		_zAxis->SetPoint1 (xc, yc, zmin);
		_zAxis->SetPoint2 (xc, yc, zmax);
	}
	else
		cerr << __FILE__ << ' ' << __LINE__ << " vtkECMAxesActor::SetPosition. Null Z axis." << endl;
}	// vtkECMAxesActor::SetPosition



void vtkECMAxesActor::GetAbscissaColor (double& r, double& g, double& b)
{
	vtkFloatingPointType	color [3]	= {0., 0., 0.};

	if (0 != _xAxisActor)
		_xAxisActor->GetProperty ( )->GetColor (color);

	r	= color [0];
	g	= color [1];
	b	= color [2];
}	// vtkECMAxesActor::GetAbscissaColor


void vtkECMAxesActor::SetAbscissaColor (double r, double g, double b)
{
	if (0 != _xAxisActor)
		_xAxisActor->GetProperty ( )->SetColor (r, g, b);
}	// vtkECMAxesActor::SetAbscissaColor


void vtkECMAxesActor::GetOrdinateColor (double& r, double& g, double& b)
{
	vtkFloatingPointType	color [3]	= {0., 0., 0.};

	if (0 != _yAxisActor)
		_yAxisActor->GetProperty ( )->GetColor (color);

	r	= color [0];
	g	= color [1];
	b	= color [2];
}	// vtkECMAxesActor::GetOrdinateColor


void vtkECMAxesActor::SetOrdinateColor (double r, double g, double b)
{
	if (0 != _yAxisActor)
		_yAxisActor->GetProperty ( )->SetColor (r, g, b);
}	// vtkECMAxesActor::SetOrdinateColor


void vtkECMAxesActor::GetElevationColor (double& r, double& g, double& b)
{
	vtkFloatingPointType	color [3]	= {0., 0., 0.};

	if (0 != _zAxisActor)
		_zAxisActor->GetProperty ( )->GetColor (color);

	r	= color [0];
	g	= color [1];
	b	= color [2];
}	// vtkECMAxesActor::GetElevationColor


void vtkECMAxesActor::SetElevationColor (double r, double g, double b)
{
	if (0 != _zAxisActor)
		_zAxisActor->GetProperty ( )->SetColor (r, g, b);
}	// vtkECMAxesActor::SetElevationColor




