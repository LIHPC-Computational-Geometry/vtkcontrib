#ifndef VTK_TRIHEDRON_COMMAND_H
#define VTK_TRIHEDRON_COMMAND_H


#include "VtkContrib/vtkTrihedron.h"

#include <vtkCommand.h>
#include <vtkCamera.h>
#include <vtkRenderer.h>



/**
 * Commande reliant un trièdre et son renderer. Cette commande doit être
 * utilisée comme observateur d'une instance de la classe 
 * vtkInteractorObserver. Elle répond aux évènements VTKIS_ROTATE
 * et VTKIS_SPIN et met alors à jour la vue du trièdre (représentée
 * ici par une instance de la classe vtkRenderer) avec celle associée à 
 * l'interacteur.
 * @author		F. Ledoux, C. Pignerol	CEA/DAM/DSSI
 */
class vtkTrihedronCommand : public vtkCommand
{
	public :

	/**
	 * Créé une instance de la classe.
	 */
	static vtkTrihedronCommand* New ( );

	/**
	 * Appelle SynchronizeViews si l'évènement à l'origine de cet
	 * appel est VTKIS_ROTATE ou VTKIS_SPIN.
	 */
	void Execute (vtkObject* caller, unsigned long eventId, void* callData);

	/**
	 * @return		Trièdre géré
	 */
	virtual vtkTrihedron* GetTrihedron ( );

	/**
	 * @param		Nouveau trièdre à géré
	 */
	virtual void SetTrihedron (vtkTrihedron* trihedron);

	/**
	 * @return		Renderer utilisé
	 */
	virtual vtkRenderer* GetRenderer ( );

	/**
	 * @param		Nouveau renderer utilisé
	 */
	virtual void SetRenderer (vtkRenderer* renderer);

	/**
	 * Synchronise sa représentation avec celle de l'interacteur qui le 
	 * pilote.
	 * @param		Caméra de la vue à suivre
	 */
	virtual void SynchronizeViews (vtkCamera* camera);


	protected :

	/**
	 * Constructeur. RAS.
	 */
	vtkTrihedronCommand ( );

	/**
	 * Destructeur. RAS.
	 */
	virtual ~vtkTrihedronCommand ( );

	/**
	 * Constructeur de copie et opérateur = : interdits.
	 */
	vtkTrihedronCommand (const vtkTrihedronCommand&);
	vtkTrihedronCommand& operator = (const vtkTrihedronCommand&);

	/** Trièdre associé. */
	vtkTrihedron*				m_trihedron;

	/** Renderer associé. */
	vtkRenderer*				m_renderer;
};	// class vtkTrihedronCommand


#endif	// VTK_TRIHEDRON_COMMAND_H
