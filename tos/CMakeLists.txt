set(
    sources
    main.cpp
    args_manager.cpp
)
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)

set(CMAKE_CXX_FLAGS "-BOOST_ALL_DYN_LINK")   
# ADD_DEFINITIONS(-DBoost_USE_STATIC_LIBS=ON)
# ADD_DEFINITIONS(-DBOOST_ALL_DYN_LIN)
# add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)
add_compile_options(-Wno-unused-parameter)
add_executable(tos ${sources})
project(tos)
# find_package(Boost::log::setupConfig)
# FIND_PACKAGE(Boost)
# IF (Boost_FOUND)
#     INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
#     ADD_DEFINITIONS( "-DHAS_BOOST" )
# ENDIF()

# include_directories(${Boost_INCLUDE_DIRS})
# LINK_DIRECTORIES(${Boost_LIBRARY_DIR})
include_directories("../toscore")

include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(tos PUBLIC toscore ${Boost_LIBRARIES} Boost::system Boost::filesystem  Boost::log Boost::thread Boost::program_options)




install(TARGETS tos DESTINATION bin)

