#
# common.cmake : instructions de compilation partagées par tous les CMakeLists.txt.
# include (version.cmake) ici ne fonctionnenent pas. Why ??? Ce fichier est déjà un
#fichier déjà inclus ???
# => include version.cmake avant celui-ci.
#

include (${CMAKE_SOURCE_DIR}/cmake/organization.cmake)


# Compilation :
add_definitions (-DVTK_CONTRIB_VERSION="${VTK_CONTRIB_VERSION}")
set (CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${EXTRA_CXX_FLAGS}")
if (VTK_7)
	message (STATUS "========================================= UTILISATION DE VTK 7 =========================================")
	if (USE_OPENGL_BACKEND)
		set (MANDATORY_CXX_OPTIONS -DMULTITHREADED_APPLICATION -DUSE_OPENGL_BACKEND -DVTK_7)	# Public
		set (MANDATORY_VTK_OPTIONS "-DvtkRenderingCore_AUTOINIT=3(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingOpenGL)" "-DvtkIOExport_AUTOINIT=1(vtkIOExportOpenGL)")
	else (USE_OPENGL_BACKEND)
		set (MANDATORY_CXX_OPTIONS -DMULTITHREADED_APPLICATION -DUSE_OPENGL2_BACKEND -DVTK_7)	# Public
		set (MANDATORY_VTK_OPTIONS "-DvtkRenderingCore_AUTOINIT=3(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingOpenGL2)" "-DvtkIOExport_AUTOINIT=1(vtkIOExportOpenGL2)")
	endif (USE_OPENGL_BACKEND)
	set (MANDATORY_PRIVATE_VTK_OPTIONS -DPATCH_VTK_MATH_SOLVE_LINEAR_SYSTEM)
elseif (VTK_8)
	message (STATUS "========================================= UTILISATION DE VTK 8 =========================================")
	set (MANDATORY_CXX_OPTIONS -DMULTITHREADED_APPLICATION -DUSE_OPENGL2_BACKEND -DVTK_8)	# Public
	set (MANDATORY_VTK_OPTIONS -DPATCH_VTK_MATH_SOLVE_LINEAR_SYSTEM)
elseif (VTK_9)
	message (STATUS "========================================= UTILISATION DE VTK 9 =========================================")
	set (MANDATORY_CXX_OPTIONS -DMULTITHREADED_APPLICATION -DUSE_OPENGL2_BACKEND -DVTK_9)	# Public
	set (MANDATORY_VTK_OPTIONS -DPATCH_VTK_MATH_SOLVE_LINEAR_SYSTEM)
else (VTK_7)
	message (STATUS "========================================= VERSION DE VTK NON DEFINIE =========================================")
	message (STATUS "==> UTILISEZ -DVTK_7=ON OU -DVTK_8=ON OU -DVTK_9=ON A LA LIGNE DE COMMANDE")
endif (VTK_7)

if (NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_definitions (-DNDEBUG)
endif()

# Edition des liens :
# A l'installation les RPATHS utilisés seront ceux spécifiés ci-dessous (liste
# de répertoires séparés par des ;) :
#set (CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/${MT_INSTALL_SHLIB_DIR})
#set (CMAKE_BUILD_WITH_INSTALL_RPATH ON)
# CMAKE_SKIP_BUILD_RPATH : avoir le rpath dans l'arborescence de developpement
set (CMAKE_SKIP_BUILD_RPATH OFF)
set (CMAKE_SKIP_RPATH OFF)

set (CMAKE_SKIP_INSTALL_RPATH OFF)
#set (CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)

# ATTENTION : enleve le build tree du rpath :
#set (CMAKE_BUILD_WITH_INSTALL_RPATH ON)
