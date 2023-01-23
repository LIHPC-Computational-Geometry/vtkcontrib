/** @file    vtkTrihedron.h
 *  @author  F. LEDOUX, C. PIGNEROL
 *  @date    14/01/2005
 */

#ifndef VTK_TRIHEDRON_H
#define VTK_TRIHEDRON_H

#include <vtkPropAssembly.h>
#include <vtkActor.h>
#include <vtkActor2D.h>
#include <vtkArrowSource.h>
#include <vtkMatrix4x4.h>
#include <vtkTextMapper.h>
#include <vtkVectorText.h>
#include <vtkTransform.h>


/**
 * Classe d'acteur représentant un trièdre. La mise à jour du 
 * trièdre peut être assurée par une instance de la classe 
 * vtkTrihedronCommand.
 * @class		vtkTrihedron
 * @brief		Classe d'acteur représentant un trièdre.
 * @author		F. Ledoux, C. Pignerol	CEA/DAM/DSSI
 * @see			vtkTrihedronCommand
 */

class vtkTrihedron : public vtkPropAssembly
{
	public:

	vtkTypeMacro(vtkTrihedron,vtkPropAssembly);


    /** Méthode de classe permettant de créer une instance de 
	 * la classe vtkTrihedron (style VTK). La couleur des axes est lue
	 * dans les attributs statiques xColor, yColor, zColor.
     *  @return un pointeur sur une instance de la classe VTK
	 */
    static vtkTrihedron* New ( );

	/**
	 * @return		Une éventuelle matrice de transformation utilisateur.
	 */
	virtual vtkTransform* GetTransform ( );

	/**
	 * @param		Une éventuelle matrice de transformation utilisateur.
	 */
	virtual void SetTransform (vtkTransform* transform);

	/**
	 * @param		Les libellés pour chacun des axes.
	 */
	virtual void SetAxisLabels (const char* x, const char* y, const char* z);

	/**
	 * @param		Les offsets par rapport au défaut pour l'affichage des
	 *				libellés. Utilise un alignement par le bas.
	 * @warning		En mode <I>2D on</I>, offsets à fournir en pixels.
	 * 				En mode <I>2D off</I>, les positions sont relatives, chaque
	 * 				axe mesurant 1.1, donc l'offset est à comparer à 1.
	 */
	virtual void SetLabelsOffsets (
							double xOffset, double yOffset, double zOffset);

	/**
	 * @param		Redimensionne l'épaisseur des axes du facteur transmis en
	 * 				argument relativement aux valeurs par défaut.
	 */
	virtual void SetAxisThicknessScale (double scale);

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
	 * Affiche les libellés dans le plan de la vue <I>true</I> ou dans
	 * l'espace 3D <I>false</I>..
	 * @see		Label2DOff
	 * @see		Label2DOn
	 */
	virtual void SetLabel2D (bool on);

	/**
	 * Affiche les libellés dans le plan de la vue.
	 * @see		Label2DOff
	 * @see		SetLabel2D
	 */
	virtual void Label2DOn ( );

	/**
	 * Affiche les libellés dans l'espace 3D.
	 * @see		Label2DOn
	 * @see		SetLabel2D
	 */
	virtual void Label2DOff ( );

	/**
	 * @return	Les éléments vtk représentant les flèches des axes.
	 */
	vtkArrowSource& GetXAxisArrowSource ( );
	vtkArrowSource& GetYAxisArrowSource ( );
	vtkArrowSource& GetZAxisArrowSource ( );

	/**
	 * @return	Les acteurs utilisés pour représenter les flèches des axes.
	 */
	vtkActor& GetXAxisArrowActor ( );
	vtkActor& GetYAxisArrowActor ( );
	vtkActor& GetZAxisArrowActor ( );

	/**
	 * @return	Les acteurs utilisés pour représenter les libellés des axes.
	 * @warning	Le pointeur peut être nul.
	 * @see		Label2DOn
	 */
	vtkActor* GetXAxisLabelActor ( );
	vtkActor* GetYAxisLabelActor ( );
	vtkActor* GetZAxisLabelActor ( );

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


	protected:

    /** @brief  Constructeur d'une instance de la classe vtkTrihedron.
     */
    vtkTrihedron ( );

    /** @brief  Destructeur d'une instance de la classe vtkTrihedron. RAS.
     */
    virtual ~vtkTrihedron ( );


	private :

	/**
	 * Constructeur de copie et opérateur = : interdits.
	 */
	vtkTrihedron (const vtkTrihedron&);
	vtkTrihedron& operator = (const vtkTrihedron&);

	/** Les flèches sources. */
	vtkArrowSource	*m_xAxisArrowSource, *m_yAxisArrowSource,
					*m_zAxisArrowSource;

	/** Les libellés des axes. Par défaut "x", "y", "z". */
	vtkVectorText	*m_xVectorText, *m_yVectorText, *m_zVectorText;

	/** Le acteurs de chacun des axes.  */
	vtkActor     *m_xAxisActor, *m_yAxisActor, *m_zAxisActor;
   
	/** Les acteurs pour l'affichage 3D des titres des axes. */
	vtkActor	*m_xLabelActor, *m_yLabelActor, *m_zLabelActor;

	/** Les acteurs pour l'affichage 2D des titres des axes. */
	vtkActor2D	*m_xLabelActor2D, *m_yLabelActor2D, *m_zLabelActor2D;

	/** Les mappers pour l'affichage 2D des titres des axes. */
	vtkTextMapper	*m_xTextMapper, *m_yTextMapper, *m_zTextMapper;

	/** L'éventuelle transformation utilisateur. */
	vtkTransform	*m_transform;

	/** Rayons/longueur par défaut du trait et de la flèche. */
	double			m_shaftRadius, m_tipRadius, m_tipLength;
};	// class vtkTrihedron


#endif // VTK_TRIHEDRON_H

