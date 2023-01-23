#include "VtkContrib/vtkPropHelper.h"
#include "VtkContrib/vtkFloatingPointType.h"

#include <assert.h>


#include <string>
#include <stdexcept>

#include <vtkActor.h>
#include <vtkAssembly.h>
#include <vtkVolume.h>
#include <vtkFollower.h>
#include <vtkLODActor.h>
#include <vtkLODProp3D.h>
#include <vtkActor2D.h>
#include <vtkImageActor.h>
#include <vtkPropAssembly.h>
#include <vtkProp3DCollection.h>

using namespace std;


vector<vtkActor*> vtkPropHelper::getActors (vtkPropAssembly* assembly)
{
	// Rem : assembly->GetActors (collection) retourne une liste vide ...
	assert ((0 != assembly) && "vtkPropHelper::getActors : nul assembly.");
	vector<vtkActor*>	actors;

	vtkPropCollection*	collection	= assembly->GetParts ( );
	vtkProp*			property	= 0;
	collection->InitTraversal ( );
	while (0 != (property = collection->GetNextProp ( )))
	{
		vtkActor*	actor	= vtkActor::SafeDownCast (property);
		if (0 != actor)
		{
			actors.push_back (actor);
			continue;
		}	// if (0 != actor)

		vtkPropAssembly*	prop	= vtkPropAssembly::SafeDownCast (property);
		if (0 != prop)
		{
			vector<vtkActor*>	list	= vtkPropHelper::getActors (prop);
			for (vector<vtkActor*>::iterator it = list.begin ( );
			     list.end ( ) != it; it++)
				actors.push_back (*it);
			continue;
		}	// if (0 != prop)

		vtkAssembly*	prop3D	= vtkAssembly::SafeDownCast (property);
		if (0 != prop3D)
		{
			vector<vtkActor*>	list	= vtkPropHelper::getActors (prop3D);
			for (vector<vtkActor*>::iterator it = list.begin ( );
			     list.end ( ) != it; it++)
				actors.push_back (*it);
			continue;
		}	// if (0 != prop3D)
	}	// while (0 != (property = collection->GetNextProp ( )))

	return actors;
}	// vtkPropHelper::getActors


vector<vtkActor*> vtkPropHelper::getActors (vtkAssembly* assembly)
{
	assert ((0 != assembly) && "vtkPropHelper::getActors : nul assembly.");
	vector<vtkActor*>		actors;

	vtkPropCollection*	collection	= vtkPropCollection::New ( );
	assembly->GetActors (collection);

	collection->InitTraversal ( );
	vtkProp*	property	= 0;
	while (0 != (property = collection->GetNextProp ( )))
	{
		vtkActor*	actor	= vtkActor::SafeDownCast (property);
		assert ((0 != actor) && "vtkPropHelper::getActors : unexpected property type.");
		actors.push_back (actor);
	}	// while (0 != (property = collection->GetNextProp ( )))
	collection->Delete ( );

	return actors;
}	// vtkPropHelper::getActors


vector<vtkActor*> vtkPropHelper::getActors (vtkActor* actor)
{
	assert ((0 != actor) && "vtkPropHelper::getActors : nul actor.");
	vector<vtkActor*>		actors;

	vtkPropCollection*	collection	= vtkPropCollection::New ( );
	actor->GetActors (collection);

	collection->InitTraversal ( );
	vtkProp*	property	= 0;
	while (0 != (property = collection->GetNextProp ( )))
	{
		vtkActor*	a	= vtkActor::SafeDownCast (property);
		assert ((0 != a) && "vtkPropHelper::getActors : unexpected property type.");
		actors.push_back (a);
	}	// while (0 != (property = collection->GetNextProp ( )))
	collection->Delete ( );

	return actors;
}	// vtkPropHelper::getActors


void vtkPropHelper::copyTextProperties (vtkTextProperty& source,
                                        vtkTextProperty& dest)
{
	dest.SetFontFamily (source.GetFontFamily ( ));
	dest.SetFontSize (source.GetFontSize ( ));
	dest.SetBold (source.GetBold ( ));
	dest.SetItalic (source.GetItalic ( ));
	vtkFloatingPointType	color [3];
	source.GetColor (color);
	dest.SetColor (color);
	dest.SetOpacity (source.GetOpacity ( ));
	dest.SetShadow (source.GetShadow ( ));
	dest.SetShadowOffset (source.GetShadowOffset ( ));
	dest.SetOrientation (source.GetOrientation ( ));
	dest.SetJustification (source.GetJustification ( ));
	dest.SetVerticalJustification (source.GetVerticalJustification ( ));
	dest.SetLineOffset (source.GetLineOffset ( ));
	dest.SetLineSpacing (source.GetLineSpacing ( ));
}	// vtkPropHelper::copyTextProperties


bool operator == (const vtkTextProperty& left, const vtkTextProperty& right)
{
	// Rem : accesseurs non constants
	vtkTextProperty&	l	= (vtkTextProperty&)left;
	vtkTextProperty&	r	= (vtkTextProperty&)right;

	if (l.GetFontFamily ( ) != r.GetFontFamily ( ))
		return false;
	if (l.GetFontSize ( ) != r.GetFontSize ( ))
		return false;
	if (l.GetBold ( ) != r.GetBold ( ))
		return false;
	if (l.GetItalic ( ) != r.GetItalic ( ))
		return false;
	if (l.GetOpacity ( ) != r.GetOpacity ( ))
		return false;
	if (l.GetShadow ( ) != r.GetShadow ( ))
		return false;
	if (l.GetShadowOffset ( ) != r.GetShadowOffset ( ))	
		return false;
	if (l.GetOrientation ( ) != r.GetOrientation ( ))	
		return false;
	if (l.GetJustification ( ) != r.GetJustification ( ))
		return false;
	if (l.GetVerticalJustification ( ) != r.GetVerticalJustification ( ))
		return false;
	if (l.GetLineOffset ( ) != r.GetLineOffset ( ))
		return false;
	if (l.GetLineSpacing ( ) != r.GetLineSpacing ( ))
		return false;
	vtkFloatingPointType	rgb1 [3], rgb2 [3];
	l.GetColor (rgb1);
	r.GetColor (rgb2);
	for (int i = 0; i < 3; i++)
		if (rgb1 [i] != rgb2 [i])
			return false;

	return true;
}	// operator == (const vtkTextProperty&, const vtkTextProperty&)
