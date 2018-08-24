
cmake_minimum_required(VERSION 3.5.1)

include(cmake/cable/bootstrap.cmake)
include(CableBuildInfo)
include(CableBuildType)
include(CableToolchains)
include(GNUInstallDirs)

if(UNIX)
    # Build deps with PIC to allow building shared libraries.
    set(toolchain cxx11-pic)
else()
    set(toolchain cxx11)
endif()
cable_configure_toolchain(DEFAULT ${toolchain})

set(TOS_CMAKE_DIR "${CMAKE_CURRENT_LIST_DIR}/cmake" CACHE PATH "The path to the cmake directory")
list(APPEND CMAKE_MODULE_PATH ${TOS_CMAKE_DIR})

# Map current configuration to configurations of imported targets.
set(CMAKE_MAP_IMPORTED_CONFIG_DEBUG Release)
set(CMAKE_MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release)

set(HUNTER_CONFIGURATION_TYPES Release)
set(HUNTER_JOBS_NUMBER 4)
set(HUNTER_CACHE_SERVERS "https://github.com/TOSIO/hunter-cache")
set(HUNTER_PASSWORDS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Hunter/passwords.cmake)

# In CI builds upload the binaries if the GITHUB_USER_PASSWORD was decrypted
# (only for branches and internal PRs).
if("$ENV{CI}" AND NOT "$ENV{GITHUB_USER_PASSWORD}" STREQUAL "")
    set(run_upload YES)
else()
    set(run_upload NO)
endif()
option(HUNTER_RUN_UPLOAD "Upload binaries to the cache server" ${run_upload})

include(HunterGate)
HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.21.16.tar.gz"
    SHA1 "aa1faccd24f33dd2c368f85a4e9ca2e5d9c249cb"
    LOCAL
)

project(toschain)
set(PROJECT_VERSION 1.4.0.dev1)



cable_set_build_type(DEFAULT Debug CONFIGURATION_TYPES Debug;Release;RelWithDebInfo)

cable_add_buildinfo_library(PREFIX tos)

set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY On)

include(TOSCcache)
include(TOSOptions)
include(TOSCompilerSettings)
include(TOSExecutableHelper)
include(TOSDependencies)
include(TOSUtils)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
hunter_add_package(Boost COMPONENTS program_options filesystem system thread context fiber log date_time chrono test)
find_package(Boost CONFIG REQUIRED program_options filesystem system thread context fiber log  date_time chrono unit_test_framework) 

hunter_add_package(jsoncpp)
find_package(jsoncpp CONFIG REQUIRED)

hunter_add_package(Snappy)
find_package(Snappy CONFIG REQUIRED)

hunter_add_package(cryptopp)
find_package(cryptopp CONFIG REQUIRED)

hunter_add_package(libjson-rpc-cpp)
find_package(libjson-rpc-cpp CONFIG REQUIRED)

hunter_add_package(libscrypt)
find_package(libscrypt CONFIG REQUIRED)

hunter_add_package(ethash)
find_package(ethash CONFIG REQUIRED)

include(ProjectSecp256k1)
include(ProjectLibFF)

# Import binaryen library before hera to make it available also to testeth.
include(cmake/ProjectBinaryen.cmake)

find_package(Threads)

if(MINIUPNPC)
    find_package(Miniupnpc 1.8.2013 REQUIRED)
endif()


configure_project()


#Global include path for all libs.
include_directories("${CMAKE_SOURCE_DIR}")

# option(ALETH_INTERPRETER_SHARED "Build aleth-interpreter as a shared library" OFF)


add_subdirectory(toscore)

add_subdirectory(baseservice/transport)

add_subdirectory(tos)
add_subdirectory(baseservice/account)
add_subdirectory(baseservice/libethcore)

add_subdirectory(sdag)
add_subdirectory(toscrypto)
#add_subdirectory(baseservice/state)

#tvm configure begin
option(TVM_USE  "build libevm" OFF )
option(TVM_TEST "build ethvm test program" OFF )
message("------------------------------------------------------------- tvm begin")
message("-- TVM_USE                                                   ${TVM_USE}")
message("-- TVM_TEST                                                  ${TVM_TEST}")
message("------------------------------------------------------------- tvm end")
message("CMAKE_STATIC_LIBRARY_PREFIX:${CMAKE_STATIC_LIBRARY_PREFIX}")
if(TVM_USE)
  add_subdirectory(contract) 
  if(TVM_TEST)
    enable_testing()
  endif()		
endif()
#tvm configure end
add_subdirectory(test)