AddTest(
    smrf-unit-test
    ../test-main.cpp
    CompressionTest.cpp
)
target_link_libraries(smrf-unit-test smrf)
AddClangFormat(smrf-unit-test)
