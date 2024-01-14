#add_executable(BenchmarkTest "main.cpp")
#find_package(GTest CONFIG REQUIRED)
#
#target_link_libraries(BenchmarkTest PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
#
#target_link_libraries(BenchmarkTest PRIVATE ipaddress)
#add_dependencies(BenchmarkTest ipaddress)
#add_test(BenchmarkTest main)
#
#set_property(TARGET BenchmarkTest PROPERTY CXX_STANDARD 17)