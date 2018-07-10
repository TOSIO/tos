file(GLOB sources "*.cpp")
file(GLOB headers "*.h")
add_library(common ${sources} ${headers})

target_link_libraries(common PUBLIC aleth-buildinfo Boost::filesystem Boost::system Boost::log Boost::thread Threads::Threads PRIVATE ethash::ethash)




