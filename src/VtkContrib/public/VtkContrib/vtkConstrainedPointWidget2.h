/*
 * Widget de saisie d'un point dans l'espace.
 * @author		Charles PIGNEROL, CEA/DAM/DSSI
 * @date		24/06/2024
 */

#ifndef VTK_CONSTRAINED_POINT_WIDGET_2_H
#define VTK_CONSTRAINED_POINT_WIDGET_2_H

#include <vtkHandleWidget.h>
#include <vtkSphereHandleRepresentation.h>

class vtkConstrainedPointHandleRepresentation2;


/**
 * <P>Un widget pour saisir interactivement un point dont déplacement peut être contraint le long d'un axe euclidien.</P>
 * <P>Par défaut le point, matérialisé par une sphère, est déplacé dans le plan de l'écran, mais on peut contraindre le déplacement 
 * le long de l'axe des abscisses, ordonnées ou élévations en pressant respectivement et simultanément la touche 'x', 'y' ou 'z'
 * (insensibilité à la casse). Une pression sur la touche 'espace' provoque un retour au déplacement dans le plan.
  *
 * <P>tests/point_widget2.cpp est un exemple d'utilisation avec callback permettant de connaître à tout moment la nouvelle position du point.</P>
 */
class vtkConstrainedPointWidget2 : public vtkHandleWidget
{
	public :

	vtkTypeMacro(vtkConstrainedPointWidget2,vtkHandleWidget)


	/**
	 * Créé une représentation de type vtkConstrainedPointHandleRepresentation2.
	 */
	virtual void CreateDefaultRepresentation ( );

	/**
	 * Instanciation de la classe.
	 */
	static vtkConstrainedPointWidget2* New ( );

	/**
	 * @return	La représentation de l'instance.
	 */
	vtkConstrainedPointHandleRepresentation2* GetConstrainedPointRepresentation ( );
	
	/**
	 * Modifie l'axe de contrainte de déplacement.
	 */
	virtual void OnChar ( );
 

	private :

	/**
	 * Constructeur. S'associe une instance de vtkConstrainedPointHandleRepresentation2 pour représenter le point et effectuer son positionnement.
	 */
	vtkConstrainedPointWidget2 ( );

	/**
	 * Destructeur. RAS.
	 */
	virtual ~vtkConstrainedPointWidget2 ( );

	/**
	 * Constructeur et opérateur = : interdits.
	 */
	vtkConstrainedPointWidget2 (const vtkConstrainedPointWidget2&);
	vtkConstrainedPointWidget2& operator = (const vtkConstrainedPointWidget2&);
};	// class vtkConstrainedPointWidget2


/**
 * <P>Classe de représentation/positionnement d'un point sur une grille non structurée.</P>
 * <P>Sa méthode (héritée) <I>GetWorldPosition (double [3])</I> permet de récupére les coordonnées courantes du point saisi.</P>
 */
class vtkConstrainedPointHandleRepresentation2 : public vtkSphereHandleRepresentation
{
	public :

	vtkTypeMacro(vtkConstrainedPointHandleRepresentation2,vtkSphereHandleRepresentation)


	public :

	/**
	 * Instanciation de la classe.
	 */
	static vtkConstrainedPointHandleRepresentation2* New ( );

	/**
	 * Versions simplifiées de la gestion interactive de la saisie.
	 */
	virtual void WidgetInteraction (double eventPos [2]);

	/**
	 * @param	Nouvel axe de contrainte de déplacement (0 -> X, 1 -> Y, 2 -> Z, autre => déplacement dans le plan de l'écran).
	 */
	virtual void SetConstraintAxis (int axis);


	private :

	/**
	 * Constructeur : RAS.
	 */
	vtkConstrainedPointHandleRepresentation2 ( );

	/**
	 * Destructeur. RAS.
	 */
	virtual ~vtkConstrainedPointHandleRepresentation2 ( );

	/**
	 * Constructeur de copie, opérateur = : interdits.
	 */
	vtkConstrainedPointHandleRepresentation2 (const vtkConstrainedPointHandleRepresentation2&);
	vtkConstrainedPointHandleRepresentation2& operator = (const vtkConstrainedPointHandleRepresentation2&);
};	// class vtkConstrainedPointHandleRepresentation2


#endif	// VTK_CONSTRAINED_POINT_WIDGET_2_H
