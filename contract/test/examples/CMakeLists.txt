
add_subdirectory(examplevm)
add_executable(example-capi capi.c)
target_link_libraries(example-capi PRIVATE tvmc tvmc-examplevm)
if(NOT MSVC)
    target_compile_options(example-capi PRIVATE -Wno-extra)
endif()
file(COPY run.sh DESTINATION ./)
