file(GLOB sources "*.cpp")
file(GLOB headers "*.h")
add_compile_options(-Wreturn-type -Werror)
add_library(tosbase ${sources} ${headers})

target_link_libraries(tosbase PUBLIC aleth-buildinfo Boost::filesystem Boost::system Boost::log Boost::thread Threads::Threads PRIVATE ethash::ethash)




