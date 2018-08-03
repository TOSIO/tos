include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/toscore)
include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/baseservice)
add_subdirectory(tvmc)
add_subdirectory(libtvm)

if (EVM_USE AND EVM_TEST)
    include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR}/test)
    add_subdirectory(test)
endif()


