# EVMC -- Ethereum Client-VM Connector API
# Copyright 2018 Pawel Bylica.
# Licensed under the MIT License. See the LICENSE file.

include(GNUInstallDirs)

add_compile_options(-fvisibility=default)

add_library(tvmc-examplevm examplevm.c)
add_library(tvmc-examplevm-so SHARED examplevm.c)
target_link_libraries(tvmc-examplevm PRIVATE tvmc)
target_link_libraries(tvmc-examplevm-so PRIVATE tvmc)
set_target_properties(tvmc-examplevm PROPERTIES DEBUG_POSTFIX "")

install(TARGETS tvmc-examplevm
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
