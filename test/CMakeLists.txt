set(–DBOOST_build_info=yes)
set(–DBOOST_log_level=all)


include_directories (
                     ${Boost_INCLUDE_DIRS}
                     )
add_definitions (-DBOOST_TEST_DYN_LINK)
enable_testing()
add_executable (tostest toscoretest/utils/RLPTest.cpp)
target_link_libraries (tostest PRIVATE
toscore 
                       ${Boost_FILESYSTEM_LIBRARY}
                       ${Boost_SYSTEM_LIBRARY}
                       ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} 
                       )

include_directories("../toscore")

include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(tostest PUBLIC toscore ${Boost_LIBRARIES} Boost::system Boost::filesystem  Boost::log Boost::thread Boost::program_options)
   
add_subdirectory(p2ptest)
# add_subdirectory(toscoretest/storage)
