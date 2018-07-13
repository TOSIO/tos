project(toscore)
file(GLOB sources "*.cpp")
file(GLOB headers "*.h")

file(GLOB commons "common/*.cpp")
file(GLOB commonh "common/*.h")

file(GLOB logs "log/*.cpp")
file(GLOB logh "log/*.h")

file(GLOB cryptos "crypto/*.cpp")
file(GLOB cryptoh "crypto/*.h")

file(GLOB storages "storage/*.cpp")
file(GLOB storageh "storage/*.h")

file(GLOB utilss "utils/*.cpp")
file(GLOB utilsh "utils/*.h")

file(GLOB concurrents "concurrent/*.cpp")
file(GLOB concurrenth "concurrent/*.h")


add_library(toscore ${sources} ${headers} ${commons} ${commonh} ${logs} ${logh} ${cryptos} ${cryptoh} ${storages} ${storageh} ${utilss} ${utilsh} ${concurrents} ${concurrenth})

target_link_libraries(toscore PUBLIC aleth-buildinfo Boost::filesystem Boost::system Boost::log Boost::thread Threads::Threads PRIVATE ethash::ethash)

find_package(LevelDB)
target_include_directories(toscore SYSTEM PUBLIC ${LEVELDB_INCLUDE_DIRS})
target_link_libraries(toscore PRIVATE ${LEVELDB_LIBRARIES})

include_directories(./)
add_subdirectory(jsonspirit)

target_link_libraries(toscore PUBLIC jsonspirit)

