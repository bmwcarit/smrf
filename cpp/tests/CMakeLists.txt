include(AddGtestGmock)

if(BUILD_UNIT_TESTS)
    add_subdirectory(unit-test)
endif()

if(BUILD_LIBFUZZER_TEST)
    add_subdirectory(libfuzzer-test)
endif()