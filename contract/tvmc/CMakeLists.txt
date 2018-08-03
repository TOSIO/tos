# EVMC: Ethereum Client-VM Connector API.
# Copyright 2018 Pawel Bylica.
# Licensed under the MIT License. See the LICENSE file.

cmake_minimum_required(VERSION 3.5)

if(TARGET tvmc)
    # The tvmc library has been already created (probably by other submodule).
    return()
endif()

option(TVMC_BUILD_TESTS "Build TVMC tests and test tools" OFF)
option(TVMC_BUILD_EXAMPLES "Build TVMC examples" OFF)
option(HUNTER_ENABLED "Enable Hunter package manager support" "${TVMC_BUILD_TESTS}")

include(cmake/cable/bootstrap.cmake)
include(CableCompilerSettings)
include(HunterGate)
include(GNUInstallDirs)
include(defaults/HunterCacheServers)

# Setup Hunter.
# Hunter is going to be initialized only if building with tests,
# where it is needed to get dependencies.
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.20.37.tar.gz"
    SHA1 "51886d10428c924cc21756abc17623bcf4986386"
)

project(tvmc)
set(PROJECT_VERSION "0.1.0.dev0")

cable_configure_compiler(NO_STACK_PROTECTION)

add_library(tvmc INTERFACE)
target_include_directories(tvmc INTERFACE include)

install(DIRECTORY include/tvmc DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

if(TVMC_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

if(TVMC_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(WIN32)
    set(CPACK_GENERATOR ZIP)
else()
    set(CPACK_GENERATOR TGZ)
endif()
string(TOLOWER ${CMAKE_SYSTEM_NAME} system_name)
set(CPACK_PACKAGE_FILE_NAME ${PROJECT_NAME}-${PROJECT_VERSION}-${system_name})
set(CPACK_PACKAGE_CHECKSUM SHA256)
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY FALSE)
include(CPack)
