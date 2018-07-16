
set(
    sources
    toscoretest/utils/RLPTest.cpp
)
add_compile_options(-Wno-unused-parameter)
add_executable(tostest ${sources})

project(tostest)

target_link_libraries(
    tostest
    PRIVATE toscore
)
include_directories("../toscore")
