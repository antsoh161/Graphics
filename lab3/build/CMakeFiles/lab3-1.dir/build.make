# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_SOURCE_DIR = /home/anton/Documents/graphic-design/lab3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/anton/Documents/graphic-design/lab3/build

# Include any dependencies generated for this target.
include CMakeFiles/lab3-1.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/lab3-1.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/lab3-1.dir/flags.make

CMakeFiles/lab3-1.dir/lab3-1.cpp.o: CMakeFiles/lab3-1.dir/flags.make
CMakeFiles/lab3-1.dir/lab3-1.cpp.o: ../lab3-1.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/anton/Documents/graphic-design/lab3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/lab3-1.dir/lab3-1.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/lab3-1.dir/lab3-1.cpp.o -c /home/anton/Documents/graphic-design/lab3/lab3-1.cpp

CMakeFiles/lab3-1.dir/lab3-1.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/lab3-1.dir/lab3-1.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/anton/Documents/graphic-design/lab3/lab3-1.cpp > CMakeFiles/lab3-1.dir/lab3-1.cpp.i

CMakeFiles/lab3-1.dir/lab3-1.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/lab3-1.dir/lab3-1.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/anton/Documents/graphic-design/lab3/lab3-1.cpp -o CMakeFiles/lab3-1.dir/lab3-1.cpp.s

# Object files for target lab3-1
lab3__1_OBJECTS = \
"CMakeFiles/lab3-1.dir/lab3-1.cpp.o"

# External object files for target lab3-1
lab3__1_EXTERNAL_OBJECTS =

lab3-1: CMakeFiles/lab3-1.dir/lab3-1.cpp.o
lab3-1: CMakeFiles/lab3-1.dir/build.make
lab3-1: /usr/lib/x86_64-linux-gnu/libGL.so
lab3-1: /usr/lib/x86_64-linux-gnu/libGLU.so
lab3-1: /usr/lib/x86_64-linux-gnu/libGLEW.so
lab3-1: liblodepng.a
lab3-1: libreadfile.a
lab3-1: CMakeFiles/lab3-1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/anton/Documents/graphic-design/lab3/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable lab3-1"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/lab3-1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/lab3-1.dir/build: lab3-1

.PHONY : CMakeFiles/lab3-1.dir/build

CMakeFiles/lab3-1.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/lab3-1.dir/cmake_clean.cmake
.PHONY : CMakeFiles/lab3-1.dir/clean

CMakeFiles/lab3-1.dir/depend:
	cd /home/anton/Documents/graphic-design/lab3/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/anton/Documents/graphic-design/lab3 /home/anton/Documents/graphic-design/lab3 /home/anton/Documents/graphic-design/lab3/build /home/anton/Documents/graphic-design/lab3/build /home/anton/Documents/graphic-design/lab3/build/CMakeFiles/lab3-1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/lab3-1.dir/depend
