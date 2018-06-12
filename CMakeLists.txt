cmake_minimum_required (VERSION 2.6)
project (Gate)
add_executable(
	test-server
	gate-handlers/base.cpp
	gate-handlers/update-car-position.cpp
	gate-request-handler-factory.cpp
	gate-server-application.cpp
)

target_link_libraries(test-server PocoNet PocoUtil PocoFoundation PocoJSON PocoRedis s2 h3)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
set (CMAKE_CXX_FLAGS_DEBUG "-DDEBUG")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O2 -fopenmp")
