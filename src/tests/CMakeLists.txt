include (${CMAKE_SOURCE_DIR}/cmake/version.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/common.cmake)
include (${CMAKE_SOURCE_DIR}/cmake/workarounds.cmake)

add_executable (point_widget point_widget.cpp)
add_executable (trihedrons trihedrons.cpp)

target_include_directories (point_widget PRIVATE ../VtkContrib/public ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options (point_widget PRIVATE ${SHARED_CFLAGS}) # Requested by Qt ...
target_link_libraries (point_widget PUBLIC VtkContrib)
target_include_directories (trihedrons PRIVATE ../VtkContrib/public ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options (trihedrons PRIVATE ${SHARED_CFLAGS}) # Requested by Qt ...
target_link_libraries (trihedrons PUBLIC VtkContrib)

# INSTALL_RPATH modifie le rpath pour les libs internes au projet :
set_target_properties (point_widget PROPERTIES INSTALL_RPATH_USE_LINK_PATH 1 INSTALL_RPATH ${CMAKE_PACKAGE_RPATH_DIR})
set_target_properties (trihedrons PROPERTIES INSTALL_RPATH_USE_LINK_PATH 1 INSTALL_RPATH ${CMAKE_PACKAGE_RPATH_DIR})
