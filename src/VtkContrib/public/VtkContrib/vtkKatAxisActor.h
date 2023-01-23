// MODIFICATIONS CP : 
//	- methode New implementee.
//  - methode GetActors surchargee.
//  - Remplacement des float par des vtkFloatingPointType

/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile$
  Language:  C++
  Date:      $Date: 2006-10-05 10:18:55 +0200 (Thu, 05 Oct 2006) $
  Version:   $Revision: 326 $
  Thanks:    Kathleen Bonnell, B Division, Lawrence Livermore Nat'l Laboratory

Copyright (c) 1993-2000 Ken Martin, Will Schroeder, Bill Lorensen 
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

 * Neither name of Ken Martin, Will Schroeder, or Bill Lorensen nor the names
   of any contributors may be used to endorse or promote products derived
   from this software without specific prior written permission.

 * Modified source versions must be plainly marked as such, and must not be
   misrepresented as being the original software.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

=========================================================================*/
// .NAME vtkKatAxisActor - Create an axis with tick marks and labels
// .SECTION Description
// vtkKatAxisActor creates an axis with tick marks, labels, and/or a title,
// depending on the particular instance variable settings. It is assumed that
// the axes is part of a bounding box and is orthoganal to one of the
// coordinate axes.  To use this class, you typically specify two points 
// defining the start and end points of the line (xyz definition using 
// vtkCoordinate class), the axis type (X, Y or Z), the axis location in 
// relation to the bounding box, the bounding box, the number of labels, and
// the data range (min,max). You can also control what parts of the axis are
// visible including the line, the tick marks, the labels, and the title. It
// is also possible to control gridlines, and specifiy on which 'side' the
// tickmarks are drawn (again with respect to the underlying assumed 
// bounding box). You can also specify the label format (a printf style format).
//
// This class decides how to locate the labels, and how to create reasonable 
// tick marks and labels. 
//
// Labels follow the camera so as to be legible from any viewpoint.
//
// The instance variables Point1 and Point2 are instances of vtkCoordinate.
// All calculations and references are in World Coordinates.
//
// .SECTION Notes
// This class was adapted from a 2D version created by Hank Childs called
// vtkHankAxisActor2D.
//
// .SECTION See Also
// vtkActor vtkVectorText vtkPolyDataMapper vtkAxisActor2D vtkCoordinate

#ifndef __vtkKatAxisActor_h
#define __vtkKatAxisActor_h
#include "VtkContrib/vtk_versions.h"
#include <VtkContrib/visit_vtk_exports.h>

#include "VtkContrib/vtkFloatingPointType.h"	// CP v 3.0.0

#include "vtkViewport.h"
#include "vtkProperty.h"
#include "vtkCoordinate.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"

#include "vtkActor.h"
#include "vtkPolyDataMapper.h"
#include "vtkVectorText.h"
#include "vtkFollower.h"
#include "vtkCamera.h"

#define VTK_MAX_LABELS    200
#define VTK_MAX_TICKS     1000

#define VTK_AXIS_TYPE_X   0
#define VTK_AXIS_TYPE_Y   1
#define VTK_AXIS_TYPE_Z   2

#define VTK_TICKS_INSIDE  0
#define VTK_TICKS_OUTSIDE 1
#define VTK_TICKS_BOTH    2

#define VTK_AXIS_POS_MINMIN 0
#define VTK_AXIS_POS_MINMAX 1
#define VTK_AXIS_POS_MAXMAX 2
#define VTK_AXIS_POS_MAXMIN 3

class VISIT_VTK_API vtkKatAxisActor : public vtkActor
{
public:
  vtkTypeMacro(vtkKatAxisActor,vtkActor);
  void PrintSelf(ostream& os, vtkIndent indent);

  // Description:
  // Instantiate object.
  static vtkKatAxisActor *New()
  { return new vtkKatAxisActor; }

  // Description:
  // Collect all actors.
  virtual void GetActors(vtkPropCollection* ac);
  
  // Description:
  // Specify the position of the first point defining the axis.
  vtkWorldCoordinateMacro(Point1); 

  // Description:
  // Specify the position of the second point defining the axis.
  vtkWorldCoordinateMacro(Point2); 

  // Description:
  // Specify the (min,max) axis range. This will be used in the generation
  // of labels, if labels are visible.
  vtkSetVector2Macro(Range,vtkFloatingPointType);	
  vtkGetVectorMacro(Range,vtkFloatingPointType,2);	

  // Description:
  void   SetBounds(vtkFloatingPointType[6]);
  vtkFloatingPointType *GetBounds(void);
  void   GetBounds(vtkFloatingPointType[6]);

  // Description:
  // Set/Get the format with which to print the labels on the axis.
  vtkSetStringMacro(LabelFormat);
  vtkGetStringMacro(LabelFormat);

  // Description:
  // Set/Get the flag that controls whether the minor ticks are visible. 
  vtkSetMacro(MinorTicksVisible, int);
  vtkGetMacro(MinorTicksVisible, int);
  vtkBooleanMacro(MinorTicksVisible, int);


  // Description:
  // Set/Get the title of the axis actor,
  vtkSetStringMacro(Title);
  vtkGetStringMacro(Title);

  // Description:
  // Set/Get the size of the major tick marks 
  vtkSetMacro(MajorTickSize, vtkFloatingPointType);
  vtkGetMacro(MajorTickSize, vtkFloatingPointType);
  
  // Description:
  // Set/Get the size of the major tick marks 
  vtkSetMacro(MinorTickSize, vtkFloatingPointType);
  vtkGetMacro(MinorTickSize, vtkFloatingPointType);

  // Description:
  // Set/Get the location of the ticks.
  vtkSetClampMacro(TickLocation, int, VTK_TICKS_INSIDE, VTK_TICKS_BOTH);
  vtkGetMacro(TickLocation, int);

  void SetTickLocationToInside(void)
    { this->SetTickLocation(VTK_TICKS_INSIDE); };
  void SetTickLocationToOutside(void)
    { this->SetTickLocation(VTK_TICKS_OUTSIDE); };
  void SetTickLocationToBoth(void)
    { this->SetTickLocation(VTK_TICKS_BOTH); };
  
  // Description:
  // Set/Get visibility of the axis line.
  vtkSetMacro(AxisVisibility, int);
  vtkGetMacro(AxisVisibility, int);
  vtkBooleanMacro(AxisVisibility, int);

  // Description:
  // Set/Get visibility of the axis tick marks.
  vtkSetMacro(TickVisibility, int);
  vtkGetMacro(TickVisibility, int);
  vtkBooleanMacro(TickVisibility, int);

  // Description:
  // Set/Get visibility of the axis labels.
  vtkSetMacro(LabelVisibility, int);
  vtkGetMacro(LabelVisibility, int);
  vtkBooleanMacro(LabelVisibility, int);

  // Description:
  // Set/Get visibility of the axis title.
  vtkSetMacro(TitleVisibility, int);
  vtkGetMacro(TitleVisibility, int);
  vtkBooleanMacro(TitleVisibility, int);

  // Description:
  // Set/Get whether gridlines should be drawn.
  vtkSetMacro(DrawGridlines, int);
  vtkGetMacro(DrawGridlines, int);
  vtkBooleanMacro(DrawGridlines, int);

  // Description:
  // Set/Get the length to use when drawing gridlines. 
  vtkSetMacro(GridlineXLength, vtkFloatingPointType);
  vtkGetMacro(GridlineXLength, vtkFloatingPointType);
  vtkSetMacro(GridlineYLength, vtkFloatingPointType);
  vtkGetMacro(GridlineYLength, vtkFloatingPointType);
  vtkSetMacro(GridlineZLength, vtkFloatingPointType);
  vtkGetMacro(GridlineZLength, vtkFloatingPointType);

  // Description:
  // Set/Get the type of this axis.
  vtkSetClampMacro(AxisType, int, VTK_AXIS_TYPE_X, VTK_AXIS_TYPE_Z);
  vtkGetMacro(AxisType, int);
  void SetAxisTypeToX(void) { this->SetAxisType(VTK_AXIS_TYPE_X); };
  void SetAxisTypeToY(void) { this->SetAxisType(VTK_AXIS_TYPE_Y); };
  void SetAxisTypeToZ(void) { this->SetAxisType(VTK_AXIS_TYPE_Z); };

  // Description:
  // Set/Get the position of this axis (in relation to an an
  // assumed bounding box).  For an x-type axis, MINMIN corresponds
  // to the x-edge in the bounding box where Y values are minimum and 
  // Z values are minimum. For a y-type axis, MAXMIN corresponds to the 
  // y-edge where X values are maximum and Z values are minimum.
  // 
  vtkSetClampMacro(AxisPosition, int, VTK_AXIS_POS_MINMIN, VTK_AXIS_POS_MAXMIN);
  vtkGetMacro(AxisPosition, int);

  void SetAxisPositionToMinMin(void)
      { this->SetAxisPosition(VTK_AXIS_POS_MINMIN); };
  void SetAxisPositionToMinMax(void)
      { this->SetAxisPosition(VTK_AXIS_POS_MINMAX); };
  void SetAxisPositionToMaxMax(void)
      { this->SetAxisPosition(VTK_AXIS_POS_MAXMAX); };
  void SetAxisPositionToMaxMin(void)
      { this->SetAxisPosition(VTK_AXIS_POS_MAXMIN); };

  // Description:
  // Set/Get the camera for this axis.  The camera is used by the
  // labels to 'follow' the camera and be legible from any viewpoint.
  vtkSetObjectMacro(Camera, vtkCamera);
  vtkGetObjectMacro(Camera, vtkCamera);

  // Description:
  // Draw the axis. 
  int RenderOverlay(vtkViewport* viewport);
  int RenderOpaqueGeometry(vtkViewport* viewport);
  int RenderTranslucentGeometry(vtkViewport *) {return 0;}

  // Description:
  // Release any graphics resources that are being consumed by this actor.
  // The parameter window could be used to determine which graphic
  // resources to release.
  void ReleaseGraphicsResources(vtkWindow *);

  // Description:
  // Computes the range of the axis given an input range.  It does not 
  // necessarily place tick marks at the endpoints so that it can place
  // them accurately where "nice" numbers lie.
  void AdjustTicksComputeRange(vtkFloatingPointType inRange[2]);	

  // Description:
  // Shallow copy of an axis actor. Overloads the virtual vtkProp method.
  void ShallowCopy(vtkProp *prop);

  // Description:
  // Set/Unset the use of a scale factor for label values. 
  void SetValueScaleFactor(const vtkFloatingPointType);
  void UnSetValueScaleFactor(void);


  void BuildAxis(vtkViewport *viewport);

protected:
  vtkKatAxisActor();
  ~vtkKatAxisActor();

  char  *Title;
  vtkFloatingPointType  Range[2];		
  vtkFloatingPointType  LastRange[2];
  char  *LabelFormat;
  int    NumberOfLabelsBuilt;
  int    MinorTicksVisible;
  int    LastMinorTicksVisible;
  int    TickLocation;
 
  int    DrawGridlines;
  int    LastDrawGridlines;
  vtkFloatingPointType  GridlineXLength;
  vtkFloatingPointType  GridlineYLength;
  vtkFloatingPointType  GridlineZLength;
  
  int    AxisVisibility;
  int    TickVisibility;
  int    LastTickVisibility;
  int    LabelVisibility;
  int    TitleVisibility;
  
  int    AxisType;
  int    AxisPosition;
  vtkFloatingPointType  Bounds[6];
  
private:
  vtkKatAxisActor(const vtkKatAxisActor&);
  void operator=(const vtkKatAxisActor&);


  void BuildXTypeAxis(vtkViewport *, vtkFloatingPointType p1[3], vtkFloatingPointType p2[3], vtkFloatingPointType ext);	
  void BuildYTypeAxis(vtkViewport *, vtkFloatingPointType p1[3], vtkFloatingPointType p2[3], vtkFloatingPointType ext);	
  void BuildZTypeAxis(vtkViewport *, vtkFloatingPointType p1[3], vtkFloatingPointType p2[3], vtkFloatingPointType ext);	

  void TransformBounds(vtkViewport *, vtkFloatingPointType bnds[6]);

  bool BuildLabels(vtkFloatingPointType, vtkFloatingPointType);
  void ScaleAndSetLabels(vtkViewport *);
  void SetNumberOfLabels(const int);

  void BuildTitle(vtkFloatingPointType p1[3], vtkFloatingPointType p2[3]);

  void SetAxisPointsAndLines(vtkFloatingPointType p1[3], vtkFloatingPointType p2[3]);
  bool BuildTickPointsForXType(vtkFloatingPointType p1[3], vtkFloatingPointType p2[3]);	
  bool BuildTickPointsForYType(vtkFloatingPointType p1[3], vtkFloatingPointType p2[3]);	
  bool BuildTickPointsForZType(vtkFloatingPointType p1[3], vtkFloatingPointType p2[3]);	

  bool TickVisibilityChanged(void);

  vtkCoordinate *Point1Coordinate;
  vtkCoordinate *Point2Coordinate;

  vtkFloatingPointType  MajorTickSize;
  vtkFloatingPointType  MinorTickSize;

  vtkFloatingPointType  MajorStart;
  vtkFloatingPointType  MinorStart;

  vtkFloatingPointType  DeltaMinor;
  vtkFloatingPointType  DeltaMajor;

  vtkFloatingPointType  valueScaleFactor;
  bool   mustAdjustValue;

  int    LastAxisPosition;
  int    LastAxisType;
  int    LastTickLocation;
  vtkFloatingPointType  LastLabelStart;

  vtkPoints         *minorTickPts;
  vtkPoints         *majorTickPts;
  vtkPoints         *gridlinePts;

  vtkVectorText     *TitleVector;
  vtkPolyDataMapper *TitleMapper;
  vtkFollower       *TitleActor;

  vtkVectorText     **LabelVectors;
  vtkPolyDataMapper **LabelMappers;
  vtkFollower       **LabelActors;

  vtkPolyData        *Axis;
  vtkPolyDataMapper  *AxisMapper;
  vtkActor           *AxisActor;

  vtkCamera          *Camera;
  vtkTimeStamp        BuildTime;
  vtkTimeStamp        BoundsTime;
  vtkTimeStamp        LabelBuildTime;

  int                 AxisHasZeroLength;
  int                 ForceLabelReset;
};


#endif
