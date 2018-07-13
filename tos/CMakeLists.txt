set(
    sources
    main.cpp
)
add_compile_options(-Wno-unused-parameter)
add_executable(tos ${sources})
project(tos)

target_link_libraries(
    tos
    PRIVATE toscore
)
include_directories("../toscore")

#install(TARGETS tos DESTINATION bin)

