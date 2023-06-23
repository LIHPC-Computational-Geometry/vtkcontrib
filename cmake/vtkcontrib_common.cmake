find_package (GUIToolkitsVariables)

include (${GUIToolkitsVariables_CMAKE_DIR}/common_vtk.cmake)	# Positionne VTK_7, VTK_8, VTK_9

# Compilation :
add_definitions (-DVTK_CONTRIB_VERSION="${VTK_CONTRIB_VERSION}")
set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${EXTRA_CXX_FLAGS}")
if (VTK_7)
	message (STATUS "========================================= UTILISATION DE VTK 7 =========================================")
	if (USE_OPENGL_BACKEND)
		set (MANDATORY_CXX_OPTIONS -DVTKGL -DUSE_OPENGL_BACKEND -DVTK_7)	# Public
		set (MANDATORY_VTK_OPTIONS "-DvtkRenderingCore_AUTOINIT=3(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingOpenGL)" "-DvtkIOExport_AUTOINIT=1(vtkIOExportOpenGL)")
	else (USE_OPENGL_BACKEND)
		set (MANDATORY_CXX_OPTIONS -DVTKGL -DUSE_OPENGL2_BACKEND -DVTK_7)	# Public
		set (MANDATORY_VTK_OPTIONS "-DvtkRenderingCore_AUTOINIT=3(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingOpenGL2)" "-DvtkIOExport_AUTOINIT=1(vtkIOExportOpenGL2)")
	endif (USE_OPENGL_BACKEND)
	set (MANDATORY_PRIVATE_VTK_OPTIONS -DPATCH_VTK_MATH_SOLVE_LINEAR_SYSTEM)
elseif (VTK_8)
	message (STATUS "========================================= UTILISATION DE VTK 8 =========================================")
	if (USE_OPENGL_BACKEND)
		message (FATAL_ERROR "==> Backend OpenGL (1) incompatible avec VTK 8.")
	endif (USE_OPENGL_BACKEND)
	set (MANDATORY_CXX_OPTIONS -DVTKGL -DUSE_OPENGL2_BACKEND -DVTK_8)	# Public
	set (MANDATORY_VTK_OPTIONS -DPATCH_VTK_MATH_SOLVE_LINEAR_SYSTEM)
elseif (VTK_9)
	message (STATUS "========================================= UTILISATION DE VTK 9 =========================================")
	if (USE_OPENGL_BACKEND)
		message (FATAL_ERROR "==> Backend OpenGL (1) incompatible avec VTK 9.")
	endif (USE_OPENGL_BACKEND)
	set (MANDATORY_CXX_OPTIONS -DVTKGL -DUSE_OPENGL2_BACKEND -DVTK_9)	# Public
	set (MANDATORY_VTK_OPTIONS -DPATCH_VTK_MATH_SOLVE_LINEAR_SYSTEM)
else (VTK_7)
	message (STATUS "========================================= VERSION DE VTK NON DEFINIE =========================================")
	message (FATAL_ERROR "==> UTILISEZ -DVTK_7:BOOL=ON OU -DVTK_8:BOOL=ON OU -DVTK_9:BOOL=ON A LA LIGNE DE COMMANDE")
endif (VTK_7)


