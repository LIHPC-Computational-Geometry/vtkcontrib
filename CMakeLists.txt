cmake_minimum_required(VERSION 3.12)

# Rem : langage C ajouté pour VTK 9 car sinon il refuse de charger MPI_C demandé par ParallelMPI ...
project (VtkContrib CXX C)

include (GNUInstallDirs)
include (cmake/version.cmake)
include (cmake/common.cmake)

set (VtkContrib_VERSION ${VTK_CONTRIB_VERSION})

# Partie normale de cmake : réalisation de Makefiles ...
	
# Ici et pas ailleurs : CMAKE_VERBOSE_MAKEFILE
set (CMAKE_VERBOSE_MAKEFILE ON)
option (BUILD_SHARED_LIBS "Creation de bibliotheques dynamiques (defaut : ON)" ON)

add_subdirectory (src)
