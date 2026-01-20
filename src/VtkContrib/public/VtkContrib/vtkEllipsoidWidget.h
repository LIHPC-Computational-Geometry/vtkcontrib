/**
 * Widget de saisie d'une ellipsoïde dans l'espace.
 * @author		Charles PIGNEROL, CEA/DAM/DCLC
 * @date		12/12/2025
 */

#ifndef VTK_ELLIPSOID_WIDGET_H
#define VTK_ELLIPSOID_WIDGET_H

#include "VtkContrib/vtkEllipsoid.h"
#include "VtkContrib/vtkPolygonFilter.h"

#include <vtkBoxWidget.h>
#include <vtkCommand.h>
#include <vtkPolyDataMapper.h>
#include <vtkParametricEllipsoid.h>
#include <vtkParametricFunctionSource.h>
#include <vtkTransform.h>


/**
 * <P>Un widget pour saisir interactivement une ellipsoïde. L'ellipsoïde est ici délimitée par 6 plans qui en définissent la bordure.
 * Les plans sont numérotés de 0 à 6 comme suit : gauche, droit, haut, bas, derrière, devant. L'ellipsoïde est tangente en ces 6 plans.</P>
 * 
 * <P>Pour mémo, la transformation portée par la classe vtkBoxWidget décrit la transformation effectuée à partir de PlaceWidget,
 * ce qui implique que la position initiale n'est pas décrite dans cette transformation.
 * Concernant l'ellipsoid, décrite de manière paramétrique (classe vtkParametricEllipsoid), elle est centrée en 0. Cette ellipsoid est
 * représentée par un acteur véhiculant une transformation analogue à celle de vtkBoxWidget mais portant en plus la position initiale.
 * </P>
 *
 * <P>tests/ellipsoid_widget.cpp est un exemple d'utilisation avec callback permettant de connaître l'ellipsoïde définie.</P>
 * @see			vtkEllipsoid
 */
class vtkEllipsoidWidget : public vtkBoxWidget
{
	public :

	vtkTypeMacro(vtkEllipsoidWidget,vtkBoxWidget)

	/**
	 * Créé une représentation de type vtkConstrainedPointHandleRepresentation2.
	 */
	virtual void CreateDefaultRepresentation ( );

	/**
	 * Instanciation de la classe.
	 */
	static vtkEllipsoidWidget* New ( );

	//@{
	/**
	 * Methods that satisfy the superclass' API.
	 */
	virtual void PlaceWidget (double bounds [6]);
	virtual void PlaceWidget (double xRadius, double yRadius, double zRadius, vtkTransform* transform);
	virtual void SetEnabled (int);
	//@}

	//@{
	/**
	 * Modifications des propriété du widget.
	 */

	/**
	 * Les dimensions de l'ellipsoïde selon les 3 axes (avant transformation).
	 */
	 virtual void GetPosition (double& x, double& y, double& z);

	/**
	 * Les dimensions de l'ellipsoïde selon les 3 axes.
	 */
	 virtual double GetXRadius ( );
	 virtual double GetYRadius ( );
	 virtual double GetZRadius ( );
	 
	/**
	 * La discrétisation de l'ellipsoïde autour des 3 axes (avant transformation).
	 */
	virtual void GetDiscretisation (unsigned int& u, unsigned int& v, unsigned int& w);
	virtual void SetDiscretisation (unsigned int u, unsigned int v, unsigned int w);

	/**
	 * @param	Opacité de l'ellipsoïde affichée.
	 */
	virtual void SetEllipsoidOpacity (double opacity);
	
	/**
	 * @param	Composantes RGB des l'ellipsoïde.
	 */
	virtual void SetEllipsoidColor (double rgb [3]);
	virtual void SetEllipsoidColor (double r, double g, double b);
	//@}


	protected :

	/**
	 * Appelé lorsque la box widget est modifiée : actualise l'ellipsoïde.
	 */
	class vtkBoxInteractionCommand : public vtkCommand
	{
		public :
		
		vtkBoxInteractionCommand ( )
			: _widget (0)
		{ }
		virtual ~vtkBoxInteractionCommand ( )
		{ _widget = 0; }
		static vtkBoxInteractionCommand* New ( );

		virtual void SetWidget (vtkEllipsoidWidget* widget)
		{ _widget	= widget; }
		virtual void Execute (vtkObject* caller, unsigned long id, void* calldata);
		
		
		private :
		
		vtkBoxInteractionCommand (const vtkBoxInteractionCommand&)
			: _widget (0)
		{ }
		vtkBoxInteractionCommand& operator = (const vtkBoxInteractionCommand&)
		{ return *this; }
		
		vtkEllipsoidWidget*		_widget;
	};	// class vtkBoxInteractionCommand

	/**
	 * Actualise la représentation de l'ellipsoïde suite à une modification de l'interacteur.
	 */
	virtual void UpdateEllipsoid ( );


	private :

	/**
	 * Constructeur.
	 */
	vtkEllipsoidWidget ( );

	/**
	 * Destructeur. RAS.
	 */
	virtual ~vtkEllipsoidWidget ( );

	/**
	 * Constructeur et opérateur = : interdits.
	 */
	vtkEllipsoidWidget (const vtkEllipsoidWidget&);
	vtkEllipsoidWidget& operator = (const vtkEllipsoidWidget&);
	
	/** L'ellipsoïde et sa représentation. */
	vtkSmartPointer<vtkParametricEllipsoid>					_ellipsoid;
	vtkSmartPointer<vtkParametricFunctionSource>			_ellipsoidSource;
	vtkSmartPointer<vtkPolygonFilter>						_polygonFilter;
	vtkSmartPointer<vtkPolyDataMapper>						_ellipsoidMapper;
	vtkSmartPointer<vtkActor>								_ellipsoidActor;
	vtkSmartPointer<vtkBoxInteractionCommand>				_interactionCallback;
	vtkSmartPointer<vtkTransform>							_transform;
};	// class vtkEllipsoidWidget


#endif	// VTK_ELLIPSOID_WIDGET_H
