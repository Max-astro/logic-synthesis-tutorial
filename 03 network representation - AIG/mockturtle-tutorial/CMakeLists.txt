# Copyright 2020, 2021 Peter Dimov
# Distributed under the Boost Software License, Version 1.0.
# https://www.boost.org/LICENSE_1_0.txt

cmake_minimum_required(VERSION 3.8)
project(turtle_tutorial)

set(CMAKE_CXX_COMPILER_VERSION 14.0.6)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set(Kitty_Inc /home/max/proj/EDA/epfl-synlibs/kitty/include/)
# set(MockTurtle_Dir /home/max/proj/EDA/epfl-synlibs/mockturtle)
# set(ENABLE_ABC ON)

# # message(STATUS "Boost version: ${Boost_VERSION}")
# # message(STATUS "Boost_INCLUDE_DIRS = ${Boost_INCLUDE_DIRS}.")
# message(STATUS "Kitty lib: ${Kitty_Inc}")
# message(STATUS "MockTurtle path: ${MockTurtle_Dir}")
# # message(STATUS "Boost lib dir: ${Boost_LIBRARY_DIRS}")


# include_directories(${Kitty_Inc}) 
# find_package(mockturtle REQUIRED ${MockTurtle_Dir})
# add_library(mockturtle ${MockTurtle_Dir})
# include_directories(${MockTurtle_Dir}/include) 


# add_subdirectory(lib)
add_subdirectory(mockturtle)

add_compile_options(-g)

# Automatically gather all cpp files in the current source dir

# add_executable(try try.cpp )
# target_link_libraries(try PUBLIC fmt mockturtle lorina kitty parallel_hashmap)

# target_link_libraries(aig PUBLIC fmt mockturtle lorina kitty parallel_hashmap)

file(GLOB EXECUTABLE_SOURCES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

foreach(SRC_FILE ${EXECUTABLE_SOURCES})
    get_filename_component(BIN_NAME ${SRC_FILE} NAME_WE)
    add_executable(${BIN_NAME} ${SRC_FILE})
    # target_link_libraries(${BIN_NAME} PUBLIC mockturtle)
    target_link_libraries(${BIN_NAME} PUBLIC fmt mockturtle lorina kitty parallel_hashmap bill percy)
endforeach()
# add_executable(al cust_alloc.cpp )

# target_link_libraries(bfs ${Boost_LIBRARIES})
