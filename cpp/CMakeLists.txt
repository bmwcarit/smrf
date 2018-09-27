set(
    SMRF_SOVERSION
    ${SMRF_VERSION_MAJOR}.${SMRF_VERSION_MINOR}
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/CMake")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

if(NOT CMAKE_BUILD_TYPE)
    message(STATUS "setting default value 'Debug' for CMAKE_BUILD_TYPE")
    set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Build type" FORCE)
endif()

set_property(
    CACHE
    CMAKE_BUILD_TYPE
    PROPERTY STRINGS
    "Debug"
    "Release"
)

if(NOT DEFINED BUILD_SHARED_LIBS)
    set(BUILD_SHARED_LIBS ON)
endif()

message(STATUS "option BUILD_SHARED_LIBS=" ${BUILD_SHARED_LIBS})

# defines GNU standard installation directories
include(GNUInstallDirs)
set(INSTALL_CMAKE_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/smrf CACHE PATH "Installation sub directory for CMake files")

# Set the binary directory
set(SMRF_BINARY_DIR ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})
message(STATUS "variable SMRF_BINARY_DIR=" ${SMRF_BINARY_DIR})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${SMRF_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${SMRF_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${SMRF_BINARY_DIR})

option(
    BUILD_TESTS
    "Build tests?"
    ON
)
message(STATUS "option BUILD_TESTS=" ${BUILD_TESTS})

option(
    BUILD_UNIT_TESTS
    "Build unit test?"
    ON
)
message(STATUS "option BUILD_UNIT_TESTS=" ${BUILD_UNIT_TESTS})

option(
    BUILD_LIBFUZZER_TEST
    "Build libfuzzer test?"
    OFF
)
message(STATUS "option BUILD_LIBFUZZER_TEST=" ${BUILD_LIBFUZZER_TEST})

if(BUILD_LIBFUZZER_TEST)
    if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        message(FATAL_ERROR "building with Clang is required for fuzzing")
    endif()
    set(FUZZER_FLAGS "-fsanitize=address -fsanitize-coverage=trace-pc-guard")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${FUZZER_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FUZZER_FLAGS}")
endif()

find_package(OpenSSL 1.0.2 REQUIRED)
include(CheckOpenSSLImportTargets)

find_package(Boost 1.58.0 REQUIRED COMPONENTS iostreams)
include(CheckBoostImportTargets)

include_directories(SYSTEM ${Boost_INCLUDE_DIRS})

include(AddClangFormat)

find_package(FlatBuffers 1.9.0 REQUIRED)

set(FLATBUFFERS_SCHEMA_FILES
    ${CMAKE_CURRENT_LIST_DIR}/../schema/smrf.fbs
    ${CMAKE_CURRENT_LIST_DIR}/../schema/encryption.fbs
)
set(FLATBUFFERS_GENERATED_DIR "${CMAKE_CURRENT_LIST_DIR}/flatbuffers_generated")
build_flatbuffers("${FLATBUFFERS_SCHEMA_FILES}" "" flatbuffers_generated "" "${FLATBUFFERS_GENERATED_DIR}" "" "")

add_subdirectory(libsmrf)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

# allow to install smrf

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/smrf/smrfConfigVersion.cmake"
    VERSION ${SMRF_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(
    EXPORT smrfTargets
    FILE smrfTargets.cmake
    NAMESPACE smrf::
    DESTINATION ${INSTALL_CMAKE_DIR}
)

install(
    FILES
        smrfConfig.cmake
        "${CMAKE_CURRENT_BINARY_DIR}/smrf/smrfConfigVersion.cmake"
        "CMake/CheckBoostImportTargets.cmake"
        "CMake/CheckOpenSSLImportTargets.cmake"
    DESTINATION ${INSTALL_CMAKE_DIR}
)
