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
CMAKE_SOURCE_DIR = /home/ankit/Desktop/Eth/ThirdSemester/AlgoLab/Week13_ExamPrep4/week13/clues

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ankit/Desktop/Eth/ThirdSemester/AlgoLab/Week13_ExamPrep4/week13/clues

# Include any dependencies generated for this target.
include CMakeFiles/clues.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/clues.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/clues.dir/flags.make

CMakeFiles/clues.dir/clues.cpp.o: CMakeFiles/clues.dir/flags.make
CMakeFiles/clues.dir/clues.cpp.o: clues.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ankit/Desktop/Eth/ThirdSemester/AlgoLab/Week13_ExamPrep4/week13/clues/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/clues.dir/clues.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/clues.dir/clues.cpp.o -c /home/ankit/Desktop/Eth/ThirdSemester/AlgoLab/Week13_ExamPrep4/week13/clues/clues.cpp

CMakeFiles/clues.dir/clues.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/clues.dir/clues.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ankit/Desktop/Eth/ThirdSemester/AlgoLab/Week13_ExamPrep4/week13/clues/clues.cpp > CMakeFiles/clues.dir/clues.cpp.i

CMakeFiles/clues.dir/clues.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/clues.dir/clues.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ankit/Desktop/Eth/ThirdSemester/AlgoLab/Week13_ExamPrep4/week13/clues/clues.cpp -o CMakeFiles/clues.dir/clues.cpp.s

CMakeFiles/clues.dir/clues.cpp.o.requires:

.PHONY : CMakeFiles/clues.dir/clues.cpp.o.requires

CMakeFiles/clues.dir/clues.cpp.o.provides: CMakeFiles/clues.dir/clues.cpp.o.requires
	$(MAKE) -f CMakeFiles/clues.dir/build.make CMakeFiles/clues.dir/clues.cpp.o.provides.build
.PHONY : CMakeFiles/clues.dir/clues.cpp.o.provides

CMakeFiles/clues.dir/clues.cpp.o.provides.build: CMakeFiles/clues.dir/clues.cpp.o


# Object files for target clues
clues_OBJECTS = \
"CMakeFiles/clues.dir/clues.cpp.o"

# External object files for target clues
clues_EXTERNAL_OBJECTS =

clues: CMakeFiles/clues.dir/clues.cpp.o
clues: CMakeFiles/clues.dir/build.make
clues: /usr/local/lib/libmpfr.so
clues: /usr/local/lib/libgmp.so
clues: /usr/local/lib/libCGAL_Core.so.11.0.1
clues: /usr/local/lib/libCGAL.so.11.0.1
clues: /usr/local/lib/libboost_thread.so
clues: /usr/local/lib/libboost_system.so
clues: /usr/lib/x86_64-linux-gnu/libpthread.so
clues: /usr/local/lib/libCGAL_Core.so.11.0.1
clues: /usr/local/lib/libCGAL.so.11.0.1
clues: /usr/local/lib/libboost_thread.so
clues: /usr/local/lib/libboost_system.so
clues: /usr/lib/x86_64-linux-gnu/libpthread.so
clues: CMakeFiles/clues.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ankit/Desktop/Eth/ThirdSemester/AlgoLab/Week13_ExamPrep4/week13/clues/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable clues"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/clues.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/clues.dir/build: clues

.PHONY : CMakeFiles/clues.dir/build

CMakeFiles/clues.dir/requires: CMakeFiles/clues.dir/clues.cpp.o.requires

.PHONY : CMakeFiles/clues.dir/requires

CMakeFiles/clues.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/clues.dir/cmake_clean.cmake
.PHONY : CMakeFiles/clues.dir/clean

CMakeFiles/clues.dir/depend:
	cd /home/ankit/Desktop/Eth/ThirdSemester/AlgoLab/Week13_ExamPrep4/week13/clues && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ankit/Desktop/Eth/ThirdSemester/AlgoLab/Week13_ExamPrep4/week13/clues /home/ankit/Desktop/Eth/ThirdSemester/AlgoLab/Week13_ExamPrep4/week13/clues /home/ankit/Desktop/Eth/ThirdSemester/AlgoLab/Week13_ExamPrep4/week13/clues /home/ankit/Desktop/Eth/ThirdSemester/AlgoLab/Week13_ExamPrep4/week13/clues /home/ankit/Desktop/Eth/ThirdSemester/AlgoLab/Week13_ExamPrep4/week13/clues/CMakeFiles/clues.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/clues.dir/depend

