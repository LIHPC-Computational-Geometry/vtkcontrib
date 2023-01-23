#ifndef VTK_VERSIONS_H
#define VTK_VERSIONS_H


#ifdef VTK_4

#ifndef vtkFloatingPointType
#define vtkFloatingPointType vtkFloatingPointType
typedef float vtkFloatingPointType;
#endif	// vtkFloatingPointType

#endif	// VTK_4

#if defined(VTK_7) || defined(VTK_8) || defined(VTK_9)

#include <vtkType.h>
#define VTK_LARGE_INTEGER VTK_INT_MAX
#define VTK_LARGE_FLOAT VTK_FLOAT_MAX

#endif	// VTK_7 || VTK_8 || VTK_9


#endif	// VTK_VERSIONS_H
