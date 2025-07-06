cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 17)

project(cudd_tutorial)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")

set(CUDD_INCLUDE_DIRS "/home/max/proj/cudd/")

set(CUDD_UTIL_DIRS ${CUDD_INCLUDE_DIRS}/util/)
set(CUDD_CPP_DIRS ${CUDD_INCLUDE_DIRS}/cplusplus/)
set(CUDD_CPP_LIBRARIES ${CUDD_CPP_DIRS}/.libs/libobj.a)

# Find Cudd library
# find_package(CUDD REQUIRED)
include_directories(${CUDD_INCLUDE_DIRS})
# include_directories(${CUDD_UTIL_DIRS})
include_directories(${CUDD_CPP_DIRS})

# Add source files
set(BASIC src/basic.cpp)
set(NQUEENS src/nqueens.cpp)

# Create executable
add_executable(basic ${BASIC})
target_link_libraries(basic ${CUDD_CPP_LIBRARIES} cudd)


add_executable(queen ${NQUEENS})
target_link_libraries(queen ${CUDD_CPP_LIBRARIES} cudd)
