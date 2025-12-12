#ifndef VTK_ELLIPSOID_H
#define VTK_ELLIPSOID_H

#include <vtkImplicitFunction.h>


/**
 * <P>Classe décrivant une ellipsoïde selon les axes Ox, Oy et Oz.</P>
 * 
 * <P>Cette classe étant une <I>vtkImplicitFunction</I>, il convient d'en positionner et orienter les instances
 * dans le repère cartésien (0, i, j, k) par transformation inverse avant d'utiliser ses fonctions Evaluate*.
 * => Instancier une <I>vtkTransform</I> commençant par une translation vers 0 puis effectuant des rotations 
 * selon Ox, Oy puis Oz que l'on lui affecte à l'instance via <I>SetTransform</I>.
 * </P>
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
