#include "VtkContrib/vtkLandmarkActor.h"
#include "VtkContrib/vtkPropHelper.h"

#include <vtkRenderer.h>
#include <vtkCommand.h>
#include <vtkAssemblyNode.h>
#include <vtkAssemblyPath.h>
#include <vtkPropCollection.h>
#include <assert.h>

using namespace std;


vtkLandmarkActor::vtkLandmarkActor ( )
	: vtkKatCubeAxesActor ( )
{
}	// vtkLandmarkActor::vtkLandmarkActor


vtkLandmarkActor::vtkLandmarkActor (const vtkLandmarkActor&)
	: vtkKatCubeAxesActor ( )
{
	assert (0 && "vtkLandmarkActor copy constructor is not allowed.");
}	// vtkLandmarkActor copy constructor


vtkLandmarkActor& vtkLandmarkActor::operator = (const vtkLandmarkActor&)
{
	assert (0 && "vtkLandmarkActor::operator = is not allowed.");
	return *this;
}	// vtkLandmarkActor::operator =


vtkLandmarkActor::~vtkLandmarkActor ( )
{
}	// vtkLandmarkActor::~vtkLandmarkActor


void vtkLandmarkActor::PrintSelf (ostream& os, vtkIndent indent)
{
	vtkKatCubeAxesActor::PrintSelf (os, indent);
}	// vtkLandmarkActor::PrintSelf


vtkLandmarkActor* vtkLandmarkActor::New ( )
{
	return new vtkLandmarkActor ( );
}	// vtkLandmarkActor::New


void vtkLandmarkActor::SetXAxisColor (
				vtkFloatingPointType red, vtkFloatingPointType green,
				vtkFloatingPointType blue)
{
	for (int i = 0; i < 4; i++)
	{
		assert ((0 != XAxes [i]) && "vtkLandmarkActor::SetXAxisColor : null axis.");
		assert ((0 != XAxes [i]->GetProperty ( )) && "vtkLandmarkActor::SetXAxisColor : null property.");
		XAxes [i]->GetProperty ( )->SetColor (red, green, blue);

		vector<vtkActor*>	actors	= vtkPropHelper::getActors (XAxes [i]);
		for (vector<vtkActor*>::iterator it = actors.begin ( );
		     actors.end ( ) != it; it++)
		{
			assert ((0 != (*it)->GetProperty ( )) && "vtkLandmarkActor::SetXAxisColor : null property.");
			(*it)->GetProperty ( )->SetColor (red, green, blue);
		}
	}	// for (int i = 0; i < 4; i++)
}	// vtkLandmarkActor::SetXAxisColor


void vtkLandmarkActor::GetXAxisColor (vtkFloatingPointType rgb [3])
{
	assert ((0 != XAxes [0]) && "vtkLandmarkActor::GetXAxisColor : null axis.");
	assert ((0 != XAxes [0]->GetProperty ( )) && "vtkLandmarkActor::GetXAxisColor : null property.");
	XAxes [0]->GetProperty ( )->GetColor (rgb);
}	// vtkLandmarkActor::GetXAxisColor


void vtkLandmarkActor::SetYAxisColor (
				vtkFloatingPointType red, vtkFloatingPointType green,
				vtkFloatingPointType blue)
{
	for (int i = 0; i < 4; i++)
	{
		assert ((0 != YAxes [i]) && "vtkLandmarkActor::SetYAxisColor : null axis.");
		assert ((0 != YAxes [i]->GetProperty ( )) && "vtkLandmarkActor::SetYAxisColor : null property.");
		YAxes [i]->GetProperty ( )->SetColor (red, green, blue);
	}	// for (int i = 0; i < 4; i++)
}	// vtkLandmarkActor::SetYAxisColor


void vtkLandmarkActor::GetYAxisColor (vtkFloatingPointType rgb [3])
{
	assert ((0 != YAxes [0]) && "vtkLandmarkActor::GetYAxisColor : null axis.");
	assert ((0 != YAxes [0]->GetProperty ( )) && "vtkLandmarkActor::GetYAxisColor : null property.");
	YAxes [0]->GetProperty ( )->GetColor (rgb);
}	// vtkLandmarkActor::GetYAxisColor


void vtkLandmarkActor::SetZAxisColor (
				vtkFloatingPointType red, vtkFloatingPointType green,
				vtkFloatingPointType blue)
{
	for (int i = 0; i < 4; i++)
	{
		assert ((0 != ZAxes [i]) && "vtkLandmarkActor::SetZAxisColor : null axis.");
		assert ((0 != ZAxes [i]->GetProperty ( )) && "vtkLandmarkActor::SetZAxisColor : null property.");
		ZAxes [i]->GetProperty ( )->SetColor (red, green, blue);
	}	// for (int i = 0; i < 4; i++)
}	// vtkLandmarkActor::SetZAxisColor


void vtkLandmarkActor::GetZAxisColor (vtkFloatingPointType rgb [3])
{
	assert ((0 != ZAxes [0]) && "vtkLandmarkActor::GetZAxisColor : null axis.");
	assert ((0 != ZAxes [0]->GetProperty ( )) && "vtkLandmarkActor::GetZAxisColor : null property.");
	ZAxes [0]->GetProperty ( )->GetColor (rgb);
}	// vtkLandmarkActor::GetZAxisColor


void vtkLandmarkActor::SetXAxisLineWidth (vtkFloatingPointType width)
{
	for (int i = 0; i < 4; i++)
	{
		assert ((0 != ZAxes [i]) && "vtkLandmarkActor::SetXAxisLineWidth : null axis.");
		assert ((0 != ZAxes [i]->GetProperty ( )) && "vtkLandmarkActor::SetXAxisLineWidth : null property.");
		ZAxes [i]->GetProperty ( )->SetLineWidth (width);
	}	// for (int i = 0; i < 4; i++)
}	// vtkLandmarkActor::SetXAxisLineWidth


vtkFloatingPointType vtkLandmarkActor::GetXAxisLineWidth ( )
{
	assert ((0 != XAxes [0]) && "vtkLandmarkActor::GetXAxisLineWidth : null axis.");
	assert ((0 != XAxes [0]->GetProperty ( )) && "vtkLandmarkActor::GetXAxisLineWidth : null property.");
	return XAxes [0]->GetProperty ( )->GetLineWidth ( );
}	// vtkLandmarkActor::GetXAxisLineWidth


void vtkLandmarkActor::SetYAxisLineWidth (vtkFloatingPointType width)
{
	for (int i = 0; i < 4; i++)
	{
		assert ((0 != ZAxes [i]) && "vtkLandmarkActor::SetYAxisLineWidth : null axis.");
		assert ((0 != ZAxes [i]->GetProperty ( )) && "vtkLandmarkActor::SetYAxisLineWidth : null property.");
		ZAxes [i]->GetProperty ( )->SetLineWidth (width);
	}	// for (int i = 0; i < 4; i++)
}	// vtkLandmarkActor::SetYAxisLineWidth


vtkFloatingPointType vtkLandmarkActor::GetYAxisLineWidth ( )
{
	assert ((0 != YAxes [0]) && "vtkLandmarkActor::GetYAxisLineWidth : null axis.");
	assert ((0 != YAxes [0]->GetProperty ( )) && "vtkLandmarkActor::GetYAxisLineWidth : null property.");
	return YAxes [0]->GetProperty ( )->GetLineWidth ( );
}	// vtkLandmarkActor::GetYAxisLineWidth


void vtkLandmarkActor::SetZAxisLineWidth (vtkFloatingPointType width)
{
	for (int i = 0; i < 4; i++)
	{
		assert ((0 != ZAxes [i]) && "vtkLandmarkActor::SetZAxisLineWidth : null axis.");
		assert ((0 != ZAxes [i]->GetProperty ( )) && "vtkLandmarkActor::SetZAxisLineWidth : null property.");
		ZAxes [i]->GetProperty ( )->SetLineWidth (width);
	}	// for (int i = 0; i < 4; i++)
}	// vtkLandmarkActor::SetZAxisLineWidth


vtkFloatingPointType vtkLandmarkActor::GetZAxisLineWidth ( )
{
	assert ((0 != ZAxes [0]) && "vtkLandmarkActor::GetZAxisLineWidth : null axis.");
	assert ((0 != ZAxes [0]->GetProperty ( )) && "vtkLandmarkActor::GetZAxisLineWidth : null property.");
	return ZAxes [0]->GetProperty ( )->GetLineWidth ( );
}	// vtkLandmarkActor::GetZAxisLineWidth


void vtkLandmarkActor::SetNonDependentAttributes ( )
{
  vtkProperty *prop = this->GetProperty();
  prop->SetAmbient(1.0);
  prop->SetDiffuse(0.0);
  for (int i = 0; i < 4; i++)
    {
    this->XAxes[i]->SetCamera(this->Camera);
    this->XAxes[i]->GetProperty( )->SetAmbient(1.0);
    this->XAxes[i]->GetProperty( )->SetDiffuse(.0);
    this->XAxes[i]->SetTickLocation(this->TickLocation);
    this->XAxes[i]->SetDrawGridlines(this->DrawXGridlines);
    this->XAxes[i]->SetBounds(this->Bounds);
    this->XAxes[i]->AxisVisibilityOn();
    this->XAxes[i]->SetLabelVisibility(this->XAxisLabelVisibility);
    this->XAxes[i]->SetTitleVisibility(this->XAxisLabelVisibility);
    this->XAxes[i]->SetTickVisibility(this->XAxisTickVisibility);
    this->XAxes[i]->SetMinorTicksVisible(this->XAxisMinorTickVisibility);

    this->YAxes[i]->SetCamera(this->Camera);
    this->YAxes[i]->GetProperty( )->SetAmbient(1.0);
    this->YAxes[i]->GetProperty( )->SetDiffuse(.0);
    this->YAxes[i]->SetTickLocation(this->TickLocation);
    this->YAxes[i]->SetDrawGridlines(this->DrawYGridlines);
    this->YAxes[i]->SetBounds(this->Bounds);
    this->YAxes[i]->AxisVisibilityOn();
    this->YAxes[i]->SetLabelVisibility(this->YAxisLabelVisibility);
    this->YAxes[i]->SetTitleVisibility(this->YAxisLabelVisibility);
    this->YAxes[i]->SetTickVisibility(this->YAxisTickVisibility);
    this->YAxes[i]->SetMinorTicksVisible(this->YAxisMinorTickVisibility);

    this->ZAxes[i]->SetCamera(this->Camera);
    this->ZAxes[i]->GetProperty( )->SetAmbient(1.0);
    this->ZAxes[i]->GetProperty( )->SetDiffuse(.0);
    this->ZAxes[i]->SetTickLocation(this->TickLocation);
    this->ZAxes[i]->SetDrawGridlines(this->DrawZGridlines);
    this->ZAxes[i]->SetBounds(this->Bounds);
    this->ZAxes[i]->AxisVisibilityOn();
    this->ZAxes[i]->SetLabelVisibility(this->ZAxisLabelVisibility);
    this->ZAxes[i]->SetTitleVisibility(this->ZAxisLabelVisibility);
    this->ZAxes[i]->SetTickVisibility(this->ZAxisTickVisibility);
    this->ZAxes[i]->SetMinorTicksVisible(this->ZAxisMinorTickVisibility);
    }
}	// vtkLandmarkActor::SetNonDependentAttributes


