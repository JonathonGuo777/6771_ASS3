# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = "/Users/guoao/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/212.5284.51/CLion.app/Contents/bin/cmake/mac/bin/cmake"

# The command to remove a file.
RM = "/Users/guoao/Library/Application Support/JetBrains/Toolbox/apps/CLion/ch-0/212.5284.51/CLion.app/Contents/bin/cmake/mac/bin/cmake" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/guoao/Desktop/comp6771/ass3/ass3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/guoao/Desktop/comp6771/ass3/ass3/cmake-build-debug

# Include any dependencies generated for this target.
include test/CMakeFiles/test_main.dir/depend.make
# Include the progress variables for this target.
include test/CMakeFiles/test_main.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/test_main.dir/flags.make

test/CMakeFiles/test_main.dir/test_main.cpp.o: test/CMakeFiles/test_main.dir/flags.make
test/CMakeFiles/test_main.dir/test_main.cpp.o: ../test/test_main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/guoao/Desktop/comp6771/ass3/ass3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/test_main.dir/test_main.cpp.o"
	cd /Users/guoao/Desktop/comp6771/ass3/ass3/cmake-build-debug/test && /usr/local/Cellar/gcc/10.2.0/bin/g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_main.dir/test_main.cpp.o -c /Users/guoao/Desktop/comp6771/ass3/ass3/test/test_main.cpp

test/CMakeFiles/test_main.dir/test_main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_main.dir/test_main.cpp.i"
	cd /Users/guoao/Desktop/comp6771/ass3/ass3/cmake-build-debug/test && /usr/local/Cellar/gcc/10.2.0/bin/g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/guoao/Desktop/comp6771/ass3/ass3/test/test_main.cpp > CMakeFiles/test_main.dir/test_main.cpp.i

test/CMakeFiles/test_main.dir/test_main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_main.dir/test_main.cpp.s"
	cd /Users/guoao/Desktop/comp6771/ass3/ass3/cmake-build-debug/test && /usr/local/Cellar/gcc/10.2.0/bin/g++-10 $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/guoao/Desktop/comp6771/ass3/ass3/test/test_main.cpp -o CMakeFiles/test_main.dir/test_main.cpp.s

# Object files for target test_main
test_main_OBJECTS = \
"CMakeFiles/test_main.dir/test_main.cpp.o"

# External object files for target test_main
test_main_EXTERNAL_OBJECTS =

test/libtest_main.a: test/CMakeFiles/test_main.dir/test_main.cpp.o
test/libtest_main.a: test/CMakeFiles/test_main.dir/build.make
test/libtest_main.a: test/CMakeFiles/test_main.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/guoao/Desktop/comp6771/ass3/ass3/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libtest_main.a"
	cd /Users/guoao/Desktop/comp6771/ass3/ass3/cmake-build-debug/test && $(CMAKE_COMMAND) -P CMakeFiles/test_main.dir/cmake_clean_target.cmake
	cd /Users/guoao/Desktop/comp6771/ass3/ass3/cmake-build-debug/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_main.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/test_main.dir/build: test/libtest_main.a
.PHONY : test/CMakeFiles/test_main.dir/build

test/CMakeFiles/test_main.dir/clean:
	cd /Users/guoao/Desktop/comp6771/ass3/ass3/cmake-build-debug/test && $(CMAKE_COMMAND) -P CMakeFiles/test_main.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/test_main.dir/clean

test/CMakeFiles/test_main.dir/depend:
	cd /Users/guoao/Desktop/comp6771/ass3/ass3/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/guoao/Desktop/comp6771/ass3/ass3 /Users/guoao/Desktop/comp6771/ass3/ass3/test /Users/guoao/Desktop/comp6771/ass3/ass3/cmake-build-debug /Users/guoao/Desktop/comp6771/ass3/ass3/cmake-build-debug/test /Users/guoao/Desktop/comp6771/ass3/ass3/cmake-build-debug/test/CMakeFiles/test_main.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/test_main.dir/depend

