cmake_minimum_required(VERSION 3.16.3)	 
							
project(di_container VERSION 1.0)		

file(GLOB SOURCES "src/*.cpp")
file(GLOB DI_SOURCES "src/*di_container.cpp")
file(GLOB TEST_SOURCES "test/src/*_test.cpp")
file(GLOB DOP_TEST_SOURCES "test/src/*.cpp")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
# set(Boost_USE_STATIC_LIBS OFF)
set(RTTR_DIR "~/MyData/rttr-build/install/share/rttr/cmake") # local path to rttr/cmake !!!



### Main
include_directories(include)
include_directories(test/include)
add_executable(app ${SOURCES})
### Main

### RTTR Main
find_package(RTTR CONFIG REQUIRED Core)
target_link_libraries(app RTTR::Core)
### RTTR Main



### GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

include_directories(${googletest_SOURCE_DIR}/googletest/include/gtest)
add_executable(unitTests ${DI_SOURCES} ${TEST_SOURCES} ${DOP_TEST_SOURCES})

target_link_libraries(
    unitTests
    gtest_main
    gtest
)

include(GoogleTest)
gtest_discover_tests(unitTests)
### GoogleTest

### RTTR GoogleTest
# find_package(RTTR CONFIG REQUIRED Core) <- already found
target_link_libraries(unitTests RTTR::Core)
### RTTR GoogleTest