#ifndef VTK_ELLIPSOID_H
#define VTK_ELLIPSOID_H

#include <vtkImplicitFunction.h>


/**
 * Classe décrivant une ellipsoïde selon les axes Ox, Oy et Oz.
 */
class vtkEllipsoid : public vtkImplicitFunction
{
	public:

	vtkTypeMacro(vtkEllipsoid, vtkImplicitFunction);

	/**
	 * Créé une ellipsoïde le long des axes Ox, Oy et Oz centrée en (0, 0, 0) de rayons 1.
	 */
	static vtkEllipsoid* New ( );

	virtual void PrintSelf (ostream& os, vtkIndent indent);

	/**
	 * Evalue la fonction implicite de l'ellipsoïde.
	 */
	virtual double EvaluateFunction (double x[3]);

	/**
	 * Evalue le gradient de l'ellipsoïde.
	 */
	virtual void EvaluateGradient (double x[3], double g[3]);

	/**
	 * Définition des rayons de l'ellipsoïde.
	 */
	vtkSetMacro (XRadius, double);
	vtkGetMacro (XRadius, double);
	vtkSetMacro (YRadius, double);
	vtkGetMacro (YRadius, double);
	vtkSetMacro (ZRadius, double);
	vtkGetMacro (ZRadius, double);


	protected :

	/**
	 * Constructeur. Centre l'ellipsoïde en (0, 0, 0) et affecte 1. à ses rayons.
	 */
	vtkEllipsoid ( );

	/**
	 * Destructeur. RAS.
	 */
	virtual ~vtkEllipsoid ( );

	/** Les rayonq de l'ellipsoïde. */
	double XRadius;
	double YRadius;
	double ZRadius;


	private :

	/**
	 * Constructeur de copie et opérateur = : interdits.
	 */
	vtkEllipsoid (const vtkEllipsoid&);
	vtkEllipsoid& operator = (const vtkEllipsoid&);
};	// class vtkEllipsoid


#endif	// VTK_ELLIPSOID_H
