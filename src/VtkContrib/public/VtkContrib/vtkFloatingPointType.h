//
// CP : pour maintient compatibilité vtkFloatingPointType avec les versions antérieures
// aux la versions 7.x.
//

#ifndef VTK_FLOATING_POINT_TYPE_H
#define VTK_FLOATING_POINT_TYPE_H

#include <vtkType.h>

#if defined(VTK_7) || defined(VTK_8) || defined(VTK_9)
typedef vtkTypeFloat64	vtkFloatingPointType;
#endif	// VTK_7 || VTK_8 || VTK_9

#endif	// VTK_FLOATING_POINT_TYPE_H
