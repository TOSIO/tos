add_executable(tosvm main.cpp)

target_link_libraries(tosvm PRIVATE ethcore ethereum tvm ethashseal toscore Boost::program_options)

target_include_directories(tosvm PRIVATE ../utils )

install(TARGETS tosvm DESTINATION bin OPTIONAL)
