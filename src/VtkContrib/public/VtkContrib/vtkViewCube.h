/** @file    vtkViewCube.h
 *  @author  C. PIGNEROL, CEA/DAM/DSSI
 *  @date    04/03/2025
 */

#ifndef VTK_VIEWCUBE_H
#define VTK_VIEWCUBE_H

#include <vtkPropAssembly.h>
#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkCellPicker.h>
#include <vtkTransform.h>
#include <vtkRenderer.h>
#include <vtkVectorText.h>

//
// REM : source d'inspiration possible : classe vtkOrientationMarkerWidget couplée à vtkAnnotatedCubeActor
//

/**
 * Classe d'acteur représentant un "ViewCube", objet interactif de repositionnement de la vue en cliquant sur ses faces.
 * La vue graphique est repositionnée perpendiculairement à la face sur laquelle on clique.
 * Ce viewcube est composé de faces intermédiaires aux 6 faces d'un cube (=> 36 faces)
 * @class		vtkViewCube
 * @brief		Classe d'acteur représentant un trièdre.
 */

class vtkViewCube : public vtkPropAssembly
{
	public:

	vtkTypeMacro(vtkViewCube,vtkPropAssembly);


    /** Méthode de classe permettant de créer une instance de  la classe vtkViewCube (style VTK).
     *  @return un pointeur sur une instance de la classe VTK
	 */
    static vtkViewCube* New ( );

	/**
	 * Associe l'instance à un renderer avec la priorité transmise en argument.
	 */
	virtual void SetRenderers (vtkRenderer* renderer, vtkRenderer* drivenRenderer);
	virtual vtkRenderer* GetRenderer ( )
	{ return Renderer; }
	
	/**
	 * @return		Une éventuelle matrice de transformation utilisateur.
	 */
	virtual vtkTransform* GetTransform ( );

	/**
	 * @param		Une éventuelle matrice de transformation utilisateur.
	 */
	virtual void SetTransform (vtkTransform* transform);

	/**
	 * @param		Les libellés pour chacune des faces.
	 */
	virtual void SetFacesLabels (const char* front, const char* back, const char* left, const char* right, const char* top, const char* bottom);

	/**
	 * Affiche ou non les libellés des faces.
	 * @see		LabelOff
	 * @see		LabelOn
	 */
	virtual void SetLabel (bool on);

	/**
	 * Affiche les libellés dans le plan de la vue.
	 * @see		Label2DOff
	 * @see		SetLabel2D
	 */
	virtual void LabelOn ( );
	virtual void LabelOff ( );
	
	/**
	 * Regarde si une face du cube est pointée aux coordonnées transmises et, le cas échéant, adapte la vue perpendiculairement à cette face.
	 */
	virtual void PickCallback (int x, int y);


	protected:

    /** @brief  Constructeur d'une instance de la classe vtkViewCube.
     */
    vtkViewCube ( );

    /** @brief  Destructeur d'une instance de la classe vtkViewCube. RAS.
     */
    virtual ~vtkViewCube ( );


	private :

	/**
	 * Constructeur de copie et opérateur = : interdits.
	 */
	vtkViewCube (const vtkViewCube&);
	vtkViewCube& operator = (const vtkViewCube&);
	
	vtkRenderer			*Renderer, *DrivenRenderer;

	/** Le "cube". */
	vtkPolyData			*CubePolyData;
	vtkActor			*CubeActor;
	vtkPolyDataMapper	*CubePolyDataMapper;
	
	/** Le picking sur les faces du ViewCube. */
	vtkCellPicker		*CellPicker;

	/** Les labels. */
	vtkVectorText		*XPlusVectorText, *XMinusVectorText, *YPlusVectorText, *YMinusVectorText, *ZPlusVectorText, *ZMinusVectorText;
	vtkActor			*XPlusActor, *XMinusActor, *YPlusActor, *YMinusActor, *ZPlusActor, *ZMinusActor;

	/** L'éventuelle transformation utilisateur. */
	vtkTransform	*Transform;
};	// class vtkViewCube


#endif // VTK_VIEWCUBE_H

