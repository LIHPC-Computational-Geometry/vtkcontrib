/*
 * Widget de saisie d'un point situé sur une grille non structurée.
 * @author		Charles PIGNEROL, CEA/DAM/DSSI
 * @date		18/05/2017
 */

#ifndef VTK_CONSTRAINED_POINT_WIDGET_H
#define VTK_CONSTRAINED_POINT_WIDGET_H

#include <vtkHandleWidget.h>
#include <vtkPointHandleRepresentation3D.h>
#include <vtkPolyDataPointPlacer.h>
#include <vtkUnstructuredGrid.h>


class vtkConstrainedPointHandleRepresentation;
class vtkUnstructuredGridPointPlacer;


/**
 * <P>Un widget pour saisir interactivement un point dont la position est
 * contrainte sur une grille non structurée.</P>
 * <P>On transmet la grille de contrainte via :<BR>
 * GetConstrainedPointRepresentation ( )->GetUGridPointPlacer (
 * 														)->SetGrid (grid);
 * </P>
 *
 * <P>tests/point_widget.cpp est un exemple d'utilisation avec callback 
 * permettant de connaître à tout moment la nouvelle position du point.</P>
 */
class vtkConstrainedPointWidget : public vtkHandleWidget
{
	public :

	vtkTypeMacro(vtkConstrainedPointWidget,vtkHandleWidget)


	/**
	 * Créé une représentation de type vtkConstrainedPointHandleRepresentation.
	 */
	virtual void CreateDefaultRepresentation ( );

	/**
	 * Instanciation de la classe.
	 */
	static vtkConstrainedPointWidget* New ( );

	/**
	 * @return	La représentation de l'instance.
	 */
	vtkConstrainedPointHandleRepresentation*
								GetConstrainedPointRepresentation ( );
 

	private :

	/**
	 * Constructeur. S'associe une instance de
	 * vtkConstrainedPointHandleRepresentation pour représenter le point et
	 * effectuer son positionnement.
	 */
	vtkConstrainedPointWidget ( );

	/**
	 * Destructeur. RAS.
	 */
	virtual ~vtkConstrainedPointWidget ( );

	/**
	 * Constructeur et opérateur = : interdits.
	 */
	vtkConstrainedPointWidget (const vtkConstrainedPointWidget&);
	vtkConstrainedPointWidget& operator = (const vtkConstrainedPointWidget&);
};	// class vtkConstrainedPointWidget


/**
 * <P>Classe de représentation/positionnement d'un point sur une grille non
 * structurée.</P>
 * <P>Sa méthode (héritée) <I>GetWorldPosition (double [3])</I> permet de
 * récupére les coordonnées courantes du point saisi.</P>
 */
class vtkConstrainedPointHandleRepresentation :
										public vtkPointHandleRepresentation3D
{
	public :

	vtkTypeMacro(vtkConstrainedPointHandleRepresentation,vtkPointHandleRepresentation3D)


	public :

	/**
	 * Instanciation de la classe.
	 */
	static vtkConstrainedPointHandleRepresentation* New ( );

	/**
	 * Versions simplifiées de la gestion interactive de la saisie.
	 */
	virtual int ComputeInteractionState (int X, int Y, int notUsed);
	virtual void WidgetInteraction (double eventPos [2]);
	virtual vtkUnstructuredGridPointPlacer* GetUGridPointPlacer ( );


	private :

	/**
	 * Constructeur : s'associe ue instance de la classe 
	 * vtkUnstructuredGridPointPlacer pour déterminer la position du point à
	 * saisir.
	 */
	vtkConstrainedPointHandleRepresentation ( );

	/**
	 * Destructeur. RAS.
	 */
	virtual ~vtkConstrainedPointHandleRepresentation ( );

	/**
	 * Constructeur de copie, opérateur = : interdits.
	 */
	vtkConstrainedPointHandleRepresentation (
							const vtkConstrainedPointHandleRepresentation&);
	vtkConstrainedPointHandleRepresentation& operator = (
							const vtkConstrainedPointHandleRepresentation&);
};	// class vtkConstrainedPointHandleRepresentation


/**
 * Classe de positionnement d'un point sur une grille non structurée.
 */
class vtkUnstructuredGridPointPlacer : public vtkPolyDataPointPlacer
{
	public :

	vtkTypeMacro(vtkUnstructuredGridPointPlacer,vtkPolyDataPointPlacer)

	/**
	 * Instanciation de la classe.
	 */
	static vtkUnstructuredGridPointPlacer* New ( );

	/**
	 * Recherche le point le plus proche sur la grille non structurée de
	 * celui transmis en argument. La distance évaluée est celle 2D, à savoir
	 * sur l'écran. En retour les coordonnées 3D du point le plus proche.
	 */
	virtual int ComputeWorldPosition (
				vtkRenderer* ren,  double displayPos [2], double worldPos[3],
				 double notUsed [9]);

	/**
	 * @param	La grille non structurée évaluée.
	 */
	virtual void SetGrid (vtkUnstructuredGrid* grid);


	private :

	/**
	 * Constructeur et destructeur. RAS.
	 */
	vtkUnstructuredGridPointPlacer ( );
	virtual ~vtkUnstructuredGridPointPlacer ( );

	/**
	 * Constructeur de copie, opérateur = : interdits.
	 */
	vtkUnstructuredGridPointPlacer (const vtkUnstructuredGridPointPlacer&);
	vtkUnstructuredGridPointPlacer& operator = (
										const vtkUnstructuredGridPointPlacer&);

	/** La grille évaluée. */
	vtkUnstructuredGrid*					Grid;
};	// class vtkUnstructuredGridPointPlacer


#endif	// VTK_CONSTRAINED_POINT_WIDGET_H
