# CMakeList.txt : CMake project for IPAddress, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_library(ipaddress STATIC
"source/IPAddress.cpp"
"source/IPAddressV4.cpp"
"source/IPAddressV6.cpp"
"source/IPEndPoint.cpp" 

"include/NodelIP.h"
"include/util/Config.h"
"include/util/Endianness.h"
"include/util/Util.h"
"include/IPVersion.h"
"include/IPAddress.h"
"include/IPAddressV4.h"
"include/IPAddressV6.h"
"include/IPEndPoint.h" 
)
target_include_directories(ipaddress
	PUBLIC include
	PRIVATE source)
# TODO: Add tests and install targets if needed.
if(IPADDRESS_LIB_BENCHMARK OR IPADDRESS_LIB_EXAMPLE OR IPADDRESS_LIB_UNIT_TEST)
	enable_testing()
	add_subdirectory(test)
endif()

set_property(TARGET ipaddress PROPERTY CXX_STANDARD 17)

install(TARGETS ipaddress
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})