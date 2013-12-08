cmake_minimum_required(VERSION 2.8)
project(usbreset)

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
set(CMAKE_BUILD_TYPE DEBUG)

if(CMAKE_COMPILER_IS_GNUCXX)
	if(CMAKE_BUILD_TYPE MATCHES "DEBUG")
		message(STATUS "warnings on")
		add_definitions(-Wall -Wextra -pedantic -ansi -Wno-unused-variable
			-Wno-unused-parameter -g)
	endif()
endif()

add_subdirectory(lib)
