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
CMAKE_SOURCE_DIR = /mnt/e/OS_LAB/Lab6

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/e/OS_LAB/Lab6

# Include any dependencies generated for this target.
include Lab6/CMakeFiles/Lab6.dir/depend.make

# Include the progress variables for this target.
include Lab6/CMakeFiles/Lab6.dir/progress.make

# Include the compile flags for this target's objects.
include Lab6/CMakeFiles/Lab6.dir/flags.make

Lab6/CMakeFiles/Lab6.dir/dp.cc.o: Lab6/CMakeFiles/Lab6.dir/flags.make
Lab6/CMakeFiles/Lab6.dir/dp.cc.o: Lab6/dp.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/e/OS_LAB/Lab6/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Lab6/CMakeFiles/Lab6.dir/dp.cc.o"
	cd /mnt/e/OS_LAB/Lab6/Lab6 && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Lab6.dir/dp.cc.o -c /mnt/e/OS_LAB/Lab6/Lab6/dp.cc

Lab6/CMakeFiles/Lab6.dir/dp.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Lab6.dir/dp.cc.i"
	cd /mnt/e/OS_LAB/Lab6/Lab6 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/e/OS_LAB/Lab6/Lab6/dp.cc > CMakeFiles/Lab6.dir/dp.cc.i

Lab6/CMakeFiles/Lab6.dir/dp.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Lab6.dir/dp.cc.s"
	cd /mnt/e/OS_LAB/Lab6/Lab6 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/e/OS_LAB/Lab6/Lab6/dp.cc -o CMakeFiles/Lab6.dir/dp.cc.s

# Object files for target Lab6
Lab6_OBJECTS = \
"CMakeFiles/Lab6.dir/dp.cc.o"

# External object files for target Lab6
Lab6_EXTERNAL_OBJECTS =

Lab6/Lab6: Lab6/CMakeFiles/Lab6.dir/dp.cc.o
Lab6/Lab6: Lab6/CMakeFiles/Lab6.dir/build.make
Lab6/Lab6: Lab6/CMakeFiles/Lab6.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/e/OS_LAB/Lab6/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Lab6"
	cd /mnt/e/OS_LAB/Lab6/Lab6 && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Lab6.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Lab6/CMakeFiles/Lab6.dir/build: Lab6/Lab6

.PHONY : Lab6/CMakeFiles/Lab6.dir/build

Lab6/CMakeFiles/Lab6.dir/clean:
	cd /mnt/e/OS_LAB/Lab6/Lab6 && $(CMAKE_COMMAND) -P CMakeFiles/Lab6.dir/cmake_clean.cmake
.PHONY : Lab6/CMakeFiles/Lab6.dir/clean

Lab6/CMakeFiles/Lab6.dir/depend:
	cd /mnt/e/OS_LAB/Lab6 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/e/OS_LAB/Lab6 /mnt/e/OS_LAB/Lab6/Lab6 /mnt/e/OS_LAB/Lab6 /mnt/e/OS_LAB/Lab6/Lab6 /mnt/e/OS_LAB/Lab6/Lab6/CMakeFiles/Lab6.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Lab6/CMakeFiles/Lab6.dir/depend

