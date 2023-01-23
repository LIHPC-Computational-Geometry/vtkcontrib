#include "VtkContrib/vtkTrihedronCommand.h"

#include <vtkObjectFactory.h>
#include <vtkCamera.h>

#include <assert.h>


vtkTrihedronCommand::vtkTrihedronCommand ( )
	: vtkCommand ( ), m_trihedron (0), m_renderer (0)
{
}	// vtkTrihedronCommand::vtkTrihedronCommand


vtkTrihedronCommand::vtkTrihedronCommand (const vtkTrihedronCommand&)
	: vtkCommand ( ), m_trihedron (0), m_renderer (0)
{
	assert (0 && "vtkTrihedronCommand copy constructor is not allowed.");
}	// vtkTrihedronCommand::vtkTrihedronCommand (const vtkTrihedronCommand&)


vtkTrihedronCommand& vtkTrihedronCommand::operator = (const vtkTrihedronCommand&)
{
	assert (0 && "vtkTrihedronCommand operator = is not allowed.");
	return *this;
}	// vtkTrihedronCommand::operator =


vtkTrihedronCommand::~vtkTrihedronCommand ( )
{
	SetTrihedron (0);
	SetRenderer (0);
}	// vtkTrihedronCommand::~vtkTrihedronCommand


vtkTrihedronCommand* vtkTrihedronCommand::New ( )
{
	vtkObject*	object	= 
			vtkObjectFactory::CreateInstance ("vtkTrihedronCommand");
	if (0 != object)
		return (vtkTrihedronCommand*)object;

    return new vtkTrihedronCommand;
}	// vtkTrihedronCommand::New


vtkTrihedron* vtkTrihedronCommand::GetTrihedron ( )
{
	return m_trihedron;
}	// vtkTrihedronCommand::GetTrihedron


void vtkTrihedronCommand::SetTrihedron (vtkTrihedron* trihedron)
{
	if (0 != m_trihedron)
		m_trihedron->Delete ( );

	m_trihedron	= trihedron;
	if (0 != m_trihedron)
		m_trihedron->Register (this);
}	// vtkTrihedronCommand::SetTrihedron


vtkRenderer* vtkTrihedronCommand::GetRenderer ( )
{
	return m_renderer;
}	// vtkTrihedronCommand::GetRenderer


void vtkTrihedronCommand::SetRenderer (vtkRenderer* renderer)
{
	if (0 != m_renderer)
		m_renderer->Delete ( );

	m_renderer	= renderer;
	if (0 != m_renderer)
		m_renderer->Register (this);
}	// vtkTrihedronCommand::SetRenderer


void vtkTrihedronCommand::Execute (
					vtkObject* caller, unsigned long eventId, void* callData)
{
	vtkRenderer*	renderer	= vtkRenderer::SafeDownCast (caller);
	if (0 == renderer)
		return;

	vtkCamera*	camera	= renderer->GetActiveCamera ( );
	if (0 != camera)
			SynchronizeViews (camera);
}	// vtkTrihedronCommand::Execute


void vtkTrihedronCommand::SynchronizeViews (vtkCamera* camera)
{
	assert ((0 != camera) && "vtkTrihedronCommand::SynchronizeViews : null camera");
	assert ((0 != GetRenderer ( )) && "vtkTrihedronCommand::SynchronizeViews : null renderer.");
	vtkCamera*	localCamera	= GetRenderer ( )->GetActiveCamera ( );
	assert ((0 != localCamera) && "vtkTrihedronCommand::SynchronizeViews : null local camera");

	localCamera->SetViewUp (camera->GetViewUp ( ));

	double	position [3],	focal [3];
	camera->GetPosition (position);
	camera->GetFocalPoint (focal);
	for (int i = 0; i < 3; i++)
		position [i]	-= focal [i];
	localCamera->SetFocalPoint (0., 0., 0.);
	localCamera->SetPosition (position);
	GetRenderer ( )->ResetCamera ( );

	double	disp1[4], disp2[4];
	GetRenderer ( )->SetWorldPoint(0,0,0,1);
	GetRenderer ( )->WorldToDisplay ( );
	GetRenderer ( )->GetDisplayPoint(disp1);
	GetRenderer ( )->SetViewPoint(0,0,0);
	GetRenderer ( )->ViewToDisplay();
	GetRenderer ( )->GetDisplayPoint(disp2);
	GetRenderer ( )->SetDisplayPoint(disp2[0],disp2[1],disp1[2]);
	GetRenderer ( )->DisplayToWorld();
	double	newPos[4]	= {0,0,0,1};
	GetRenderer ( )->GetWorldPoint(newPos);

	if (newPos[3])
	{
		newPos[0]	/=newPos[3];
		newPos[1]	/=newPos[3];
		newPos[2]	/=newPos[3];
		newPos[3]	= 1.;
	}

	double	cFoc [3], cPos [3];
	localCamera->GetFocalPoint(cFoc);
	localCamera->GetPosition(cPos);
	localCamera->SetFocalPoint (
		cFoc[0] - newPos[0], cFoc[1] - newPos[1], cFoc[2] - newPos[2]);
	localCamera->SetPosition (
		cPos[0] - newPos[0], cPos[1] - newPos[1], cPos[2] - newPos[2]);
}	// vtkTrihedronCommand::SynchronizeViews


