

#target_link_libraries(hello ${Boost_LIBRARIES})


#find_package (Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
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

