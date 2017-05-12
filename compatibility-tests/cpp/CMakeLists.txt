cmake_minimum_required(VERSION 3.2)
project(compatibility-test)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

set(SMRF_CPP_DIR "${CMAKE_CURRENT_LIST_DIR}/../../cpp/")
list(APPEND CMAKE_MODULE_PATH "${SMRF_CPP_DIR}/CMake")

find_package(smrf REQUIRED)
include(AddGtestGmock)
include(AddClangFormat)

find_package(Boost 1.58.0 REQUIRED
    COMPONENTS
    filesystem
    program_options
)

function(ConfigureTest TARGET)
    target_include_directories(
        ${TARGET}
        SYSTEM
        PRIVATE
        ${SMRF_CPP_DIR}
    )

    target_link_libraries(
        ${TARGET}
        Boost::program_options
        smrf::smrf
    )
    AddClangFormat(${TARGET})
endfunction(ConfigureTest)


AddTest(
    serializer
    serializer.cpp
    Options.h
)
ConfigureTest(serializer)

AddTest(
    deserializer
    deserializer.cpp
    Options.h
)
ConfigureTest(deserializer)
