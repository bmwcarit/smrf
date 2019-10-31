option(
    USE_PLATFORM_GTEST_GMOCK
    "Resolve dependency to gtest and gmock from the system?"
    ON
)
message(STATUS "option USE_PLATFORM_GTEST_GMOCK=" ${USE_PLATFORM_GTEST_GMOCK})

include(AddExternalProject)
find_package(Threads) # sets CMAKE_THREAD_LIBS_INIT

if (USE_PLATFORM_GTEST_GMOCK)

    find_package(GTest REQUIRED)
    find_package(GMock REQUIRED)

else (USE_PLATFORM_GTEST_GMOCK)
    ### clone googletest git repository ###########################################################

    # this repository contains both googletest and googlemock
    AddExternalProject(
        googletest-repo
        DOWNLOAD_DIR "googletest-repo"
        TIMEOUT 10
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.10.0
        CONFIGURE_COMMAND "" # Disable configuring
        BUILD_COMMAND "" # Disable building
        # Patch commands below are specific for release-1.10.0 to fix
        # compiler warnings wrt. switch statements not covering all values.
        PATCH_COMMAND
            COMMAND "sed" "-i" "1 i #pragma GCC diagnostic ignored \"-Wswitch-enum\"" "googletest/src/gtest.cc"
            COMMAND "sed" "-i" "2 i #pragma GCC diagnostic ignored \"-Wsign-conversion\"" "googletest/src/gtest.cc"
            COMMAND "sed" "-i" "1 i #pragma GCC diagnostic ignored \"-Wsign-conversion\"" "googlemock/include/gmock/gmock-more-actions.h"
            COMMAND "sed" "-i" "1 i #pragma GCC diagnostic ignored \"-Wswitch-enum\"" "googlemock/src/gmock-spec-builders.cc"
        INSTALL_COMMAND "" # Disable install step
    )

    get_property(EP_PREFIX DIRECTORY PROPERTY EP_PREFIX)

    ### Add google test ###########################################################

    # build googletest library
    AddExternalProject(
        googletest
        DOWNLOAD_COMMAND "" # already downloaded by googletest-repo
        SOURCE_DIR "${EP_PREFIX}/src/googletest-repo/googletest"
        BUILD_BYPRODUCTS "${EP_PREFIX}/src/googletest-build/${CMAKE_STATIC_LIBRARY_PREFIX}gtest${CMAKE_STATIC_LIBRARY_SUFFIX}"
        CMAKE_ARGS -Dgtest_force_shared_crt=ON
                   -Dgtest_disable_pthreads=${DISABLE_PTHREAD_CMAKE_ARG}
        INSTALL_COMMAND "" # Disable install step
    )

    ExternalProject_Get_Property(googletest binary_dir)
    set(googletest_binary_dir ${binary_dir})
    message(STATUS "variable googletest_binary_dir=${googletest_binary_dir}")

    # build googletest AFTER downloading the sources
    add_dependencies(googletest googletest-repo)

    # Specify include dir
    ExternalProject_Get_Property(googletest source_dir)
    set(googletest_source_dir ${source_dir})
    message(STATUS "variable googletest_source_dir=${googletest_source_dir}")
    set(GTEST_INCLUDE_DIRS ${googletest_source_dir}/include)
    set(GTEST_LIBRARIES ${googletest_binary_dir}/lib/libgtest.a)

    ### Add google mock ###########################################################

    # build googlemock library
    AddExternalProject(
        googlemock
        DOWNLOAD_COMMAND "" # already downloaded by googletest-repo
        SOURCE_DIR "${EP_PREFIX}/src/googletest-repo/googlemock"
        BUILD_BYPRODUCTS "${EP_PREFIX}/src/googlemock-build/${CMAKE_STATIC_LIBRARY_PREFIX}gmock${CMAKE_STATIC_LIBRARY_SUFFIX}"
        CMAKE_ARGS -Dgtest_force_shared_crt=ON
                   -Dgtest_disable_pthreads=${DISABLE_PTHREAD_CMAKE_ARG}
        INSTALL_COMMAND "" # Disable install step
    )

    ExternalProject_Get_Property(googlemock binary_dir)
    set(googlemock_binary_dir ${binary_dir})
    message(STATUS "variable googlemock_binary_dir=${googlemock_binary_dir}")

    # build googlemock AFTER downloading the sources
    add_dependencies(googlemock googletest-repo)

    # Specify include dir
    ExternalProject_Get_Property(googlemock source_dir)
    set(googlemock_source_dir ${source_dir})
    message(STATUS "variable googlemock_source_dir=${googlemock_source_dir}")
    set(GMOCK_INCLUDE_DIRS ${googlemock_source_dir}/include)
    set(GMOCK_LIBRARIES ${googlemock_binary_dir}/lib/libgmock.a)
endif(USE_PLATFORM_GTEST_GMOCK)

function(AddTest TARGET)
    add_executable(
        ${TARGET}
        ${ARGN}
    )

    if(NOT USE_PLATFORM_GTEST_GMOCK)
        add_dependencies(${TARGET} googletest)
        add_dependencies(${TARGET} googlemock)
    endif(NOT USE_PLATFORM_GTEST_GMOCK)

    target_include_directories(
        ${TARGET}
        SYSTEM
        PRIVATE
        ${GTEST_INCLUDE_DIRS}
        ${GMOCK_INCLUDE_DIRS}
    )

    target_link_libraries(
        ${TARGET}
        ${GTEST_LIBRARIES}
        ${GMOCK_LIBRARIES}
        ${CMAKE_THREAD_LIBS_INIT}
    )
endfunction(AddTest)
