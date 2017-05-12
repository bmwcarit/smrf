find_library(
    LIBFUZZER_LIBRARY
    "${CMAKE_STATIC_LIBRARY_PREFIX}Fuzzer${CMAKE_STATIC_LIBRARY_SUFFIX}"
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(libfuzzer DEFAULT_MSG LIBFUZZER_LIBRARY)

mark_as_advanced(LIBFUZZER_LIBRARY)

if(LIBFUZZER_FOUND)
    add_library(LLVM::Fuzzer STATIC IMPORTED)
    set_target_properties(
        LLVM::Fuzzer
        PROPERTIES
        IMPORTED_LINK_INTERFACE_LANGUAGES "C"
        IMPORTED_LOCATION "${LIBFUZZER_LIBRARY}"
    )
endif(LIBFUZZER_FOUND)