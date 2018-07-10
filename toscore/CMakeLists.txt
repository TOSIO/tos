file(GLOB sources "*.cpp")
file(GLOB headers "*.h")
add_library(toscore ${sources} ${headers})

target_link_libraries(toscore PUBLIC aleth-buildinfo Boost::filesystem Boost::system Boost::log Boost::thread Threads::Threads PRIVATE ethash::ethash)

find_package(LevelDB)
target_include_directories(toscore SYSTEM PUBLIC ${LEVELDB_INCLUDE_DIRS})
target_link_libraries(toscore PRIVATE ${LEVELDB_LIBRARIES})

include_directories(./)
add_subdirectory(jsonspirit)

add_subdirectory(common)

add_subdirectory(log)

add_subdirectory(crypto)

add_subdirectory(storage)

add_subdirectory(utils)


target_link_libraries(toscore PRIVATE common log crypto storage utils)
