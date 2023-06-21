#include "VtkContrib/vtkImplicitTorus.h"
#include <vtkObjectFactory.h>

#include <math.h>


vtkStandardNewMacro (vtkImplicitTorus);


vtkImplicitTorus::vtkImplicitTorus ( )
	: vtkImplicitFunction ( ), CrossSectionRadius (0.1), RingRadius (0.5)
{
	Center [0]	= Center [1]	= Center [2]	= 0.;
}	// vtkImplicitTorus::vtkImplicitTorus


vtkImplicitTorus::~vtkImplicitTorus ( )
{
}	// vtkImplicitTorus::~vtkImplicitTorus


double vtkImplicitTorus::EvaluateFunction (double coords [3])
{
	return EvaluateFunction (coords [0],coords [1], coords [2]);
}	// vtkImplicitTorus::EvaluateFunction


double vtkImplicitTorus::EvaluateFunction (double x, double y, double z)
{
	const double	dx2	= (x - Center [0]) * (x - Center [0]);
	const double	dy2	= (y - Center [1]) * (y - Center [1]);
	const double	dz2	= (z - Center [2]) * (z - Center [2]);
	
	return dx2 - (CrossSectionRadius * CrossSectionRadius - (sqrt (dy2 + dz2) - RingRadius) * (sqrt (dy2 + dz2) - RingRadius));
}	// vtkImplicitTorus::EvaluateFunction


void vtkImplicitTorus::EvaluateGradient (double coords [3], double gradient [3])
{	// Not validated. Algo issued from vtkCylinder::EvaluateGradient (VTK 7.1.1)
	// Torus is along elevation z-axis.

	// Determine the radial vector from the point x to the line. This means finding the closest point to the line. Get parametric
	// location along cylinder axis. Remember Axis is normalized.
	const double	Axis [3]	= { 1., 0., 0. };	// Attribute of class vtkCylinder ...
//	const double	t			= Axis [0] * (coords [0] - Center [0]) + Axis [1] * (coords [1] - Center [1]) + Axis [2] * (coords [2] - Center [2]);
	const double	t			= (coords [0] - Center [0]);

	// Compute closest point
	double	cp [3]	= { 0., 0., 0. };
	cp [0]	= Center [0] + t * Axis [0];
	cp [1]	= Center [1] + t * Axis [1];
	cp [2]	= Center [2] + t * Axis [2];

	// Gradient is 2*r. Project onto x-y-z axes.
	gradient [0]	= 2.0 * (coords [0] - cp [0]);
	gradient [1]	= 2.0 * (coords [1] - cp [1]);
	gradient [2]	= 2.0 * (coords [2] - cp [2]);

/* Code from vtkSphere :
 	gradient [0]	= 2.0 * coords [0];
	gradient [1]	= 2.0 * coords [1];
	gradient [2]	= 2.0 * coords [2];	*/
}	// vtkImplicitTorus::EvaluateGradient


void vtkImplicitTorus::PrintSelf (ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os,indent);

	os << "Center: (" << Center [0] << ", " << Center [1] << ", " << Center [2] << ")\n"
	   << indent << "CrossSectionRadius: " << CrossSectionRadius << "\n"
	   << indent << "RingRadius: " << RingRadius << ")\n";
}	// vtkImplicitTorus::PrintSelf
