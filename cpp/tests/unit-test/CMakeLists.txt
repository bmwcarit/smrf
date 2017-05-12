AddTest(
    smrf-unit-test
    ../test-main.cpp
    ../RoundtripMessage.h
    CompressionTest.cpp
    MessagePrefixTest.cpp
    MessageDeserializerTest.cpp
    RoundtripTest.cpp
)
target_link_libraries(smrf-unit-test smrf)
AddClangFormat(smrf-unit-test)
