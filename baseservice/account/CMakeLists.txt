file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.h")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/toscrypto")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/toscore")
include_directories(SYSTEM "${CMAKE_SOURCE_DIR}/baseservice")

add_library(tosaccount ${SOURCES} ${HEADERS})

#include_directories("../toscore")

target_link_libraries(tosaccount PUBLIC toscore jsonspirit Secp256k1 PRIVATE libff::ff cryptopp-static libscrypt::scrypt)
