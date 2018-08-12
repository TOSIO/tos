PROJECT(crypto_test)
# set(CMAKE_CXX_STANDARD 11)
# set(CMAKE_CXX_STANDARD_REQUIRED True)

# set(Boost_USE_STATIC_LIBS ON)
# set(Boost_USE_MULTITHREADED ON)
# set(CMAKE_CXX_FLAGS "-BOOST_ALL_DYN_LINK")

# FILE(GLOB SOURCE "../../toscore/crypto/Base64.cpp")
SET(SRC_LIST
            ./ReadJsonTest.cpp
            ./ArgManagerTest.cpp
            ./Concurrent_queue_Test.cpp
            ./FileSystem.cpp
            ./JsonUtilTest.cpp
            ./RangeMaskTest.cpp
            ./RLPTest.cpp
        )

#INCLUDE_DIRECTORIES("../../baseservice/transport/")
#INCLUDE_DIRECTORIES("../../baseservice/transport/deps")
# INCLUDE_DIRECTORIES("../../toscore/crypto")
INCLUDE_DIRECTORIES("../../toscore")
#INCLUDE_DIRECTORIES("../../build/deps/include")


#INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}")
#INCLUDE_DIRECTORIES("/mnt/hgfs/vm-shared/secp256k1/include")
#LINK_DIRECTORIES("/mnt/hgfs/vm-shared/secp256k1/lib/lib")




# add_definitions(-std=c++11)
# add_definitions(-DHAVE_CONFIG_H)
add_compile_options(-Wno-unused-parameter)
#link_libraries("libsecp256k1.a")

ADD_EXECUTABLE(crypto_test ${SRC_LIST} )
# include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(crypto_test PUBLIC toscore Boost::system Boost::filesystem  Boost::log  Boost::unit_test_framework Boost::program_options Secp256k1)

# set_target_properties(crypto_test PROPERTIES LINKER_LANGUAGE CXX)

