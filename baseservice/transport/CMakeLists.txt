PROJECT(transport)         
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#FILE(GLOB SOURCE "./addrdb.cpp")      
SET(SRC_LIST 
            ./netaddress.cpp
            ./netbase.cpp
            ./addrman.cpp
            ./addrdb.cpp
            ./net.cpp
            ./argsproxy.cpp
            ./protocol.cpp
            ./timedata.cpp
            ./ui_interface.cpp
            ./deps/log.cpp
            ./deps/uint256.cpp
            ./deps/sync.cpp
            ./deps/threadinterrupt.cpp
            ./deps/threadtrace.cpp
            ./deps/utilstrencodings.cpp
            ./deps/arith_uint256.cpp
            ./deps/utiltime.cpp
            ./deps/random.cpp
            ./deps/scheduler.cpp
            ./deps/util.cpp
            ./deps/crypto/chacha20.cpp
            ./deps/crypto/sha512.cpp
            ./deps/crypto/sha256.cpp
            ./deps/crypto/sha256_sse4.cpp
            ./deps/hash.cpp
            ./deps/support/cleanse.cpp
            ./deps/support/lockedpool.cpp
        )

INCLUDE_DIRECTORIES("./")
INCLUDE_DIRECTORIES("./deps")
INCLUDE_DIRECTORIES("../../")
INCLUDE_DIRECTORIES("../../toscore")
INCLUDE_DIRECTORIES("../../build/deps/include")

#INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}")
#INCLUDE_DIRECTORIES("/mnt/hgfs/vm-shared/secp256k1/include")
#LINK_DIRECTORIES("/mnt/hgfs/vm-shared/secp256k1/lib/lib")  
add_definitions(-std=c++11)
add_definitions(-DHAVE_CONFIG_H)
add_compile_options(-Wno-unused-parameter -Wno-implicit-fallthrough)

#link_libraries("libsecp256k1.a")


ADD_LIBRARY(transport ${SRC_LIST} )
target_link_libraries(transport PUBLIC toscore ${Boost_LIBRARIES}  ssl crypto Secp256k1 Boost::system Boost::date_time Boost::chrono Boost::filesystem  Boost::log Boost::thread Boost::program_options Boost::unit_test_framework)

set_target_properties(transport PROPERTIES LINKER_LANGUAGE CXX)

