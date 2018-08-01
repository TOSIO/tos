file(GLOB SOURCES "*.cpp")
file(GLOB HEADERS "*.h")

add_library(toscrypto ${SOURCES} ${HEADERS})
target_link_libraries(toscrypto PUBLIC toscore jsonspirit Secp256k1 PRIVATE libff::ff cryptopp-static libscrypt::scrypt)
