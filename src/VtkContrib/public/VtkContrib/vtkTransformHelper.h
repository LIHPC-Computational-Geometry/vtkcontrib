/**
 * \file		vtkSimpleTransformMemento.h
 * \author		Charles PIGNEROL, CEA/DAM/DCLC
 * \date		24/03/2026
 */
#ifndef VTK_TRANSFORM_HELPER_H
#define VTK_TRANSFORM_HELPER_H

#include "VtkContrib/vtk_versions.h"
#include <vtkTransform.h>
#include <vtkProp3D.h>


/**
 * Classe permettant d'avoir des services autour de la classe vtkTransform de VTK.
 */
class vtkTransformHelper
{
	public :

	/**
	 * Effectue une action de "shrink" de la propriété reçue en premier argument du facteur donné en second argument. Cette action
	 * est matérialisée par une mise à l'échelle de "factor" et d'un déplacement.
	 */
	static void ShrinkProperty (vtkProp3D& prop3D, double factor);

	/** 
	 * Effectue une action de "shrink" de la propriété reçue en premier argument du facteur donné en second argument. Cette action
	 * est matérialisée par une mise à l'échelle de "factor" et d'un déplacement. Les coordonnées du centre de la propriété 
	 * sont transmises en argument, et non plus récupérées auprès de celle-ci.
	 */
	static void ShrinkProperty (vtkProp3D& prop3D, double factor, double xCenter, double yCenter, double zCenter);

	/**
	 * Classe permettant d'avoir une représentation structurée d'une instance "simple" de la classe vtkTransform, à savoir une translation et une 
	 * rotation autour de chaque axe, l'ensemble des transformations étant toutes intrinsèques ou extrinsèques.
	 * Classe créée à la base pour pouvoir, à partir d'une instance de cette classe conservée par exemple dans une IHM, recréer une instance de 
	 * vtkTransform à partir de la méthode vtkTransformHelper::CreateTransform (vtkSimpleTransformMemento).
	 *
	 * Une transformation intrinsèque est une transformation géométrique qui est appliquée directement à un objet ou à un système de coordonnées
	 * dans son propre repère. Contrairement aux transformations extrinsèques, qui sont appliquées par rapport à un repère externe, les 
	 * transformations intrinsèques modifient les propriétés internes de l'objet ou du repère lui-même (position, orientation, forme, taille).
	 * 
	 * Par convention :
	 * 
	 * Transformation extrinsèque :
	 * - Les mises à l'échelle sont effectuées en premier,
	 * - Selon translationFirst la translation est effectuée avant ou après les rotations,
	 * - Les rotations sont effectuées autour de Oz, puis autour de Ox, puis autour de Oy.
	 * 
	 * Transformation intrinsèque :
	 * - Les mises à l'échelle sont effectuées en premier,
	 * - Selon translationFirst la translation est effectuée avant ou après les rotations,
	 * - Les rotations sont effectuées autour de Oy (angle phi), puis autour de Oz (angle theta), puis autour de Ox (angle omega).
	 */
	struct vtkSimpleTransformMemento
	{
		public :
	
		bool	isExtrinsic;
		
		// Les éventuelles mises à l'échelle. Effectuées avant toute autre opération.
		double	scaleX, scaleY, scaleZ;

		// Paramétrage transformation extrinsèque/intrinsèque :
		double	xoy, xoz, yoz;	// theta, phi, omega
		double	dx, dy, dz;
		bool	translationFirst;

		// Pour l'application cliente :
		void*	userData;

		/**
		 * Instancie une transformation extrinsèque par défaut.
		 */
		vtkSimpleTransformMemento ( );

		/**
		 * Constructeur de copie. RAS.
		 */
		vtkSimpleTransformMemento (const vtkSimpleTransformMemento& m);

		/**
		 * Opérateur =. RAS.
		 */
		vtkSimpleTransformMemento& operator = (const vtkSimpleTransformMemento& m);

		/**
		 * Opérateur ==
		 * @return		true si les paramètres décrivent la même transformation. Dest attributs peuvent donc être différents si ils ne contribuent
		 *				pas à la description de la transformation.
		 */
		bool operator == (const vtkSimpleTransformMemento& m) const;
		bool operator != (const vtkSimpleTransformMemento& m) const;
		
		/**
		 * @return		true si les paramètres correspondent à une absence de transformation.
		 */
		bool IsIdentity ( ) const;
		
		/**
		 * Destructeur. RAS.
		 */
		 ~vtkSimpleTransformMemento ( );
	};	// struct vtkSimpleTransformMemento

	/**
	 * Créé une instance de la classe vtkTransform conformément aux paramètres transmis en arguments.
	 * La destruction de l'instance créée est à la charge de l'appelant.
	 */
	static vtkTransform* CreateTransform (const vtkSimpleTransformMemento& memento);


	private :

	vtkTransformHelper ( );

	vtkTransformHelper (const vtkTransformHelper&);

	vtkTransformHelper& operator = (const vtkTransformHelper&);

	~vtkTransformHelper ( );
};	// class vtkTransformHelper


#endif	// VTK_TRANSFORM_HELPER_H
