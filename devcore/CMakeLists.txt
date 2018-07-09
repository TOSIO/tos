file(GLOB sources "*.cpp")
file(GLOB headers "*.h")
add_library(devcore ${sources} ${headers})

#target_include_directories(devcore SYSTEM PUBLIC ${Boost_INCLUDE_DIR} PRIVATE ./json_spirit)

#target_link_libraries(devcore PUBLIC libjsonspirit)
target_link_libraries(devcore PUBLIC aleth-buildinfo Boost::filesystem Boost::system Boost::log Boost::thread Threads::Threads PRIVATE ethash::ethash)

find_package(LevelDB)
target_include_directories(devcore SYSTEM PUBLIC ${LEVELDB_INCLUDE_DIRS})
target_link_libraries(devcore PRIVATE ${LEVELDB_LIBRARIES})

include_directories(./)
add_subdirectory(json_spirit)
