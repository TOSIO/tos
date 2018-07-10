file(GLOB sources "*.cpp")
file(GLOB headers "*.h")
add_library(storage ${sources} ${headers})

target_link_libraries(storage PRIVATE ethash::ethash)

