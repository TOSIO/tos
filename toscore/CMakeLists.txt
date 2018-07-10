file(GLOB sources "*.cpp")
file(GLOB headers "*.h")
add_library(devcore ${sources} ${headers})

target_link_libraries(devcore PUBLIC aleth-buildinfo Boost::filesystem Boost::system Boost::log Boost::thread Threads::Threads PRIVATE ethash::ethash)

find_package(LevelDB)
target_include_directories(devcore SYSTEM PUBLIC ${LEVELDB_INCLUDE_DIRS})
target_link_libraries(devcore PRIVATE ${LEVELDB_LIBRARIES})

include_directories(./)
add_subdirectory(jsonspirit)

add_subdirectory(common)

add_subdirectory(log)
