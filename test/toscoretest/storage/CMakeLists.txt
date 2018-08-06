PROJECT(storage_test)         
   
SET(SRC_LIST 
            DBTest.cpp
        )

INCLUDE_DIRECTORIES("../../toscore")

add_compile_options(-Wno-unused-parameter)

ADD_EXECUTABLE(storage_test ${SRC_LIST} )


target_link_libraries(storage_test PUBLIC tos-buildinfo leveldb Boost::filesystem Boost::system Boost::log Boost::thread Threads::Threads PRIVATE ethash::ethash Secp256k1)






