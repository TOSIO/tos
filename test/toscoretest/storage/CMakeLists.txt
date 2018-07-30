PROJECT(storage_test)         
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(CMAKE_CXX_FLAGS "-BOOST_ALL_DYN_LINK")   

#FILE(GLOB SOURCE "./addrdb.cpp")      
SET(SRC_LIST 
            ./DBTest.cpp
        )

INCLUDE_DIRECTORIES("../../toscore")
#INCLUDE_DIRECTORIES("../../baseservice/transport/deps")
#INCLUDE_DIRECTORIES("../../")
#INCLUDE_DIRECTORIES("../../toscore")
#INCLUDE_DIRECTORIES("../../build/deps/include")

#INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}")
#INCLUDE_DIRECTORIES("/mnt/hgfs/vm-shared/secp256k1/include")
#LINK_DIRECTORIES("/mnt/hgfs/vm-shared/secp256k1/lib/lib")  




add_definitions(-std=c++11)
# add_definitions(-DHAVE_CONFIG_H)
add_compile_options(-Wno-unused-parameter)
#link_libraries("libsecp256k1.a")

# find_package(LevelDB)
ADD_EXECUTABLE(storage_test ${SRC_LIST} )
# include_directories(${Boost_INCLUDE_DIRS})
# target_link_libraries(storage_test PUBLIC storage toscore  ${Boost_LIBRARIES} levedb PRIVATE ${LEVELDB_LIBRARIES})
target_include_directories(storage_test SYSTEM PUBLIC ${LEVELDB_INCLUDE_DIRS})
target_link_libraries(storage_test PRIVATE ${LEVELDB_LIBRARIES})
# set_target_properties(storage_test PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(storage_test PUBLIC tos-buildinfo Boost::filesystem Boost::system Boost::log Boost::thread Threads::Threads PRIVATE ethash::ethash Secp256k1)
# target_link_libraries(storage_test PUBLIC  storage toscore  ${Boost_LIBRARIES} )

# find_package(LevelDB)
# target_include_directories(storage_test SYSTEM PUBLIC ${LEVELDB_INCLUDE_DIRS})
# target_link_libraries(storage_test PRIVATE ${LEVELDB_LIBRARIES})

# find_package(LevelDB)
# target_include_directories(toscore SYSTEM PUBLIC ${LEVELDB_INCLUDE_DIRS})
# target_link_libraries(toscore PRIVATE ${LEVELDB_LIBRARIES})




