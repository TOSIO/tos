file(GLOB sources "*.cpp")
file(GLOB headers "*.h")
add_library(storage ${sources} ${headers})

