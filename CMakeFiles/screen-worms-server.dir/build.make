# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /home/rafal/sik/sik2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/rafal/sik/sik2

# Include any dependencies generated for this target.
include CMakeFiles/screen-worms-server.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/screen-worms-server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/screen-worms-server.dir/flags.make

CMakeFiles/screen-worms-server.dir/screen-worms-server.cpp.o: CMakeFiles/screen-worms-server.dir/flags.make
CMakeFiles/screen-worms-server.dir/screen-worms-server.cpp.o: screen-worms-server.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rafal/sik/sik2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/screen-worms-server.dir/screen-worms-server.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/screen-worms-server.dir/screen-worms-server.cpp.o -c /home/rafal/sik/sik2/screen-worms-server.cpp

CMakeFiles/screen-worms-server.dir/screen-worms-server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/screen-worms-server.dir/screen-worms-server.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rafal/sik/sik2/screen-worms-server.cpp > CMakeFiles/screen-worms-server.dir/screen-worms-server.cpp.i

CMakeFiles/screen-worms-server.dir/screen-worms-server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/screen-worms-server.dir/screen-worms-server.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rafal/sik/sik2/screen-worms-server.cpp -o CMakeFiles/screen-worms-server.dir/screen-worms-server.cpp.s

CMakeFiles/screen-worms-server.dir/screen-worms-server.cpp.o.requires:

.PHONY : CMakeFiles/screen-worms-server.dir/screen-worms-server.cpp.o.requires

CMakeFiles/screen-worms-server.dir/screen-worms-server.cpp.o.provides: CMakeFiles/screen-worms-server.dir/screen-worms-server.cpp.o.requires
	$(MAKE) -f CMakeFiles/screen-worms-server.dir/build.make CMakeFiles/screen-worms-server.dir/screen-worms-server.cpp.o.provides.build
.PHONY : CMakeFiles/screen-worms-server.dir/screen-worms-server.cpp.o.provides

CMakeFiles/screen-worms-server.dir/screen-worms-server.cpp.o.provides.build: CMakeFiles/screen-worms-server.dir/screen-worms-server.cpp.o


# Object files for target screen-worms-server
screen__worms__server_OBJECTS = \
"CMakeFiles/screen-worms-server.dir/screen-worms-server.cpp.o"

# External object files for target screen-worms-server
screen__worms__server_EXTERNAL_OBJECTS =

screen-worms-server: CMakeFiles/screen-worms-server.dir/screen-worms-server.cpp.o
screen-worms-server: CMakeFiles/screen-worms-server.dir/build.make
screen-worms-server: liberr.a
screen-worms-server: CMakeFiles/screen-worms-server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/rafal/sik/sik2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable screen-worms-server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/screen-worms-server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/screen-worms-server.dir/build: screen-worms-server

.PHONY : CMakeFiles/screen-worms-server.dir/build

CMakeFiles/screen-worms-server.dir/requires: CMakeFiles/screen-worms-server.dir/screen-worms-server.cpp.o.requires

.PHONY : CMakeFiles/screen-worms-server.dir/requires

CMakeFiles/screen-worms-server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/screen-worms-server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/screen-worms-server.dir/clean

CMakeFiles/screen-worms-server.dir/depend:
	cd /home/rafal/sik/sik2 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/rafal/sik/sik2 /home/rafal/sik/sik2 /home/rafal/sik/sik2 /home/rafal/sik/sik2 /home/rafal/sik/sik2/CMakeFiles/screen-worms-server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/screen-worms-server.dir/depend

