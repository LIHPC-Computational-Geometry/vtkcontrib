include (${CMAKE_SOURCE_DIR}/cmake/version.cmake)

find_package (GUIToolkitsVariables)

include (${GUIToolkitsVariables_CMAKE_DIR}/common.cmake)
include (${GUIToolkitsVariables_CMAKE_DIR}/workarounds.cmake)

set (ALL_EXECUTABLES areapicker point_widget point_widget2 trihedrons torus)

foreach (exe ${ALL_EXECUTABLES}) 
	add_executable (${exe} ${exe}.cpp)
	target_include_directories (${exe} PRIVATE ../VtkContrib/public ${CMAKE_CURRENT_SOURCE_DIR})
	target_compile_options (${exe} PRIVATE ${SHARED_CFLAGS})
	target_link_libraries (${exe} PUBLIC VtkContrib)
	# INSTALL_RPATH modifie le rpath pour les libs internes au projet :
	set_target_properties (${exe} PROPERTIES INSTALL_RPATH_USE_LINK_PATH 1 INSTALL_RPATH ${CMAKE_PACKAGE_RPATH_DIR})
endforeach (exe)
   

