#ifndef VTK_PROP_HELPER
#define VTK_PROP_HELPER

#include "VtkContrib/vtk_versions.h"
#include <vtkProp.h>
#include <vtkAssembly.h>
#include <vtkPropAssembly.h>
#include <vtkTextProperty.h>
#include <vector>



/**
 * Assistant pour des operations sur les proprietes.
 * \author	C. Pignerol  CEA/DAM/DSSI
 * */
class vtkPropHelper
{
	public :


	/**
	 * Précondition : assembly non nul.
	 * @return		un vecteur contenant la liste des acteurs de la 
	 *				propriété.
	 */
	static std::vector<vtkActor*> getActors (vtkPropAssembly* assembly);

	/**
	 * Précondition : assembly non nul.
	 * @return		un vecteur contenant la liste des acteurs de la 
	 *				propriété.
	 */
	static std::vector<vtkActor*> getActors (vtkAssembly* assembly);

	/**
	 * Précondition : actor non nul.
	 * @return		un vecteur contenant la liste des acteurs de la 
	 *				propriété.
	 */
	static std::vector<vtkActor*> getActors (vtkActor* actor);

	/**
	 * Copie les propriétés d'une instance à l'autre (type DeepCopy).
	 * @param		instance à copier. Elle n'est pas constante car ses
	 *				accesseurs en lecture ne sont pas constants.
	 * @param		instance modifiée par l'opération de copie.
	 */
	static void copyTextProperties (vtkTextProperty& source, 
	                                vtkTextProperty& dest);

	private :

	vtkPropHelper ( );

	vtkPropHelper (const vtkPropHelper&);

	vtkPropHelper& operator = (const vtkPropHelper&);

	~vtkPropHelper ( );
};	// class vtkPropHelper


/**
 * @return		true si les deux instances sont égales, sinon false.
 */
bool operator == (const vtkTextProperty& left, const vtkTextProperty& right);

/**
 * @return		true si les deux instances sont différentes, sinon false.
 */
inline bool operator != (const vtkTextProperty& left, const vtkTextProperty& right)
{ return !(left == right); }


#endif	// VTK_PROP_HELPER
