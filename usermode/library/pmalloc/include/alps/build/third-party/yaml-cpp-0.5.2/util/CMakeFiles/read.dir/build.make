# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.4

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
CMAKE_SOURCE_DIR = /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build

# Include any dependencies generated for this target.
include third-party/yaml-cpp-0.5.2/util/CMakeFiles/read.dir/depend.make

# Include the progress variables for this target.
include third-party/yaml-cpp-0.5.2/util/CMakeFiles/read.dir/progress.make

# Include the compile flags for this target's objects.
include third-party/yaml-cpp-0.5.2/util/CMakeFiles/read.dir/flags.make

third-party/yaml-cpp-0.5.2/util/CMakeFiles/read.dir/read.cpp.o: third-party/yaml-cpp-0.5.2/util/CMakeFiles/read.dir/flags.make
third-party/yaml-cpp-0.5.2/util/CMakeFiles/read.dir/read.cpp.o: ../third-party/yaml-cpp-0.5.2/util/read.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object third-party/yaml-cpp-0.5.2/util/CMakeFiles/read.dir/read.cpp.o"
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/third-party/yaml-cpp-0.5.2/util && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/read.dir/read.cpp.o -c /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/third-party/yaml-cpp-0.5.2/util/read.cpp

third-party/yaml-cpp-0.5.2/util/CMakeFiles/read.dir/read.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/read.dir/read.cpp.i"
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/third-party/yaml-cpp-0.5.2/util && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/third-party/yaml-cpp-0.5.2/util/read.cpp > CMakeFiles/read.dir/read.cpp.i

third-party/yaml-cpp-0.5.2/util/CMakeFiles/read.dir/read.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/read.dir/read.cpp.s"
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/third-party/yaml-cpp-0.5.2/util && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/third-party/yaml-cpp-0.5.2/util/read.cpp -o CMakeFiles/read.dir/read.cpp.s

third-party/yaml-cpp-0.5.2/util/CMakeFiles/read.dir/read.cpp.o.requires:

.PHONY : third-party/yaml-cpp-0.5.2/util/CMakeFiles/read.dir/read.cpp.o.requires

third-party/yaml-cpp-0.5.2/util/CMakeFiles/read.dir/read.cpp.o.provides: third-party/yaml-cpp-0.5.2/util/CMakeFiles/read.dir/read.cpp.o.requires
	$(MAKE) -f third-party/yaml-cpp-0.5.2/util/CMakeFiles/read.dir/build.make third-party/yaml-cpp-0.5.2/util/CMakeFiles/read.dir/read.cpp.o.provides.build
.PHONY : third-party/yaml-cpp-0.5.2/util/CMakeFiles/read.dir/read.cpp.o.provides

third-party/yaml-cpp-0.5.2/util/CMakeFiles/read.dir/read.cpp.o.provides.build: third-party/yaml-cpp-0.5.2/util/CMakeFiles/read.dir/read.cpp.o


# Object files for target read
read_OBJECTS = \
"CMakeFiles/read.dir/read.cpp.o"

# External object files for target read
read_EXTERNAL_OBJECTS =

third-party/yaml-cpp-0.5.2/util/read: third-party/yaml-cpp-0.5.2/util/CMakeFiles/read.dir/read.cpp.o
third-party/yaml-cpp-0.5.2/util/read: third-party/yaml-cpp-0.5.2/util/CMakeFiles/read.dir/build.make
third-party/yaml-cpp-0.5.2/util/read: third-party/yaml-cpp-0.5.2/libyaml-cpp.a
third-party/yaml-cpp-0.5.2/util/read: third-party/yaml-cpp-0.5.2/util/CMakeFiles/read.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable read"
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/third-party/yaml-cpp-0.5.2/util && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/read.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
third-party/yaml-cpp-0.5.2/util/CMakeFiles/read.dir/build: third-party/yaml-cpp-0.5.2/util/read

.PHONY : third-party/yaml-cpp-0.5.2/util/CMakeFiles/read.dir/build

third-party/yaml-cpp-0.5.2/util/CMakeFiles/read.dir/requires: third-party/yaml-cpp-0.5.2/util/CMakeFiles/read.dir/read.cpp.o.requires

.PHONY : third-party/yaml-cpp-0.5.2/util/CMakeFiles/read.dir/requires

third-party/yaml-cpp-0.5.2/util/CMakeFiles/read.dir/clean:
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/third-party/yaml-cpp-0.5.2/util && $(CMAKE_COMMAND) -P CMakeFiles/read.dir/cmake_clean.cmake
.PHONY : third-party/yaml-cpp-0.5.2/util/CMakeFiles/read.dir/clean

third-party/yaml-cpp-0.5.2/util/CMakeFiles/read.dir/depend:
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/third-party/yaml-cpp-0.5.2/util /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/third-party/yaml-cpp-0.5.2/util /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/third-party/yaml-cpp-0.5.2/util/CMakeFiles/read.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : third-party/yaml-cpp-0.5.2/util/CMakeFiles/read.dir/depend

