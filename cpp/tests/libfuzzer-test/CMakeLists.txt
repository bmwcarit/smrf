find_package(LibFuzzer REQUIRED)
find_package(Threads)

add_executable(
    smrf-libfuzzer-test
    entrypoint.cpp
)

target_link_libraries(
    smrf-libfuzzer-test
    smrf
    LLVM::Fuzzer
    ${CMAKE_THREAD_LIBS_INIT}
)

AddClangFormat(smrf-libfuzzer-test)

