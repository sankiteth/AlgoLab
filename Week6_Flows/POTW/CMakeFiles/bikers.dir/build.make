# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/ankit/Desktop/Eth/ThirdSemester/AlgoLab/Week6/POTW

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ankit/Desktop/Eth/ThirdSemester/AlgoLab/Week6/POTW

# Include any dependencies generated for this target.
include CMakeFiles/bikers.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/bikers.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/bikers.dir/flags.make

CMakeFiles/bikers.dir/bikers.cpp.o: CMakeFiles/bikers.dir/flags.make
CMakeFiles/bikers.dir/bikers.cpp.o: bikers.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ankit/Desktop/Eth/ThirdSemester/AlgoLab/Week6/POTW/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/bikers.dir/bikers.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/bikers.dir/bikers.cpp.o -c /home/ankit/Desktop/Eth/ThirdSemester/AlgoLab/Week6/POTW/bikers.cpp

CMakeFiles/bikers.dir/bikers.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bikers.dir/bikers.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ankit/Desktop/Eth/ThirdSemester/AlgoLab/Week6/POTW/bikers.cpp > CMakeFiles/bikers.dir/bikers.cpp.i

CMakeFiles/bikers.dir/bikers.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bikers.dir/bikers.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ankit/Desktop/Eth/ThirdSemester/AlgoLab/Week6/POTW/bikers.cpp -o CMakeFiles/bikers.dir/bikers.cpp.s

CMakeFiles/bikers.dir/bikers.cpp.o.requires:

.PHONY : CMakeFiles/bikers.dir/bikers.cpp.o.requires

CMakeFiles/bikers.dir/bikers.cpp.o.provides: CMakeFiles/bikers.dir/bikers.cpp.o.requires
	$(MAKE) -f CMakeFiles/bikers.dir/build.make CMakeFiles/bikers.dir/bikers.cpp.o.provides.build
.PHONY : CMakeFiles/bikers.dir/bikers.cpp.o.provides

CMakeFiles/bikers.dir/bikers.cpp.o.provides.build: CMakeFiles/bikers.dir/bikers.cpp.o


# Object files for target bikers
bikers_OBJECTS = \
"CMakeFiles/bikers.dir/bikers.cpp.o"

# External object files for target bikers
bikers_EXTERNAL_OBJECTS =

bikers: CMakeFiles/bikers.dir/bikers.cpp.o
bikers: CMakeFiles/bikers.dir/build.make
bikers: /usr/local/lib/libmpfr.so
bikers: /usr/local/lib/libgmp.so
bikers: /usr/local/lib/libCGAL_Core.so.11.0.1
bikers: /usr/local/lib/libCGAL.so.11.0.1
bikers: /usr/local/lib/libboost_thread.so
bikers: /usr/local/lib/libboost_system.so
bikers: /usr/lib/x86_64-linux-gnu/libpthread.so
bikers: /usr/local/lib/libCGAL_Core.so.11.0.1
bikers: /usr/local/lib/libCGAL.so.11.0.1
bikers: /usr/local/lib/libboost_thread.so
bikers: /usr/local/lib/libboost_system.so
bikers: /usr/lib/x86_64-linux-gnu/libpthread.so
bikers: CMakeFiles/bikers.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ankit/Desktop/Eth/ThirdSemester/AlgoLab/Week6/POTW/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable bikers"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bikers.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/bikers.dir/build: bikers

.PHONY : CMakeFiles/bikers.dir/build

CMakeFiles/bikers.dir/requires: CMakeFiles/bikers.dir/bikers.cpp.o.requires

.PHONY : CMakeFiles/bikers.dir/requires

CMakeFiles/bikers.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/bikers.dir/cmake_clean.cmake
.PHONY : CMakeFiles/bikers.dir/clean

CMakeFiles/bikers.dir/depend:
	cd /home/ankit/Desktop/Eth/ThirdSemester/AlgoLab/Week6/POTW && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ankit/Desktop/Eth/ThirdSemester/AlgoLab/Week6/POTW /home/ankit/Desktop/Eth/ThirdSemester/AlgoLab/Week6/POTW /home/ankit/Desktop/Eth/ThirdSemester/AlgoLab/Week6/POTW /home/ankit/Desktop/Eth/ThirdSemester/AlgoLab/Week6/POTW /home/ankit/Desktop/Eth/ThirdSemester/AlgoLab/Week6/POTW/CMakeFiles/bikers.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/bikers.dir/depend

