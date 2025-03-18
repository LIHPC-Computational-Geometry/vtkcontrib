/** @file    vtkViewCubeActor.h
 *  @author  C. PIGNEROL, CEA/DAM/DSSI
 *  @date    04/03/2025
 */

#ifndef VTK_VIEWCUBE_ACTOR_H
#define VTK_VIEWCUBE_ACTOR_H

#include <vtkPropAssembly.h>
#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkCellPicker.h>
#include <vtkTransform.h>
#include <vtkRenderer.h>
#include <vtkVectorText.h>
#include <vtkSmartPointer.h>

//
// REM : source d'inspiration possible : classe vtkOrientationMarkerWidget couplée à vtkAnnotatedCubeActor
//

/**
 * Classe d'acteur représentant un "ViewCube", objet interactif de repositionnement de la vue en cliquant sur ses faces.
 * La vue graphique est repositionnée perpendiculairement à la face sur laquelle on clique.
 * Ce viewcube est composé de faces intermédiaires aux 6 faces d'un cube (=> 26 faces)
 * @class		vtkViewCubeActor
 * @brief		Classe d'acteur représentant un trièdre.
 */

class vtkViewCubeActor : public vtkPropAssembly
{
	public:

	vtkTypeMacro(vtkViewCubeActor,vtkPropAssembly);


    /** Méthode de classe permettant de créer une instance de  la classe vtkViewCubeActor (style VTK).
     *  @return un pointeur sur une instance de la classe VTK
	 */
    static vtkViewCubeActor* New ( );

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
	 * Modifie l'opacité des faces conformément à la valeur transmise en argument.
	 */
	virtual void SetFacesOpacity (double opacity);

	/**
	 * Regarde si une face du cube est pointée aux coordonnées transmises et, le cas échéant, adapte la vue perpendiculairement à cette face.
	 */
	virtual void PickCallback (int x, int y);


	protected:

    /** @brief  Constructeur d'une instance de la classe vtkViewCubeActor.
     */
    vtkViewCubeActor ( );

    /** @brief  Destructeur d'une instance de la classe vtkViewCubeActor. RAS.
     */
    virtual ~vtkViewCubeActor ( );


	private :

	/**
	 * Constructeur de copie et opérateur = : interdits.
	 */
	vtkViewCubeActor (const vtkViewCubeActor&);
	vtkViewCubeActor& operator = (const vtkViewCubeActor&);
	
	vtkRenderer			*Renderer, *DrivenRenderer;

	/** Le "cube". */
	vtkSmartPointer<vtkPolyData>		CubePolyData;
	vtkSmartPointer<vtkActor>			CubeActor;
	vtkSmartPointer<vtkPolyDataMapper>	CubePolyDataMapper;
	
	/** Le picking sur les faces du ViewCube. */
	vtkSmartPointer<vtkCellPicker>		CellPicker;

	/** Les labels. */
	vtkSmartPointer<vtkVectorText>		XPlusVectorText, XMinusVectorText, YPlusVectorText, YMinusVectorText, ZPlusVectorText, ZMinusVectorText;
	vtkSmartPointer<vtkActor>			XPlusActor, XMinusActor, YPlusActor, YMinusActor, ZPlusActor, ZMinusActor;

	/** L'éventuelle transformation utilisateur. */
	vtkTransform	*Transform;
};	// class vtkViewCubeActor


#endif // VTK_VIEWCUBE_ACTOR_H

