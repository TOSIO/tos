file(GLOB sources "*.cpp")
file(GLOB headers "*.h")
add_library(crypto ${sources} ${headers})

target_link_libraries(crypto PRIVATE ethash::ethash)
