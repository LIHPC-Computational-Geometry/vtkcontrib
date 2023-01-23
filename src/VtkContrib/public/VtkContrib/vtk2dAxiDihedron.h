/**
 * @file    vtk2dAxiDihedron.h
 *  @author  C. PIGNEROL
 *  @date    18/09/2019
 */

#ifndef VTK_5

#ifndef VTK_2DAXI_DIHEDRON_H
#define VTK_2DAXI_DIHEDRON_H

#include "VtkContrib/vtkTrihedron.h"

#include <vtkPolyLineSource.h>
#include <vtkConeSource.h>

/**
 * <P>Classe d'acteur représentant un dièdre pour scènes graphiques de type 2D
 * axisymétriques. 2 axes représentent le plan et une flèche autour de l'axe de
 * rotation (1er axe) représente la symétrie axiale.</P>
 * <P>Détails d'implémentation :
 * <UL>
 * <LI>L'axe des abscisses devient Z, l'axe des ordonnées devient X, et l'axe
 * des élévations celui de la symétrie axiale Y,
 * <LI>La flèche accessible via <I>GetZAxisArrowSource</I> est désactivée (mais
 * non détruite) et remplacée par une ligne brisée + cône,
 * </UL>
 * </P>
 * @class	vtk2dAxiDihedron
 * @author	C. PIGNEROL  CEA/DAM/DSSI
 * @see		vtkTrihedron
 * @see		vtkTrihedronCommand
 */
class vtk2dAxiDihedron : public vtkTrihedron
{
	public :

	vtkTypeMacro(vtk2dAxiDihedron,vtkTrihedron);

	/**
	  * Instanciation de la classe.
	  */
	static vtk2dAxiDihedron* New ( );

	/**
	 * @param		Une éventuelle matrice de transformation utilisateur.
	 */
	virtual void SetTransform (vtkTransform* transform);

	/**
	 * @param		Redimensionne l'épaisseur des axes du facteur transmis en
	 * 				argument relativement aux valeurs par défaut.
	 */
	virtual void SetAxisThicknessScale (double scale);

	/**
	 * @param		Composantes RGB pour l'axe des abscisses.
	 */
	virtual void SetElevationColor (double r, double g, double b);


	private :

	/**
	 * Constructeur. Transforme le 3ème axe en flèche autour de l'axe de
	 * rotation.
	 */	
	vtk2dAxiDihedron ( );

	/**
	 * Destructeur. RAS.
	 */
	virtual ~vtk2dAxiDihedron ( );

	/**
	 * Constructeur de copie et opérateur = : interdits.
	 */
	vtk2dAxiDihedron (const vtk2dAxiDihedron&);
	vtk2dAxiDihedron& operator = (const vtk2dAxiDihedron&);
	vtkActor*			m_zShaftActor;
	vtkActor*			m_zTipActor;
	vtkPolyLineSource*	m_zShaftCurveSource;
	vtkConeSource*		m_zTipCurveSource;
};	// class vtk2dAxiDihedron

#endif	// VTK_5

#endif	// VTK_2DAXI_DIHEDRON_H
