#
# Création d'un tar.gz version ouverte du projet
# Créé le fichier nom_projet-version.tar.gz dans le répertoire parent du projet.
#

# Requiert les variables cmake suivantes :
# PROJECT_NAME			: nom du projet
# PROJECT_VERSION		: version du projet
# EXPORTED_FILES_LIST	: liste de fichiers et répertoires à archiver

# IMPORTANT : EXPORTED_FILES_LIST doit être une liste et non une chaîne de caractères où chaque
# fichier/répertoire est séparé du précédant par un espace, car dans ce cas l'ensemble des
# fichiers/répertoires est considéré comme un seul et même argument par tar qui du coup ne
# trouve pas le fichier/répertoire considéré.

string (TOLOWER ${PROJECT_NAME} PACKAGE_NAME)
get_filename_component (PROJECT_PARENT_DIR "${CMAKE_SOURCE_DIR}" PATH)
set (SOURCES_PACKAGE_FILE_NAME "${PACKAGE_NAME}-${PROJECT_VERSION}.tar.gz")
message (STATUS "--> Archivage des fichiers et répertoires ${PROJECT_ROOT_DIR} dans le fichier ${PROJECT_PARENT_DIR}/${PACKAGE_NAME}-${PROJECT_VERSION}.tar.gz ...")
set (EXCLUDED_PATTERNS_OPTIONS "")
if (EXCLUDED_EXPORT_PATTERN)
	foreach (PATTERN IN LISTS EXCLUDED_EXPORT_PATTERN)
		list (APPEND EXCLUDED_PATTERNS_OPTIONS "-exclude" ${PATTERN})
	endforeach (PATTERN IN LISTS EXCLUDED_EXPORT_PATTERN)
endif (EXCLUDED_EXPORT_PATTERN)
execute_process (COMMAND expurge -expurge -r -in ${PROJECT_ROOT_DIR} -out ${CMAKE_BINARY_DIR}/${PROJECT_NAME} ${EXCLUDED_PATTERNS_OPTIONS} WORKING_DIRECTORY ${PROJECT_PARENT_DIR})
execute_process (COMMAND tar cvfz ${SOURCES_PACKAGE_FILE_NAME} ${PROJECT_NAME} WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
execute_process (COMMAND mv ${CMAKE_BINARY_DIR}/${PACKAGE_NAME}-${PROJECT_VERSION}.tar.gz ${PROJECT_PARENT_DIR})
message (STATUS "--> Archive ouverte ${PROJECT_PARENT_DIR}/${PACKAGE_NAME}-${PROJECT_VERSION}.tar.gz créée.")

