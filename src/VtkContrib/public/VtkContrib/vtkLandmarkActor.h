#ifndef VTK_LANDMARK_ACTOR_H
#define VTK_LANDMARK_ACTOR_H


#include "VtkContrib/vtkKatCubeAxesActor.h"


/**
 * Classe définissant un repère composé de 3 axes.
 * \author	C. Pignerol  CEA/DAM/DSSI
 */
class vtkLandmarkActor : public vtkKatCubeAxesActor
{
	public :

	virtual ~vtkLandmarkActor ( );

	static vtkLandmarkActor* New ( );

	virtual void PrintSelf (ostream& os, vtkIndent indent);

	virtual void SetXAxisColor (
					vtkFloatingPointType red, vtkFloatingPointType green,
					vtkFloatingPointType blue);
	virtual void GetXAxisColor (vtkFloatingPointType rgb [3]);
	virtual void SetYAxisColor (
					vtkFloatingPointType red, vtkFloatingPointType green,
					vtkFloatingPointType blue);
	virtual void GetYAxisColor (vtkFloatingPointType rgb [3]);
	virtual void SetZAxisColor (
					vtkFloatingPointType red, vtkFloatingPointType green,
					vtkFloatingPointType blue);
	virtual void GetZAxisColor (vtkFloatingPointType rgb [3]);
	virtual void SetXAxisLineWidth (vtkFloatingPointType width);
	virtual vtkFloatingPointType GetXAxisLineWidth ( );
	virtual void SetYAxisLineWidth (vtkFloatingPointType width);
	virtual vtkFloatingPointType GetYAxisLineWidth ( );
	virtual void SetZAxisLineWidth (vtkFloatingPointType width);
	virtual vtkFloatingPointType GetZAxisLineWidth ( );


	protected :

	vtkLandmarkActor ( );

	void SetNonDependentAttributes ( );


	private :

	vtkLandmarkActor (const vtkLandmarkActor&);
	vtkLandmarkActor& operator = (const vtkLandmarkActor&);
};	// class vtkLandmarkActor



#endif	// VTK_LANDMARK_ACTOR_H
