# based on: http://stackoverflow.com/questions/11813271/embed-resources-eg-shader-code-images-into-executable-library-with-cmake
#
# example:
# cmake -DTOS_RES_FILE=test.cmake -P resources.cmake
#
# where test.cmake is:
# 
# # BEGIN OF cmake.test
# 
# set(copydlls "copydlls.cmake")
# set(conf "configure.cmake")
# 
# # this three properties must be set!
#
# set(TOS_RESOURCE_NAME "EthResources")
# set(TOS_RESOURCE_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}")
# set(TOS_RESOURCES "copydlls" "conf")
#
# # END of cmake.test
#

# should define TOS_RESOURCES
include(${TOS_RES_FILE})

set(TOS_RESULT_DATA "")
set(TOS_RESULT_INIT "")

# resource is a name visible for cpp application 
foreach(resource ${TOS_RESOURCES})

	# filename is the name of file which will be used in app
	set(filename ${${resource}})

	# filedata is a file content
	file(READ ${filename} filedata HEX)

	# read full name of the file
	file(GLOB filename ${filename})

	# Convert hex data for C compatibility
	string(REGEX REPLACE "([0-9a-f][0-9a-f])" "0x\\1," filedata ${filedata})

	# append static variables to result variable
	set(TOS_RESULT_DATA "${TOS_RESULT_DATA}	static const unsigned char eth_${resource}[] = {\n	// ${filename}\n	${filedata}\n};\n")

	# append init resources
	set(TOS_RESULT_INIT "${TOS_RESULT_INIT}	m_resources[\"${resource}\"] = (char const*)eth_${resource};\n")
	set(TOS_RESULT_INIT "${TOS_RESULT_INIT}	m_sizes[\"${resource}\"]     = sizeof(eth_${resource});\n")

endforeach(resource)

set(TOS_DST_NAME "${TOS_RESOURCE_LOCATION}/${TOS_RESOURCE_NAME}")

configure_file("${CMAKE_CURRENT_LIST_DIR}/resource.hpp.in" "${TOS_DST_NAME}.hpp.tmp")

include("${CMAKE_CURRENT_LIST_DIR}/../EthUtils.cmake")
replace_if_different("${TOS_DST_NAME}.hpp.tmp" "${TOS_DST_NAME}.hpp")
