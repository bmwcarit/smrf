# Enable ExternalProject CMake module
include(ExternalProject)

# Set default ExternalProject root directory
set_directory_properties(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/ThirdParty)

# wrapper function around ExternalProject_Add
# it sets default arguments which are used for every external project
function(AddExternalProject NAME)
    if (NOT TARGET ${NAME})
        set(INPUT_ARGS "${ARGV}")
        list(REMOVE_AT INPUT_ARGS 0)
        set(DEFAULT_EP_ARGS "")
        list(
            APPEND
            DEFAULT_EP_ARGS
            # Disable svn update
            UPDATE_COMMAND ""
            # Wrap download, configure and build steps in a script to log output
            LOG_DOWNLOAD ON
            LOG_CONFIGURE ON
            LOG_BUILD ON
        )

        list(FIND INPUT_ARGS "NO_PROPAGATE_CMAKE_ARGS" NO_PROPAGATE_CMAKE_ARGS_INDEX)
        if (NO_PROPAGATE_CMAKE_ARGS_INDEX EQUAL -1)
            list(
                APPEND
                DEFAULT_EP_ARGS
                CMAKE_CACHE_ARGS "-DCMAKE_C_COMPILER:FILEPATH=${CMAKE_C_COMPILER}"
                                 "-DCMAKE_CXX_COMPILER:FILEPATH=${CMAKE_CXX_COMPILER}"
                                 "-DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}"
            )
        else()
            list(REMOVE_AT INPUT_ARGS ${NO_PROPAGATE_CMAKE_ARGS_INDEX})
        endif()

        # if CMAKE_VERSION > 3.2 ninja generator can be used by NOT removing BUILD_BYPRODUCTS
        # otherwise, BUILD_BYPRODUCTS and the following value from the argument list is removed
        list(FIND INPUT_ARGS "BUILD_BYPRODUCTS" BUILD_BYPRODUCTS_INDEX)
        if (BUILD_BYPRODUCTS_INDEX GREATER -1)
            if(CMAKE_VERSION LESS 3.2)
                list(REMOVE_AT INPUT_ARGS ${BUILD_BYPRODUCTS_INDEX})
                list(REMOVE_AT INPUT_ARGS ${BUILD_BYPRODUCTS_INDEX})
            endif()
        endif()

        # pass the arguments on to ExternalProject
        set(ALL_ARGS "${INPUT_ARGS}" "${DEFAULT_EP_ARGS}")
        ExternalProject_Add(${NAME} "${ALL_ARGS}")
    endif(NOT TARGET ${NAME})
endfunction(AddExternalProject)
