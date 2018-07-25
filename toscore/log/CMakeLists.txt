file(GLOB sources "*.cpp")
file(GLOB headers "*.h")
add_library(log ${sources} ${headers})

