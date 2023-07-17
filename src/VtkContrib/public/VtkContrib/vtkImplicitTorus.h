#ifndef VTK_IMPLICIT_TORUS_H
#define VTK_IMPLICIT_TORUS_H

/**
 * @class   vtkImplicitTorus
 * @brief   implicit function for a torus
 * @author		Charles PIGNEROL, CEA/DAM/DSSI
 *
 * vtkImplicitTorus computes the implicit function and/or gradient for a torus.
 */



#include <vtkCommonDataModelModule.h>
#include <vtkImplicitFunction.h>

class vtkImplicitTorus : public vtkImplicitFunction
{
	public :

	vtkTypeMacro (vtkImplicitTorus, vtkImplicitFunction);
	virtual void PrintSelf (ostream& os, vtkIndent indent);


	/**
	 * Construct a torus in yOz plane along x-axis centered at (0,0,0) with ring radius = 0.5 and cross section radius = 0.1.
	 */
	static vtkImplicitTorus* New ( );

	/**
	 * Evaluate torus equation x^2 - (csr^2 - (sqrt (y^2 + z^2) - rr)^2) where csr is cross section radius
	 * and rr is ring radius.
	 */
	 virtual double EvaluateFunction (double coords [3]);
	 virtual double EvaluateFunction (double x, double y, double z);

	/**
	 * Evaluate torus gradient.
	 */
	virtual void EvaluateGradient (double coords [3], double gradient [3]);

	/**
	 * Set / get the center of the torus. Default is (0.,0., 0.).
	 */
	 vtkSetVector3Macro (Center, double);
	 vtkGetVectorMacro (Center, double, 3);
	 
	/**
	 * Set / get the cross section radius of the torus. Default is 0.1.
	 */
	vtkSetMacro (CrossSectionRadius, double);
	vtkGetMacro (CrossSectionRadius, double);

	/**
	 * Set / get the ring radius of the torus. Default is 0.5.
	 */
	vtkSetMacro (RingRadius, double);
	vtkGetMacro (RingRadius, double);


	protected :
	
	vtkImplicitTorus ( );
	virtual ~vtkImplicitTorus ( );

	double	Center [3];
	double	CrossSectionRadius, RingRadius;


	private :
	
	vtkImplicitTorus (const vtkImplicitTorus&);
	vtkImplicitTorus& operator = (const vtkImplicitTorus&);
};	// class vtkImplicitTorus


#endif	// VTK_IMPLICIT_TORUS_H
