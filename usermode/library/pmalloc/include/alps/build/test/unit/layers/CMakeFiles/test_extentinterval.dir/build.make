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
include test/unit/layers/CMakeFiles/test_extentinterval.dir/depend.make

# Include the progress variables for this target.
include test/unit/layers/CMakeFiles/test_extentinterval.dir/progress.make

# Include the compile flags for this target's objects.
include test/unit/layers/CMakeFiles/test_extentinterval.dir/flags.make

test/unit/layers/CMakeFiles/test_extentinterval.dir/test_extentinterval.cc.o: test/unit/layers/CMakeFiles/test_extentinterval.dir/flags.make
test/unit/layers/CMakeFiles/test_extentinterval.dir/test_extentinterval.cc.o: ../test/unit/layers/test_extentinterval.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/unit/layers/CMakeFiles/test_extentinterval.dir/test_extentinterval.cc.o"
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/test/unit/layers && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/test_extentinterval.dir/test_extentinterval.cc.o -c /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/test/unit/layers/test_extentinterval.cc

test/unit/layers/CMakeFiles/test_extentinterval.dir/test_extentinterval.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test_extentinterval.dir/test_extentinterval.cc.i"
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/test/unit/layers && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/test/unit/layers/test_extentinterval.cc > CMakeFiles/test_extentinterval.dir/test_extentinterval.cc.i

test/unit/layers/CMakeFiles/test_extentinterval.dir/test_extentinterval.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test_extentinterval.dir/test_extentinterval.cc.s"
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/test/unit/layers && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/test/unit/layers/test_extentinterval.cc -o CMakeFiles/test_extentinterval.dir/test_extentinterval.cc.s

test/unit/layers/CMakeFiles/test_extentinterval.dir/test_extentinterval.cc.o.requires:

.PHONY : test/unit/layers/CMakeFiles/test_extentinterval.dir/test_extentinterval.cc.o.requires

test/unit/layers/CMakeFiles/test_extentinterval.dir/test_extentinterval.cc.o.provides: test/unit/layers/CMakeFiles/test_extentinterval.dir/test_extentinterval.cc.o.requires
	$(MAKE) -f test/unit/layers/CMakeFiles/test_extentinterval.dir/build.make test/unit/layers/CMakeFiles/test_extentinterval.dir/test_extentinterval.cc.o.provides.build
.PHONY : test/unit/layers/CMakeFiles/test_extentinterval.dir/test_extentinterval.cc.o.provides

test/unit/layers/CMakeFiles/test_extentinterval.dir/test_extentinterval.cc.o.provides.build: test/unit/layers/CMakeFiles/test_extentinterval.dir/test_extentinterval.cc.o


# Object files for target test_extentinterval
test_extentinterval_OBJECTS = \
"CMakeFiles/test_extentinterval.dir/test_extentinterval.cc.o"

# External object files for target test_extentinterval
test_extentinterval_EXTERNAL_OBJECTS =

test/unit/layers/test_extentinterval: test/unit/layers/CMakeFiles/test_extentinterval.dir/test_extentinterval.cc.o
test/unit/layers/test_extentinterval: test/unit/layers/CMakeFiles/test_extentinterval.dir/build.make
test/unit/layers/test_extentinterval: third-party/gtest/libgtest.a
test/unit/layers/test_extentinterval: src/libalps.so
test/unit/layers/test_extentinterval: third-party/libbacktrace/libbacktrace.a
test/unit/layers/test_extentinterval: third-party/yaml-cpp-0.5.2/libyaml-cpp.a
test/unit/layers/test_extentinterval: test/unit/layers/CMakeFiles/test_extentinterval.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test_extentinterval"
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/test/unit/layers && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test_extentinterval.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/unit/layers/CMakeFiles/test_extentinterval.dir/build: test/unit/layers/test_extentinterval

.PHONY : test/unit/layers/CMakeFiles/test_extentinterval.dir/build

test/unit/layers/CMakeFiles/test_extentinterval.dir/requires: test/unit/layers/CMakeFiles/test_extentinterval.dir/test_extentinterval.cc.o.requires

.PHONY : test/unit/layers/CMakeFiles/test_extentinterval.dir/requires

test/unit/layers/CMakeFiles/test_extentinterval.dir/clean:
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/test/unit/layers && $(CMAKE_COMMAND) -P CMakeFiles/test_extentinterval.dir/cmake_clean.cmake
.PHONY : test/unit/layers/CMakeFiles/test_extentinterval.dir/clean

test/unit/layers/CMakeFiles/test_extentinterval.dir/depend:
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/test/unit/layers /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/test/unit/layers /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/test/unit/layers/CMakeFiles/test_extentinterval.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/unit/layers/CMakeFiles/test_extentinterval.dir/depend
