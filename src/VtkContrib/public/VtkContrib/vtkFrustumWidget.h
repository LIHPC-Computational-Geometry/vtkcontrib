/**
 * Widget de saisie d'un tronc dans l'espace.
 * @author		Charles PIGNEROL, CEA/DAM/DCLC
 * @date		03/10/2025
 */

#ifndef VTK_FRUSTUM_WIDGET_H
#define VTK_FRUSTUM_WIDGET_H

#include "VtkContrib/vtkPolygonFilter.h"

#include <vtkActor.h>
#include <vtk3DWidget.h>
#include <vtkActor.h>
#include <vtkCommand.h>
#include <vtkImplicitPlaneWidget.h>
#include <vtkPolyDataMapper.h>
#include <vtkFrustumSource.h>
#include <vtkPlane.h>
#include <vtkPlanes.h>
#include <vtkTubeFilter.h>

#include <map>
#include <vector>

class vtkPlaneWidgetCallbackCommand;

/**
 * <P>Un widget pour saisir interactivement un tronc. Un tronc est ici défini comme étant la zone intérieure à 6 plans qui en définissent la bordure.
 * Les plans sont numérotés de 0 à 6 comme suit : gauche, droit, haut, bas, derrière, devant.</P>
 *
 * <P>tests/frustum_widget.cpp est un exemple d'utilisation avec callback permettant de connaître le tronc défini.</P>
 */
class vtkFrustumWidget : public vtk3DWidget
{
	friend class vtkPlaneWidgetCallbackCommand;
	
	public :

	vtkTypeMacro(vtkFrustumWidget,vtk3DWidget)

	/**
	 * Pour l'accès au paramétrage des 6 plans du tronc (ordre de la classe vtkFrustumSource).
	 */
	enum SIDE { LEFT, RIGHT, BOTTOM, TOP, BACK, FRONT };

	/**
	 * Créé une représentation de type vtkConstrainedPointHandleRepresentation2.
	 */
	virtual void CreateDefaultRepresentation ( );

	/**
	 * Instanciation de la classe.
	 */
	static vtkFrustumWidget* New ( );

	//@{
	/**
	 * Methods that satisfy the superclass' API.
	 */
	virtual void SetInteractor (vtkRenderWindowInteractor* iren);
	virtual void SetEnabled (int);
	virtual void PlaceWidget(double bounds [6]);
	virtual void SetPlaceFactor (double factor);
	//@} 

	//@{
	/**
	 * Définition du plan.
	 */

	/**
	 * @param	indice du plan modifié ou consulté.
	 * @param	nouvelle origine.
	 * @param	nouvelle normale.
	 */
	virtual void SetPlane (SIDE p, double origin [3], double normal [3]);
	virtual void GetPlane (SIDE p, double origin [3], double normal [3]);
	virtual void SetPlaneOrigin (SIDE p, double origin [3]);
	virtual void GetPlaneOrigin (SIDE p, double origin [3]);
	virtual void SetPlaneNormal (SIDE p, double normal [3]);
	virtual void GetPlaneNormal (SIDE p, double normal [3]);

	//@{
	/**
	 * Modifications des propriété du widget.
	 */

	// @param Opacité du tronc (6 plans) affiché.
	virtual void SetOpacity (double opacity);
	
	/**
	 * @param	Composantes RGB des 3 interacteurs axiaux (gauche/droite, bas/haut, arrière/avant).
	 * @warning	SetInteractorEnabled doit être invoqué après On ( ) pour être effectif.
	 */
	virtual void SetInteractorColor (SIDE axis, double rgb [3]);
	virtual void SetInteractorEnabled (SIDE axis, bool enabled);
	//@}

	/**
	 * @return	La boite englobante du tronc.
	 */
	virtual void GetBounds (double bounds [6]);

	/**
	 * @return	La définition du tronc.
	 */
	virtual const vtkPlanes& GetPlanes ( ) const;
	virtual vtkPlanes& GetPlanes ( );

	/**
	 * Modification des 6 plans.
	 */
	virtual void SetPlanes (vtkPlanes& planes);

	/**
	 * Ajoute le couple de plans p1 et p2 de la liste des paires de plan devant rester parallèles.
	 * Dans sa forme à 1 argument le second plan est celui implicitement apairé (gauche/droite, bas/haut, arrière/avant).
	 */
	virtual void AddParallelPlanes (SIDE p1, SIDE p2);
	virtual void AddParallelPlanes (SIDE p);

	/**
	 * Enlève le couple de plans p1 et p2 de la liste des paires de plan devant rester parallèles.
	 */
	virtual void RemoveParallelPlanes (SIDE p1, SIDE p2);
	virtual void RemoveParallelPlanes (SIDE p);
	
	/**
	 * @return	Les couples de plans restant parallèles.
	 */
	virtual std::map<int, int> GetParallelPlanes ( ) const;
	

	protected :

	/**
	 * Classe permettant de contourner des difficultés rencontrées.
	 * Un objectif est également de ne pas grossir les interacteurs car par défaut ils prennent beaucoup de place
	 * et il en faut 6 pour le tronc.
	 */
	class vtkInternalPlaneWidget : public vtkImplicitPlaneWidget
	{
		public :
		
		static vtkInternalPlaneWidget* New ( );

		/**
		 * Ne grossissent pas les interacteurs.
		 */
		virtual void PlaceWidget (double bounds [6]);
		virtual void SizeHandles ( );

		/**
		 * Contrairement à vtkImpliciPlane::SetOrigin (double [3]), cette méthode permet de modifier l'origine du
		 * plan sans la restreindre à sa boite englobante courante => Une nouvelle boite englobante est éventuellement
		 * définie en sortie de cet appel.
		 * @since	5.13.0
		 */
		virtual void SetUnboundedOrigin (double origin [3]);

		/**
		 * Nouvelle position => actualise la représentation sans recalculer la position qui est transmise en argument.
		 */
		virtual void UpdateRepresentation (double origin [3]);
		
		/**
		 * @return	Le rayon de la poignée.
		 */
		virtual double GetHandleRadius ( );
		
		/**
		 * Affecte la couleur transmise en argument à l'interacteur.
		 */
		virtual void SetInteractorColor (double rgb [3]);

		/**
		 * (Dés)active l'interacteur tout en le laissant visible.
		 */
		virtual void SetInteractorEnabled (bool enabled);


		protected :
		
		vtkInternalPlaneWidget ( );
		virtual ~vtkInternalPlaneWidget ( );


		private :

		vtkInternalPlaneWidget (const vtkInternalPlaneWidget&);
		vtkInternalPlaneWidget& operator = (const vtkInternalPlaneWidget&);

		/// true si en cours de placement du widget.
		bool		_placing;
	};	// class vtkInternalPlaneWidget
	
	/**
	 * @return	le p-ième plan du tronc
	 */
	virtual vtkInternalPlaneWidget* GetPlane (SIDE p);
	virtual const vtkInternalPlaneWidget* GetPlane (SIDE p) const;

	/**
	 * Actualisation lors de la modification d'un plan.
	 */
	virtual void Update (vtkInternalPlaneWidget* caller);


	private :

	/**
	 * Constructeur.
	 */
	vtkFrustumWidget ( );

	/**
	 * Destructeur. RAS.
	 */
	virtual ~vtkFrustumWidget ( );

	/**
	 * Constructeur et opérateur = : interdits.
	 */
	vtkFrustumWidget (const vtkFrustumWidget&);
	vtkFrustumWidget& operator = (const vtkFrustumWidget&);
	
	/** Le tronc et sa représentation. */
	vtkSmartPointer<vtkFrustumSource>						_frustum;
	vtkSmartPointer<vtkPolygonFilter>						_polygonFilter;
	vtkSmartPointer<vtkPlanes>								_planes;
	vtkSmartPointer<vtkPolyDataMapper>						_frustumMapper;
	vtkSmartPointer<vtkActor>								_frustumActor;
	/** Le contour du tronc et sa représentation. */
	vtkSmartPointer<vtkTubeFilter>							_contourFilter;
	vtkSmartPointer<vtkPolyDataMapper>						_contourMapper;
	vtkSmartPointer<vtkActor>								_contourActor;
	/** Les widgets interactifs. */
	std::vector<vtkSmartPointer<vtkInternalPlaneWidget>>	_planeWidgets;
	vtkSmartPointer<vtkPlaneWidgetCallbackCommand>			_planeCallback;
	/** Pour des opérations temporaires de récupération de données. */
	vtkSmartPointer<vtkPlane>								_tmpPlane;
	
	/* Les paires de plans parallèles entre-eux. */
	std::map<int, int>										_parallelPlanes;
};	// class vtkFrustumWidget


class vtkPlaneWidgetCallbackCommand : public vtkCommand
{
	public :
	
	static vtkPlaneWidgetCallbackCommand* New ( )
	{ return new vtkPlaneWidgetCallbackCommand ( ); }

	virtual ~vtkPlaneWidgetCallbackCommand ( )
	{ }
	
	virtual void Execute (vtkObject* caller, unsigned long id, void* calldata);

	virtual void SetFrustumWidget (vtkFrustumWidget* widget)
	{
		_frustumWidget	= widget; 
	}	// SetFrustumWidget

	protected :
	
	vtkPlaneWidgetCallbackCommand ( )
		: vtkCommand ( ), _frustumWidget (0)
	{ }


	private :
	
	vtkPlaneWidgetCallbackCommand (const vtkPlaneWidgetCallbackCommand&)
	{ assert (0 && "vtkPlaneWidgetCallbackCommand copy constructor is not allowed."); }
	vtkPlaneWidgetCallbackCommand& operator = (const vtkPlaneWidgetCallbackCommand&)
	{
		assert (0 && "vtkPlaneWidgetCallbackCommand assignment operator is not allowed.");
		return *this;
	}
	
	vtkFrustumWidget*	_frustumWidget;
};	// class vtkPlaneWidgetCallbackCommand


#endif	// VTK_FRUSTUM_WIDGET_H
