#include <VtkContrib/vtkEllipsoid.h>
#include <vtkObjectFactory.h>


vtkStandardNewMacro (vtkEllipsoid);


vtkEllipsoid::vtkEllipsoid (const vtkEllipsoid&)
	: vtkImplicitFunction ( ), XRadius (1.), YRadius (1.), ZRadius (1.)
{
}	// vtkEllipsoid::vtkEllipsoid


vtkEllipsoid& vtkEllipsoid::operator = (const vtkEllipsoid&)
{
	return *this;
}	// vtkEllipsoid::operator =


vtkEllipsoid::vtkEllipsoid ( )
{
	this->XRadius = 1.0;
	this->YRadius = 1.0;
	this->ZRadius = 1.0;
}	// vtkEllipsoid::vtkEllipsoid


vtkEllipsoid::~vtkEllipsoid ( )
{
}	// vtkEllipsoid::~vtkEllipsoid


double vtkEllipsoid::EvaluateFunction (double x [3])
{
	double	x2	= x [0] * x [0];
	double	y2	= x [1] * x [1];
	double	z2	= x [2] * x [2];
	return (x2 / (this->XRadius * this->XRadius) + y2 / (this->YRadius * this->YRadius) + z2 / (this->ZRadius * this->ZRadius)) - 1.0;
}	// vtkEllipsoid::EvaluateFunction


void vtkEllipsoid::EvaluateGradient (double x [3], double g [3])
{
	g [0] = 2.0 * x [0] / (this->XRadius * this->XRadius);
	g [1] = 2.0 * x [1] / (this->YRadius * this->YRadius);
	g [2] = 2.0 * x [2] / (this->ZRadius * this->ZRadius);
}	// vtkEllipsoid::EvaluateGradient


void vtkEllipsoid::PrintSelf (ostream& os, vtkIndent indent)
{
        this->Superclass::PrintSelf(os,indent);

        os << "XRadius: " << this->XRadius << ", YRadius:" << this->YRadius << ", ZRadius:" << this->ZRadius << "\n";
}       // vtkEllipsoid::PrintSelf
