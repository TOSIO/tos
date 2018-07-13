


project(tostest)

target_link_libraries(
    tostest
    PRIVATE toscore
)
include_directories("../toscore")
