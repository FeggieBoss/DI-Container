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

# Utility rule file for run_tests.

# Include the progress variables for this target.
include src/unit_tests/CMakeFiles/run_tests.dir/progress.make

src/unit_tests/CMakeFiles/run_tests: bin/unit_tests
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/feggie/MyData/rttr-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Running unit_tests"
	cd /home/feggie/MyData/rttr-build/src/unit_tests && ../../bin/unit_tests

run_tests: src/unit_tests/CMakeFiles/run_tests
run_tests: src/unit_tests/CMakeFiles/run_tests.dir/build.make

.PHONY : run_tests

# Rule to build all files generated by this target.
src/unit_tests/CMakeFiles/run_tests.dir/build: run_tests

.PHONY : src/unit_tests/CMakeFiles/run_tests.dir/build

src/unit_tests/CMakeFiles/run_tests.dir/clean:
	cd /home/feggie/MyData/rttr-build/src/unit_tests && $(CMAKE_COMMAND) -P CMakeFiles/run_tests.dir/cmake_clean.cmake
.PHONY : src/unit_tests/CMakeFiles/run_tests.dir/clean

src/unit_tests/CMakeFiles/run_tests.dir/depend:
	cd /home/feggie/MyData/rttr-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/feggie/MyData/rttr-0.9.6 /home/feggie/MyData/rttr-0.9.6/src/unit_tests /home/feggie/MyData/rttr-build /home/feggie/MyData/rttr-build/src/unit_tests /home/feggie/MyData/rttr-build/src/unit_tests/CMakeFiles/run_tests.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/unit_tests/CMakeFiles/run_tests.dir/depend

