file(GLOB sources "*.cpp")
file(GLOB headers "*.h")
add_compile_options(-Wno-unused-parameter)
add_library(jsonspirit ${sources} ${headers})


