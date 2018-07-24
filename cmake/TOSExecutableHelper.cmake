#
# this function requires the following variables to be specified:
# TOS_VERSION
# PROJECT_NAME
# PROJECT_VERSION
# PROJECT_COPYRIGHT_YEAR
# PROJECT_VENDOR
# PROJECT_DOMAIN_SECOND
# PROJECT_DOMAIN_FIRST
# SRC_LIST
# HEADERS
#
# params:
# ICON
#

macro(eth_copy_dll EXECUTABLE DLL)
	# dlls must be unsubstitud list variable (without ${}) in format
	# optimized;path_to_dll.dll;debug;path_to_dlld.dll
	if(DEFINED MSVC)
		list(GET ${DLL} 1 DLL_RELEASE)
		list(GET ${DLL} 3 DLL_DEBUG)
		add_custom_command(TARGET ${EXECUTABLE}
			PRE_BUILD
			COMMAND ${CMAKE_COMMAND} ARGS
			-DDLL_RELEASE="${DLL_RELEASE}"
			-DDLL_DEBUG="${DLL_DEBUG}"
			-DCONF="$<CONFIGURATION>"
			-DDESTINATION="${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}"
			-P "${TOS_SCRIPTS_DIR}/copydlls.cmake"
		)
	endif()
endmacro()

macro(eth_copy_dlls EXECUTABLE)
	foreach(dll ${ARGN})
		eth_copy_dll(${EXECUTABLE} ${dll})
	endforeach(dll)
endmacro()

macro(jsonrpcstub_create EXECUTABLE SPEC SERVERNAME SERVERDIR SERVERFILENAME CLIENTNAME CLIENTDIR CLIENTFILENAME)
	if (TOS_JSON_RPC_STUB)
		add_custom_target(${SPEC}stub)
		add_custom_command(
		TARGET ${SPEC}stub
		POST_BUILD
		DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${SPEC}"
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
		COMMAND ${CMAKE_COMMAND} -DTOS_SPEC_PATH="${CMAKE_CURRENT_SOURCE_DIR}/${SPEC}" -DTOS_SOURCE_DIR="${CMAKE_SOURCE_DIR}" -DTOS_CMAKE_DIR="${TOS_CMAKE_DIR}"
			-DTOS_CLIENT_DIR="${CLIENTDIR}"
			-DTOS_CLIENT_NAME=${CLIENTNAME}
			-DTOS_CLIENT_FILENAME=${CLIENTFILENAME}
			-DTOS_SERVER_DIR="${SERVERDIR}"
			-DTOS_SERVER_NAME=${SERVERNAME}
			-DTOS_SERVER_FILENAME=${SERVERFILENAME}
			-DTOS_JSON_RPC_STUB="${TOS_JSON_RPC_STUB}"
			-P "${TOS_SCRIPTS_DIR}/jsonrpcstub.cmake"
			)
		add_dependencies(${EXECUTABLE} ${SPEC}stub)
	endif ()
endmacro()

