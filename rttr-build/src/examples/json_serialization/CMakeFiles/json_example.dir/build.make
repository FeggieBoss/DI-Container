# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/feggie/MyData/rttr-0.9.6

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/feggie/MyData/rttr-build

# Include any dependencies generated for this target.
include src/examples/json_serialization/CMakeFiles/json_example.dir/depend.make

# Include the progress variables for this target.
include src/examples/json_serialization/CMakeFiles/json_example.dir/progress.make

# Include the compile flags for this target's objects.
include src/examples/json_serialization/CMakeFiles/json_example.dir/flags.make

src/examples/json_serialization/CMakeFiles/json_example.dir/main.cpp.o: src/examples/json_serialization/CMakeFiles/json_example.dir/flags.make
src/examples/json_serialization/CMakeFiles/json_example.dir/main.cpp.o: /home/feggie/MyData/rttr-0.9.6/src/examples/json_serialization/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/feggie/MyData/rttr-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/examples/json_serialization/CMakeFiles/json_example.dir/main.cpp.o"
	cd /home/feggie/MyData/rttr-build/src/examples/json_serialization && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/json_example.dir/main.cpp.o -c /home/feggie/MyData/rttr-0.9.6/src/examples/json_serialization/main.cpp

src/examples/json_serialization/CMakeFiles/json_example.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/json_example.dir/main.cpp.i"
	cd /home/feggie/MyData/rttr-build/src/examples/json_serialization && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/feggie/MyData/rttr-0.9.6/src/examples/json_serialization/main.cpp > CMakeFiles/json_example.dir/main.cpp.i

src/examples/json_serialization/CMakeFiles/json_example.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/json_example.dir/main.cpp.s"
	cd /home/feggie/MyData/rttr-build/src/examples/json_serialization && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/feggie/MyData/rttr-0.9.6/src/examples/json_serialization/main.cpp -o CMakeFiles/json_example.dir/main.cpp.s

src/examples/json_serialization/CMakeFiles/json_example.dir/to_json.cpp.o: src/examples/json_serialization/CMakeFiles/json_example.dir/flags.make
src/examples/json_serialization/CMakeFiles/json_example.dir/to_json.cpp.o: /home/feggie/MyData/rttr-0.9.6/src/examples/json_serialization/to_json.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/feggie/MyData/rttr-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/examples/json_serialization/CMakeFiles/json_example.dir/to_json.cpp.o"
	cd /home/feggie/MyData/rttr-build/src/examples/json_serialization && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/json_example.dir/to_json.cpp.o -c /home/feggie/MyData/rttr-0.9.6/src/examples/json_serialization/to_json.cpp

src/examples/json_serialization/CMakeFiles/json_example.dir/to_json.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/json_example.dir/to_json.cpp.i"
	cd /home/feggie/MyData/rttr-build/src/examples/json_serialization && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/feggie/MyData/rttr-0.9.6/src/examples/json_serialization/to_json.cpp > CMakeFiles/json_example.dir/to_json.cpp.i

src/examples/json_serialization/CMakeFiles/json_example.dir/to_json.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/json_example.dir/to_json.cpp.s"
	cd /home/feggie/MyData/rttr-build/src/examples/json_serialization && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/feggie/MyData/rttr-0.9.6/src/examples/json_serialization/to_json.cpp -o CMakeFiles/json_example.dir/to_json.cpp.s

src/examples/json_serialization/CMakeFiles/json_example.dir/from_json.cpp.o: src/examples/json_serialization/CMakeFiles/json_example.dir/flags.make
src/examples/json_serialization/CMakeFiles/json_example.dir/from_json.cpp.o: /home/feggie/MyData/rttr-0.9.6/src/examples/json_serialization/from_json.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/feggie/MyData/rttr-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/examples/json_serialization/CMakeFiles/json_example.dir/from_json.cpp.o"
	cd /home/feggie/MyData/rttr-build/src/examples/json_serialization && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/json_example.dir/from_json.cpp.o -c /home/feggie/MyData/rttr-0.9.6/src/examples/json_serialization/from_json.cpp

src/examples/json_serialization/CMakeFiles/json_example.dir/from_json.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/json_example.dir/from_json.cpp.i"
	cd /home/feggie/MyData/rttr-build/src/examples/json_serialization && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/feggie/MyData/rttr-0.9.6/src/examples/json_serialization/from_json.cpp > CMakeFiles/json_example.dir/from_json.cpp.i

src/examples/json_serialization/CMakeFiles/json_example.dir/from_json.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/json_example.dir/from_json.cpp.s"
	cd /home/feggie/MyData/rttr-build/src/examples/json_serialization && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/feggie/MyData/rttr-0.9.6/src/examples/json_serialization/from_json.cpp -o CMakeFiles/json_example.dir/from_json.cpp.s

# Object files for target json_example
json_example_OBJECTS = \
"CMakeFiles/json_example.dir/main.cpp.o" \
"CMakeFiles/json_example.dir/to_json.cpp.o" \
"CMakeFiles/json_example.dir/from_json.cpp.o"

# External object files for target json_example
json_example_EXTERNAL_OBJECTS =

bin/json_example: src/examples/json_serialization/CMakeFiles/json_example.dir/main.cpp.o
bin/json_example: src/examples/json_serialization/CMakeFiles/json_example.dir/to_json.cpp.o
bin/json_example: src/examples/json_serialization/CMakeFiles/json_example.dir/from_json.cpp.o
bin/json_example: src/examples/json_serialization/CMakeFiles/json_example.dir/build.make
bin/json_example: lib/librttr_core.so.0.9.6
bin/json_example: src/examples/json_serialization/CMakeFiles/json_example.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/feggie/MyData/rttr-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable ../../../bin/json_example"
	cd /home/feggie/MyData/rttr-build/src/examples/json_serialization && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/json_example.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/examples/json_serialization/CMakeFiles/json_example.dir/build: bin/json_example

.PHONY : src/examples/json_serialization/CMakeFiles/json_example.dir/build

src/examples/json_serialization/CMakeFiles/json_example.dir/clean:
	cd /home/feggie/MyData/rttr-build/src/examples/json_serialization && $(CMAKE_COMMAND) -P CMakeFiles/json_example.dir/cmake_clean.cmake
.PHONY : src/examples/json_serialization/CMakeFiles/json_example.dir/clean

src/examples/json_serialization/CMakeFiles/json_example.dir/depend:
	cd /home/feggie/MyData/rttr-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/feggie/MyData/rttr-0.9.6 /home/feggie/MyData/rttr-0.9.6/src/examples/json_serialization /home/feggie/MyData/rttr-build /home/feggie/MyData/rttr-build/src/examples/json_serialization /home/feggie/MyData/rttr-build/src/examples/json_serialization/CMakeFiles/json_example.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/examples/json_serialization/CMakeFiles/json_example.dir/depend
