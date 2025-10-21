#ifndef VTK_POLYGON_FILTER_H
#define VTK_POLYGON_FILTER_H


#include <vtkPolyDataAlgorithm.h>
#include <vtkIndent.h>
#include <vtkCell.h>
#include <iostream>


/** <P>Filtre permettant d'extraire les polygones d'une instance de vtkPolyData.
 * </P>
 * @author		Charles PIGNEROL, CEA/DAM/DCLC
 */
class vtkPolygonFilter : public vtkPolyDataAlgorithm
{
	public :

	/** Destructeur : RAS
	 */
	virtual ~vtkPolygonFilter ( );

	/** Instanciation de cette classe.
	 */
	static vtkPolygonFilter* New ( );

	vtkTypeMacro(vtkPolygonFilter,vtkPolyDataAlgorithm);

	/**
	 * Affiche quelques infos sur l'instance.
	 * @param		flux d'impression
	 * @param		indentation utilisée
	 */
	virtual void PrintSelf (ostream& os, vtkIndent indent);

	/**
	 * Effectue l'extraction.
	 */
	virtual int RequestData (vtkInformation* request, vtkInformationVector** inputVector, vtkInformationVector* outputVector);


	protected :

	/** Constructeur.
	 */
	vtkPolygonFilter ( );
	

	private :

	/** Constructeur de copie : interdit. */
	vtkPolygonFilter (const vtkPolygonFilter&);

	/** Opérateur = : interdit. */
	vtkPolygonFilter& operator = (const vtkPolygonFilter&);
};	// class vtkPolygonFilter


#endif	// VTK_POLYGON_FILTER_H
