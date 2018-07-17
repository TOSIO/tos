PROJECT(transport)         
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#FILE(GLOB SOURCE "./addrdb.cpp")      
SET(SRC_LIST ./netaddress.cpp
            ./netbase.cpp
            ./addrman.cpp
            ./addrdb.cpp
            ./net.cpp
            ./deps/utilstrencodings.cpp
            ./deps/timedata.cpp
            ./deps/uint256.cpp
            ./deps/utiltime.cpp
            ./deps/random.cpp
            ./deps/crypto/chacha20.cpp
            ./deps/crypto/sha512.cpp
            ./deps/support/cleanse.cpp
            ./deps/support/lockedpool.cpp
        )

INCLUDE_DIRECTORIES("./")
INCLUDE_DIRECTORIES("./deps")
INCLUDE_DIRECTORIES("../../")

#INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}")
#INCLUDE_DIRECTORIES("/mnt/hgfs/vm-shared/secp256k1/include")
#LINK_DIRECTORIES("/mnt/hgfs/vm-shared/secp256k1/lib/lib")  
add_definitions(-std=c++11)
add_definitions(-DHAVE_CONFIG_H)
#link_libraries("libsecp256k1.a")
ADD_EXECUTABLE(hello ${SRC_LIST})
