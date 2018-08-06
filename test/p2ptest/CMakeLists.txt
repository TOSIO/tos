PROJECT(transport_test)         
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(CMAKE_CXX_FLAGS "-BOOST_ALL_DYN_LINK")   

#FILE(GLOB SOURCE "./addrdb.cpp")      
SET(SRC_LIST 
            ./netaddress_tests.cpp
            ./addrman_tests.cpp
            ./limitedmap_tests.cpp
            ./netbase_tests.cpp
            ./random_tests.cpp
            ./uint256_tests.cpp
            ./net_tests.cpp
        )

INCLUDE_DIRECTORIES("../../baseservice/transport/")
INCLUDE_DIRECTORIES("../../baseservice/transport/deps")
#INCLUDE_DIRECTORIES("../../")
INCLUDE_DIRECTORIES("../../toscore")
INCLUDE_DIRECTORIES("../../build/deps/include")

#INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}")
#INCLUDE_DIRECTORIES("/mnt/hgfs/vm-shared/secp256k1/include")
#LINK_DIRECTORIES("/mnt/hgfs/vm-shared/secp256k1/lib/lib")  




add_definitions(-std=c++11)
add_definitions(-DHAVE_CONFIG_H)
add_compile_options(-Wno-unused-parameter)
#link_libraries("libsecp256k1.a")

ADD_EXECUTABLE(transport_test ${SRC_LIST} )
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(transport_test PUBLIC  transport toscore  ${Boost_LIBRARIES} Boost::unit_test_framework)

set_target_properties(transport_test PROPERTIES LINKER_LANGUAGE CXX)

