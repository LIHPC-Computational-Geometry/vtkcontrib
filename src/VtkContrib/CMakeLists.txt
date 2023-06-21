set (CURRENT_PACKAGE_NAME "VtkContrib")
set (CURRENT_PACKAGE_MACRO_NAME "VTK_CONTRIB")

find_package (GUIToolkitsVariables)
find_package (OpenGL)	# Pour Spack 0.18.1

include (${CMAKE_SOURCE_DIR}/cmake/version.cmake)
include (${GUIToolkitsVariables_CMAKE_DIR}/common.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/vtkcontrib_common.cmake)
include (${GUIToolkitsVariables_CMAKE_DIR}/workarounds.cmake)

# Curieusement les dépendances semblent inexister avec cette version de VTK, ou ne sont pas bien prises en compte ...
# => On ajoute MPI
if (VTK_7)
	if (MPI_mpi_LIBRARY)
		set (VTK_PARALLEL_PACKAGES vtkParallelMPI vtkRenderingParallel vtkParallelCore)
	endif (MPI_mpi_LIBRARY)
	if (USE_OPENGL_BACKEND)
		set (VTK_REQUIRED_PACKAGES vtkRenderingLOD vtkInteractionStyle vtkFiltersHybrid vtkFiltersExtraction vtkFiltersCore vtkCommonTransforms vtkCommonMath vtkInteractionWidgets vtkImagingHybrid vtkRenderingOpenGL vtkRenderingFreeType vtkRenderingLabel vtkFiltersImaging vtkRenderingGL2PS vtkIOImage vtkIOLegacy vtkIOExportOpenGL vtkIOExport vtkIOXML vtkIOCore vtkCommonExecutionModel vtkCommonDataModel vtkRenderingAnnotation vtkFiltersSources vtkFiltersGeometry vtkFiltersGeneral vtkRenderingCore vtkCommonSystem vtkCommonCore vtkgl2ps vtkpng vtksys)
		set (VTK_OPENGL_BACKEND OpenGL)
	else (USE_OPENGL_BACKEND)
		set (VTK_REQUIRED_PACKAGES vtkRenderingLOD vtkInteractionStyle vtkFiltersHybrid vtkFiltersExtraction vtkFiltersCore #
	                           vtkCommonTransforms vtkCommonMath vtkInteractionWidgets #
	                           vtkImagingHybrid vtkRenderingOpenGL2 vtkRenderingFreeType vtkRenderingLabel #
	                           vtkFiltersImaging vtkRenderingGL2PSOpenGL2 vtkIOImage vtkIOLegacy vtkIOExportOpenGL2 vtkIOExport #
	                           vtkIOXML vtkIOCore vtkCommonExecutionModel vtkCommonDataModel vtkRenderingAnnotation vtkFiltersSources #
	                           vtkFiltersGeometry vtkFiltersGeneral vtkRenderingCore vtkCommonCore vtkgl2ps vtkpng vtksys #
	                           vtkCommonColor vtkFiltersGeometry vtkFiltersGeneral vtkFiltersCore  vtkCommonComputationalGeometry #
	                           vtkCommonSystem vtkCommonMisc vtkFiltersHybrid  vtkFiltersModeling vtkImagingGeneral #
	                           vtkRenderingVolume vtkFiltersGeneral vtkImagingCore vtkCommonSystem vtkfreetype vtkglew)
		set (VTK_OPENGL_BACKEND OpenGL2)
	endif (USE_OPENGL_BACKEND)
	find_package (VTK 7.1 REQUIRED COMPONENTS ${VTK_REQUIRED_PACKAGES})
	set (VTK_LIB_DEPENDENCIES ${VTK_PARALLEL_PACKAGES} ${VTK_REQUIRED_PACKAGES})
	set (VTK_INC_DEPENDENCIES ${VTK_INCLUDE_DIRS})
	# Suite pb sur une plateforme :
	if (MPI_mpi_LIBRARY)
		set (VTK_LIB_DEPENDENCIES "${VTK_LIB_DEPENDENCIES};${MPI_mpi_LIBRARY}")
	endif (MPI_mpi_LIBRARY)	# if (MPI_mpi_LIBRARY)
elseif (VTK_8)
	if (MPI_mpi_LIBRARY)
		set (VTK_PARALLEL_PACKAGES vtkParallelCore vtkRenderingParallel)
	endif (MPI_mpi_LIBRARY)
	set (VTK_REQUIRED_PACKAGES vtkIOExport vtkIOImage vtkInteractionWidgets vtkRenderingOpenGL2)
	find_package (VTK 8 REQUIRED COMPONENTS ${VTK_PARALLEL_PACKAGES} ${VTK_REQUIRED_PACKAGES})
	include(${VTK_USE_FILE})	# From Examples/GUI/Qt/ImageViewer/CMakeLists.txt
	set (VTK_LIB_DEPENDENCIES ${VTK_LIBRARIES})
	set (VTK_INC_DEPENDENCIES)	# Empty, not usefull with VTK 8
	set (VTK_OPENGL_BACKEND OpenGL2)
elseif (VTK_9)
	message (STATUS "========================================= UTILISATION DE VTK 9 =========================================")
	if (MPI_mpi_LIBRARY)
		set (VTK_PARALLEL_PACKAGES ParallelMPI IOParallel ParallelCore)
	endif (MPI_mpi_LIBRARY)
	set (VTK_REQUIRED_PACKAGES  ImagingHybrid ImagingCore IOExport IOImage InteractionWidgets RenderingOpenGL2)
	find_package (VTK 9 REQUIRED COMPONENTS ${VTK_PARALLEL_PACKAGES} ${VTK_REQUIRED_PACKAGES})
	set (VTK_LIB_DEPENDENCIES ${VTK_LIBRARIES})
	set (VTK_INC_DEPENDENCIES)	# Empty, not usefull with VTK 9
	set (VTK_OPENGL_BACKEND OpenGL2)
	if (MPI_mpi_LIBRARY)
		set (VTK_LIB_DEPENDENCIES "${VTK_LIB_DEPENDENCIES};${MPI_mpi_LIBRARY}")
	endif (MPI_mpi_LIBRARY)	# if (MPI_mpi_LIBRARY)
else (VTK_9)
	message (FATAL_ERROR "==> UTILISEZ -DVTK_7:BOOL=ON OU -DVTK_8:BOOL=ON OU -DVTK_9:BOOL=ON A LA LIGNE DE COMMANDE")
endif (VTK_7)	

find_package (MPI)

file (GLOB HEADERS public/${CURRENT_PACKAGE_NAME}/*.h)
file (GLOB CPP_SOURCES *.cpp)

# On ajoute les en-têtes aux sources. C'est utile pour cmake dans certains cas,
# par exemple lorsqu'ils doivent être pré-processés (moc, ...).
add_library (VtkContrib ${CPP_SOURCES} ${HEADERS})
set (ALL_TARGETS VtkContrib)
set_property (TARGET VtkContrib PROPERTY VERSION ${VTK_CONTRIB_VERSION})
set_property (TARGET VtkContrib PROPERTY SOVERSION ${VTK_CONTRIB_MAJOR_VERSION})
if (VTK_9)
	vtk_module_autoinit (TARGETS VtkContrib MODULES ${VTK_LIBRARIES})
endif (VTK_9)

# Rem : en Qt v 4.* on utilise -DUSE_QT_WEBKIT
set (VTK_CONTRIB_PUBLIC_FLAGS "${MANDATORY_CXX_OPTIONS} -DVTK_OPENGL_BACKEND=\"${VTK_OPENGL_BACKEND}\"" "${MANDATORY_VTK_OPTIONS}")
set (VTK_CONTRIB_PRIVATE_FLAGS -DVTK_CONTRIB_VERSION="${VTK_CONTRIB_VERSION}" "${MANDATORY_PRIVATE_VTK_OPTIONS}")

target_include_directories (VtkContrib PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/public>$<INSTALL_INTERFACE:${CMAKE_INSTALL_PREFIX}/include>)
target_compile_definitions (VtkContrib PUBLIC ${VTK_CONTRIB_PUBLIC_FLAGS})
target_compile_definitions (VtkContrib PRIVATE ${VTK_CONTRIB_PRIVATE_FLAGS})
target_compile_options (VtkContrib PRIVATE ${SHARED_CFLAGS})	# Requested by Qt ...
target_include_directories (VtkContrib PUBLIC ${VTK_INC_DEPENDENCIES} ${VTK_MPI_INC_DIR})
if (VTK_MPI_CFLAGS)
	target_compile_options (VtkContrib PUBLIC ${VTK_MPI_CFLAGS})
endif (VTK_MPI_CFLAGS)
if (VTK_MPI_LDFLAGS)
	target_link_options (VtkContrib PUBLIC ${VTK_MPI_LDFLAGS})
endif (VTK_MPI_LDFLAGS)
target_link_libraries (VtkContrib PUBLIC ${VTK_LIB_DEPENDENCIES} ${VTK_MPI_LIBS})
target_link_libraries (VtkContrib PUBLIC OpenGL::GL)	# Pour Spack 0.18.1


# ============================================= ATTENTION : ZONE SENSIBLE ==============================================
# On gère ici toute la misère du monde ...
if (NOT ${PLATFORM} STREQUAL "Ubuntu")
	if (VTK_MPI_LIB_DIR)
		target_link_directories (VtkContrib PUBLIC "${VTK_MPI_LIB_DIR}")	# Probablement à revoir par la suite car entraîné par VTK
	endif (VTK_MPI_LIB_DIR)
endif (NOT ${PLATFORM} STREQUAL "Ubuntu")
# ============================================= !ATTENTION : ZONE SENSIBLE ==============================================

# Etre capable une fois installée de retrouver vtk*, ... :
# (Rem : en son absence on a Set runtime path of "/tmp/pignerol/install/lib/libVtkContrib.so.5.0.0" to "") ...
set_target_properties (VtkContrib PROPERTIES INSTALL_RPATH_USE_LINK_PATH 1)
# Inefficace ici : CMAKE_AUTOMOC => dans le CMakeLists.txt racine
#set (CMAKE_AUTOMOC ON)

# INSTALLATION :
include(CMakePackageConfigHelpers)
# Génération du fichier contenant les informations sur la distribution :
configure_file (cmake/VtkContribDistribution.h.in  ${CMAKE_CURRENT_BINARY_DIR}/VtkContribDistribution.tmp)
configure_file (${CMAKE_CURRENT_BINARY_DIR}/VtkContribDistribution.tmp  ${CMAKE_CURRENT_BINARY_DIR}/VtkContribDistribution.h)

# ConfigPackageLocation : c'est plus ou moins standardisé, le défaut étant lib/cmake.
# On nos recommande très vivement d'utiliser ce répertoire afin de limiter les soucis par la suite,
# notamment au niveau des vues.
set (ConfigPackageLocation ${CMAKE_CMAKE_DIR})

# La génération des règles d'installation :
install (TARGETS ${ALL_TARGETS} EXPORT ${CMAKE_PACKAGE_TARGETS_NAME}
         ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
         LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
         RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
         INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
         
# Recopie des en-têtes dans le répertoire d'installation :
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CURRENT_PACKAGE_NAME})
# Dont VtkContribDistribution.h généré plus haut :
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/VtkContribDistribution.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CURRENT_PACKAGE_NAME})

# Le fichier contenant les informations de version de ce module :
write_basic_package_version_file(${CMAKE_PACKAGE_VERSION_FILE} VERSION ${VTK_CONTRIB_VERSION} COMPATIBILITY SameMajorVersion)
export (EXPORT ${CMAKE_PACKAGE_TARGETS_NAME} FILE ${CMAKE_PACKAGE_TARGETS_FILE} NAMESPACE ${CMAKE_MODULE_NAMESPACE})

# Utilisation par d'autres modules de VtkContrib :
set (INCLUDE_INSTALL_DIR include/)		# VtkContrib_INCLUDE_DIR avec VtkContribConfig.cmake.in
set (LIB_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR})	# VtkContrib_LIB_DIR avec VtkContribConfig.cmake.in
#set (RUNTIME_INSTALL_DIR bin/)		# VtkContrib_RUNTIME_DIR avec VtkContribConfig.cmake.in
# Création du fichier VtkContribConfig.cmake qui définira VtkContrib_INCLUDE_DIR, VtkContrib_LIB_DIR et VtkContrib_BIN_DIR :
configure_package_config_file (cmake/${CMAKE_PACKAGE_CONFIG_NAME}.cmake.in ${CMAKE_PACKAGE_CONFIG_FILE}
	INSTALL_DESTINATION ${CMAKE_CMAKE_DIR}
	PATH_VARS INCLUDE_INSTALL_DIR LIB_INSTALL_DIR PLATFORM)

# Copie du fichier VtkContribTargets.cmake dans prefix/lib/VtkContrib/cmake :
install (EXPORT ${CMAKE_PACKAGE_TARGETS_NAME}
	FILE ${CMAKE_PACKAGE_TARGETS_BASENAME} NAMESPACE ${CMAKE_MODULE_NAMESPACE}
	DESTINATION ${ConfigPackageLocation} PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ)
# Copie des fichiers VtkContribConfig.cmake et VtkContribConfigVersion.cmake dans prefix/lib/VtkContrib/cmake :
install (FILES ${CMAKE_PACKAGE_CONFIG_FILE} ${CMAKE_PACKAGE_VERSION_FILE} DESTINATION ${ConfigPackageLocation} PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ)

