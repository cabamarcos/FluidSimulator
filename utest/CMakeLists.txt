include(GoogleTest)

add_executable(utest grid_test.cpp progargs_test.cpp utils_test.cpp)
target_link_libraries(utest PRIVATE sim GTest::gtest GTest::gtest_main)
target_include_directories(utest PRIVATE ..)

gtest_discover_tests(utest)