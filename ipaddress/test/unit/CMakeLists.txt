add_executable(UnitTest "main.cpp")
find_package(GTest CONFIG REQUIRED)

target_link_libraries(UnitTest PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

target_link_libraries(UnitTest PRIVATE ipaddress)
add_dependencies(UnitTest ipaddress)
add_test(UnitTest main)

set_property(TARGET UnitTest PROPERTY CXX_STANDARD 17)