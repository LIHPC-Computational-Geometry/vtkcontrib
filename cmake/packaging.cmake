#
# Création d'un tar.gz du projet
# Créé le fichier nom_projet-version.tar.gz dans le répertoire parent du projet.
#

# Requiert les variables cmake suivantes :
# PROJECT_NAME			: nom du projet
# PROJECT_VERSION		: version du projet
# ARCHIVED_FILES_LIST	: liste de fichiers et répertoires à archiver

# IMPORTANT : ARCHIVED_FILES_LIST doit être une liste et non une chaîne de caractère où chaque
# fichier/répertoire est séparé du précédant par un espace, car dans ce cas l'ensemble des
# fichiers/répertoires est considéré comme un seul et même arguement par tar qui du coup ne
# trouve pas le fichier/répertoire considéré.

string (TOLOWER ${PROJECT_NAME} PACKAGE_NAME)
get_filename_component (PROJECT_PARENT_DIR "${CMAKE_SOURCE_DIR}" PATH)
set (SOURCES_PACKAGE_FILE_NAME "${PACKAGE_NAME}-${PROJECT_VERSION}.tar.gz")
message (STATUS "--> Archivage des fichiers et répertoires ${ARCHIVED_FILES} dans le fichier ${PROJECT_PARENT_DIR}/${PACKAGE_NAME}-${PROJECT_VERSION}.tar.gz ...")
# Rem : les 2 commandes ci-dessous fonctionnent
#execute_process (COMMAND tar cvfz ${PROJECT_PARENT_DIR}/${SOURCES_PACKAGE_FILE_NAME} -C ${PROJECT_PARENT_DIR} ${ARCHIVED_FILES_LIST})
execute_process (COMMAND tar cvfz ${SOURCES_PACKAGE_FILE_NAME} ${ARCHIVED_FILES_LIST} WORKING_DIRECTORY ${PROJECT_PARENT_DIR})
message (STATUS "--> Archive ${PROJECT_PARENT_DIR}/${PACKAGE_NAME}-${PROJECT_VERSION}.tar.gz créée.")

