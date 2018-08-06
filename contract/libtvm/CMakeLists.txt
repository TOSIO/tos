
set(sources
    TVMC.cpp TVMC.h
    ExtVMFace.cpp ExtVMFace.h
    Instruction.cpp Instruction.h
    interpreter.h
    LegacyVM.cpp LegacyVM.h
    LegacyVMCalls.cpp
    LegacyVMOpt.cpp
    VMFace.h
    VMConfig.h
    VM.cpp VM.h
    VMCalls.cpp
    VMOpt.cpp
    VMSIMD.cpp
    VMValidate.cpp
    VMFactory.cpp VMFactory.h
)

include_directories(SYSTEM ${CMAKE_CURRENT_SOURCE_DIR})
add_library(tvm ${sources})

target_link_libraries(tvm PUBLIC toscore tvmc PRIVATE tos-buildinfo jsoncpp_lib_static Boost::program_options ${CMAKE_DL_LIBS})

if(EVM_OPTIMIZE)
    target_compile_definitions(tvm PRIVATE EVM_OPTIMIZE)
endif()

if(EVMJIT)
    target_link_libraries(tvm PRIVATE evmjit)
    target_compile_definitions(tvm PRIVATE ETH_EVMJIT)
endif()

if(HERA)
    target_link_libraries(tvm PRIVATE hera)
    target_compile_definitions(tvm PRIVATE ETH_HERA)
endif()
