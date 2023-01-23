/** @file    vtkECMAxesActor.h
 *  @author  F. LEDOUX, C. PIGNEROL
 *  @date    14/01/2005
 */

#ifndef VTK_ECM_AXES_ACTOR_H
#define VTK_ECM_AXES_ACTOR_H

#include <vtkPropAssembly.h>
#include <vtkActor.h>
#include <vtkLineSource.h>


/**
 * Classe d'acteur représentant un repère matérialisé sous forme de croix.
 * \author		C. Pignerol	CEA/DAM/DSSI
 * \date		09/10/2012
 */

class vtkECMAxesActor : public vtkPropAssembly
{
	public:

	vtkTypeMacro(vtkECMAxesActor,vtkPropAssembly);


    /** Méthode de classe permettant de créer une instance de 
	 * la classe vtkECMAxesActor (style VTK). La couleur des axes est lue
	 * dans les attributs statiques xColor, yColor, zColor.
     * \return		Un pointeur sur une instance de la classe VTK
	 */
    static vtkECMAxesActor* New ( );

	/**
	 * \param		Nouvelle position (xmin, xmax, ymin, ymax, zmin, zmax)
	 *				des axes.
	 * \see			SetPosition
	 */
	virtual void GetPosition (double& xmin, double& xmax, double& ymin,
	                          double& ymax, double& zmin, double& zmax);


	/**
	 * \param		Nouvelle position (xmin, xmax, ymin, ymax, zmin, zmax)
	 *				des axes.
	 * \see			GetPosition
	 */
	virtual void SetPosition (double xmin, double xmax, double ymin,
	                          double ymax, double zmin, double zmax);

	/**
	 * @param		En retour, composantes RGB pour l'axe des abscisses.
	 */
	virtual void GetAbscissaColor (double& r, double& g, double& b);

	/**
	 * @param		Composantes RGB pour l'axe des abscisses.
	 */
	virtual void SetAbscissaColor (double r, double g, double b);

	/**
	 * @param		En retour, composantes RGB pour l'axe des ordonnées.
	 */
	virtual void GetOrdinateColor (double& r, double& g, double& b);

	/**
	 * @param		Composantes RGB pour l'axe des ordonnées.
	 */
	virtual void SetOrdinateColor (double r, double g, double b);

	/**
	 * @param		En retour, composantes RGB pour l'axe des élévations.
	 */
	virtual void GetElevationColor (double& r, double& g, double& b);

	/**
	 * @param		Composantes RGB pour l'axe des élévations.
	 */
	virtual void SetElevationColor (double r, double g, double b);

	/**
	 * Couleur associée aux abscisses.
	 */
	static double	xColor [3];
 
	/**
	 * Couleur associée aux ordonnées.
	 */
	static double	yColor [3];
 
	/**
	 * Couleur associée aux élévations.
	 */
	static double	zColor [3];


	private:

    /**
	 * Constructeur d'une instance de la classe vtkECMAxesActor.
     */
    vtkECMAxesActor ( );

    /**
	 * Destructeur d'une instance de la classe vtkECMAxesActor. RAS.
     */
    virtual ~vtkECMAxesActor ( );

	/**
	 * Constructeur de copie et opérateur = : interdits.
	 */
	vtkECMAxesActor (const vtkECMAxesActor&);
	vtkECMAxesActor& operator = (const vtkECMAxesActor&);

	/** Le acteurs de chacun des axes.  */
	vtkActor			*_xAxisActor, *_yAxisActor, *_zAxisActor;

	/** Les définitions des axes. */
	vtkLineSource	*_xAxis, *_yAxis, *_zAxis;
};	// class vtkECMAxesActor


#endif // VTK_ECM_AXES_ACTOR_H

