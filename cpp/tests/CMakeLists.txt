include(AddGtestGmock)
AddTest(unit-test test-main.cpp)
target_link_libraries(unit-test smrf)
