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
CMAKE_SOURCE_DIR = /home/ankit/Desktop/Eth/ThirdSemester/AlgoLab/Week11_ExamPrep2/week11/strikesback

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ankit/Desktop/Eth/ThirdSemester/AlgoLab/Week11_ExamPrep2/week11/strikesback

# Include any dependencies generated for this target.
include CMakeFiles/strikesback.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/strikesback.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/strikesback.dir/flags.make

CMakeFiles/strikesback.dir/strikesback.cpp.o: CMakeFiles/strikesback.dir/flags.make
CMakeFiles/strikesback.dir/strikesback.cpp.o: strikesback.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ankit/Desktop/Eth/ThirdSemester/AlgoLab/Week11_ExamPrep2/week11/strikesback/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/strikesback.dir/strikesback.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/strikesback.dir/strikesback.cpp.o -c /home/ankit/Desktop/Eth/ThirdSemester/AlgoLab/Week11_ExamPrep2/week11/strikesback/strikesback.cpp

CMakeFiles/strikesback.dir/strikesback.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/strikesback.dir/strikesback.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ankit/Desktop/Eth/ThirdSemester/AlgoLab/Week11_ExamPrep2/week11/strikesback/strikesback.cpp > CMakeFiles/strikesback.dir/strikesback.cpp.i

CMakeFiles/strikesback.dir/strikesback.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/strikesback.dir/strikesback.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ankit/Desktop/Eth/ThirdSemester/AlgoLab/Week11_ExamPrep2/week11/strikesback/strikesback.cpp -o CMakeFiles/strikesback.dir/strikesback.cpp.s

CMakeFiles/strikesback.dir/strikesback.cpp.o.requires:

.PHONY : CMakeFiles/strikesback.dir/strikesback.cpp.o.requires

CMakeFiles/strikesback.dir/strikesback.cpp.o.provides: CMakeFiles/strikesback.dir/strikesback.cpp.o.requires
	$(MAKE) -f CMakeFiles/strikesback.dir/build.make CMakeFiles/strikesback.dir/strikesback.cpp.o.provides.build
.PHONY : CMakeFiles/strikesback.dir/strikesback.cpp.o.provides

CMakeFiles/strikesback.dir/strikesback.cpp.o.provides.build: CMakeFiles/strikesback.dir/strikesback.cpp.o


# Object files for target strikesback
strikesback_OBJECTS = \
"CMakeFiles/strikesback.dir/strikesback.cpp.o"

# External object files for target strikesback
strikesback_EXTERNAL_OBJECTS =

strikesback: CMakeFiles/strikesback.dir/strikesback.cpp.o
strikesback: CMakeFiles/strikesback.dir/build.make
strikesback: /usr/local/lib/libmpfr.so
strikesback: /usr/local/lib/libgmp.so
strikesback: /usr/local/lib/libCGAL_Core.so.11.0.1
strikesback: /usr/local/lib/libCGAL.so.11.0.1
strikesback: /usr/local/lib/libboost_thread.so
strikesback: /usr/local/lib/libboost_system.so
strikesback: /usr/lib/x86_64-linux-gnu/libpthread.so
strikesback: /usr/local/lib/libCGAL_Core.so.11.0.1
strikesback: /usr/local/lib/libCGAL.so.11.0.1
strikesback: /usr/local/lib/libboost_thread.so
strikesback: /usr/local/lib/libboost_system.so
strikesback: /usr/lib/x86_64-linux-gnu/libpthread.so
strikesback: CMakeFiles/strikesback.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ankit/Desktop/Eth/ThirdSemester/AlgoLab/Week11_ExamPrep2/week11/strikesback/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable strikesback"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/strikesback.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/strikesback.dir/build: strikesback

.PHONY : CMakeFiles/strikesback.dir/build

CMakeFiles/strikesback.dir/requires: CMakeFiles/strikesback.dir/strikesback.cpp.o.requires

.PHONY : CMakeFiles/strikesback.dir/requires

CMakeFiles/strikesback.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/strikesback.dir/cmake_clean.cmake
.PHONY : CMakeFiles/strikesback.dir/clean

CMakeFiles/strikesback.dir/depend:
	cd /home/ankit/Desktop/Eth/ThirdSemester/AlgoLab/Week11_ExamPrep2/week11/strikesback && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ankit/Desktop/Eth/ThirdSemester/AlgoLab/Week11_ExamPrep2/week11/strikesback /home/ankit/Desktop/Eth/ThirdSemester/AlgoLab/Week11_ExamPrep2/week11/strikesback /home/ankit/Desktop/Eth/ThirdSemester/AlgoLab/Week11_ExamPrep2/week11/strikesback /home/ankit/Desktop/Eth/ThirdSemester/AlgoLab/Week11_ExamPrep2/week11/strikesback /home/ankit/Desktop/Eth/ThirdSemester/AlgoLab/Week11_ExamPrep2/week11/strikesback/CMakeFiles/strikesback.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/strikesback.dir/depend

