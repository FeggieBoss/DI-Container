cmake_minimum_required(VERSION 3.16.3)	 
							
project(di_container VERSION 1.0)		

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
set(Boost_USE_STATIC_LIBS OFF)
set(RTTR_DIR "~/MyData/rttr-build/install/share/rttr/cmake")

find_package(RTTR CONFIG REQUIRED Core)

add_executable(app ${SOURCES})
target_link_libraries(app RTTR::Core)