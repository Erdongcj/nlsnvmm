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
include src/CMakeFiles/alps.dir/depend.make

# Include the progress variables for this target.
include src/CMakeFiles/alps.dir/progress.make

# Include the compile flags for this target's objects.
include src/CMakeFiles/alps.dir/flags.make

src/CMakeFiles/alps.dir/common/assert_nd.cc.o: src/CMakeFiles/alps.dir/flags.make
src/CMakeFiles/alps.dir/common/assert_nd.cc.o: ../src/common/assert_nd.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/CMakeFiles/alps.dir/common/assert_nd.cc.o"
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/alps.dir/common/assert_nd.cc.o -c /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/src/common/assert_nd.cc

src/CMakeFiles/alps.dir/common/assert_nd.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/alps.dir/common/assert_nd.cc.i"
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/src/common/assert_nd.cc > CMakeFiles/alps.dir/common/assert_nd.cc.i

src/CMakeFiles/alps.dir/common/assert_nd.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/alps.dir/common/assert_nd.cc.s"
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/src/common/assert_nd.cc -o CMakeFiles/alps.dir/common/assert_nd.cc.s

src/CMakeFiles/alps.dir/common/assert_nd.cc.o.requires:

.PHONY : src/CMakeFiles/alps.dir/common/assert_nd.cc.o.requires

src/CMakeFiles/alps.dir/common/assert_nd.cc.o.provides: src/CMakeFiles/alps.dir/common/assert_nd.cc.o.requires
	$(MAKE) -f src/CMakeFiles/alps.dir/build.make src/CMakeFiles/alps.dir/common/assert_nd.cc.o.provides.build
.PHONY : src/CMakeFiles/alps.dir/common/assert_nd.cc.o.provides

src/CMakeFiles/alps.dir/common/assert_nd.cc.o.provides.build: src/CMakeFiles/alps.dir/common/assert_nd.cc.o


src/CMakeFiles/alps.dir/common/assorted_func.cc.o: src/CMakeFiles/alps.dir/flags.make
src/CMakeFiles/alps.dir/common/assorted_func.cc.o: ../src/common/assorted_func.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/CMakeFiles/alps.dir/common/assorted_func.cc.o"
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/alps.dir/common/assorted_func.cc.o -c /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/src/common/assorted_func.cc

src/CMakeFiles/alps.dir/common/assorted_func.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/alps.dir/common/assorted_func.cc.i"
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/src/common/assorted_func.cc > CMakeFiles/alps.dir/common/assorted_func.cc.i

src/CMakeFiles/alps.dir/common/assorted_func.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/alps.dir/common/assorted_func.cc.s"
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/src/common/assorted_func.cc -o CMakeFiles/alps.dir/common/assorted_func.cc.s

src/CMakeFiles/alps.dir/common/assorted_func.cc.o.requires:

.PHONY : src/CMakeFiles/alps.dir/common/assorted_func.cc.o.requires

src/CMakeFiles/alps.dir/common/assorted_func.cc.o.provides: src/CMakeFiles/alps.dir/common/assorted_func.cc.o.requires
	$(MAKE) -f src/CMakeFiles/alps.dir/build.make src/CMakeFiles/alps.dir/common/assorted_func.cc.o.provides.build
.PHONY : src/CMakeFiles/alps.dir/common/assorted_func.cc.o.provides

src/CMakeFiles/alps.dir/common/assorted_func.cc.o.provides.build: src/CMakeFiles/alps.dir/common/assorted_func.cc.o


src/CMakeFiles/alps.dir/common/debug_options.cc.o: src/CMakeFiles/alps.dir/flags.make
src/CMakeFiles/alps.dir/common/debug_options.cc.o: ../src/common/debug_options.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/CMakeFiles/alps.dir/common/debug_options.cc.o"
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/alps.dir/common/debug_options.cc.o -c /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/src/common/debug_options.cc

src/CMakeFiles/alps.dir/common/debug_options.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/alps.dir/common/debug_options.cc.i"
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/src/common/debug_options.cc > CMakeFiles/alps.dir/common/debug_options.cc.i

src/CMakeFiles/alps.dir/common/debug_options.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/alps.dir/common/debug_options.cc.s"
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/src/common/debug_options.cc -o CMakeFiles/alps.dir/common/debug_options.cc.s

src/CMakeFiles/alps.dir/common/debug_options.cc.o.requires:

.PHONY : src/CMakeFiles/alps.dir/common/debug_options.cc.o.requires

src/CMakeFiles/alps.dir/common/debug_options.cc.o.provides: src/CMakeFiles/alps.dir/common/debug_options.cc.o.requires
	$(MAKE) -f src/CMakeFiles/alps.dir/build.make src/CMakeFiles/alps.dir/common/debug_options.cc.o.provides.build
.PHONY : src/CMakeFiles/alps.dir/common/debug_options.cc.o.provides

src/CMakeFiles/alps.dir/common/debug_options.cc.o.provides.build: src/CMakeFiles/alps.dir/common/debug_options.cc.o


src/CMakeFiles/alps.dir/common/externalizable.cc.o: src/CMakeFiles/alps.dir/flags.make
src/CMakeFiles/alps.dir/common/externalizable.cc.o: ../src/common/externalizable.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/CMakeFiles/alps.dir/common/externalizable.cc.o"
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/alps.dir/common/externalizable.cc.o -c /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/src/common/externalizable.cc

src/CMakeFiles/alps.dir/common/externalizable.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/alps.dir/common/externalizable.cc.i"
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/src/common/externalizable.cc > CMakeFiles/alps.dir/common/externalizable.cc.i

src/CMakeFiles/alps.dir/common/externalizable.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/alps.dir/common/externalizable.cc.s"
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/src/common/externalizable.cc -o CMakeFiles/alps.dir/common/externalizable.cc.s

src/CMakeFiles/alps.dir/common/externalizable.cc.o.requires:

.PHONY : src/CMakeFiles/alps.dir/common/externalizable.cc.o.requires

src/CMakeFiles/alps.dir/common/externalizable.cc.o.provides: src/CMakeFiles/alps.dir/common/externalizable.cc.o.requires
	$(MAKE) -f src/CMakeFiles/alps.dir/build.make src/CMakeFiles/alps.dir/common/externalizable.cc.o.provides.build
.PHONY : src/CMakeFiles/alps.dir/common/externalizable.cc.o.provides

src/CMakeFiles/alps.dir/common/externalizable.cc.o.provides.build: src/CMakeFiles/alps.dir/common/externalizable.cc.o


src/CMakeFiles/alps.dir/common/error_stack.cc.o: src/CMakeFiles/alps.dir/flags.make
src/CMakeFiles/alps.dir/common/error_stack.cc.o: ../src/common/error_stack.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object src/CMakeFiles/alps.dir/common/error_stack.cc.o"
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/alps.dir/common/error_stack.cc.o -c /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/src/common/error_stack.cc

src/CMakeFiles/alps.dir/common/error_stack.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/alps.dir/common/error_stack.cc.i"
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/src/common/error_stack.cc > CMakeFiles/alps.dir/common/error_stack.cc.i

src/CMakeFiles/alps.dir/common/error_stack.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/alps.dir/common/error_stack.cc.s"
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/src/common/error_stack.cc -o CMakeFiles/alps.dir/common/error_stack.cc.s

src/CMakeFiles/alps.dir/common/error_stack.cc.o.requires:

.PHONY : src/CMakeFiles/alps.dir/common/error_stack.cc.o.requires

src/CMakeFiles/alps.dir/common/error_stack.cc.o.provides: src/CMakeFiles/alps.dir/common/error_stack.cc.o.requires
	$(MAKE) -f src/CMakeFiles/alps.dir/build.make src/CMakeFiles/alps.dir/common/error_stack.cc.o.provides.build
.PHONY : src/CMakeFiles/alps.dir/common/error_stack.cc.o.provides

src/CMakeFiles/alps.dir/common/error_stack.cc.o.provides.build: src/CMakeFiles/alps.dir/common/error_stack.cc.o


src/CMakeFiles/alps.dir/common/log.cc.o: src/CMakeFiles/alps.dir/flags.make
src/CMakeFiles/alps.dir/common/log.cc.o: ../src/common/log.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object src/CMakeFiles/alps.dir/common/log.cc.o"
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/alps.dir/common/log.cc.o -c /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/src/common/log.cc

src/CMakeFiles/alps.dir/common/log.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/alps.dir/common/log.cc.i"
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/src/common/log.cc > CMakeFiles/alps.dir/common/log.cc.i

src/CMakeFiles/alps.dir/common/log.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/alps.dir/common/log.cc.s"
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/src/common/log.cc -o CMakeFiles/alps.dir/common/log.cc.s

src/CMakeFiles/alps.dir/common/log.cc.o.requires:

.PHONY : src/CMakeFiles/alps.dir/common/log.cc.o.requires

src/CMakeFiles/alps.dir/common/log.cc.o.provides: src/CMakeFiles/alps.dir/common/log.cc.o.requires
	$(MAKE) -f src/CMakeFiles/alps.dir/build.make src/CMakeFiles/alps.dir/common/log.cc.o.provides.build
.PHONY : src/CMakeFiles/alps.dir/common/log.cc.o.provides

src/CMakeFiles/alps.dir/common/log.cc.o.provides.build: src/CMakeFiles/alps.dir/common/log.cc.o


src/CMakeFiles/alps.dir/common/os.cc.o: src/CMakeFiles/alps.dir/flags.make
src/CMakeFiles/alps.dir/common/os.cc.o: ../src/common/os.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object src/CMakeFiles/alps.dir/common/os.cc.o"
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/alps.dir/common/os.cc.o -c /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/src/common/os.cc

src/CMakeFiles/alps.dir/common/os.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/alps.dir/common/os.cc.i"
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/src/common/os.cc > CMakeFiles/alps.dir/common/os.cc.i

src/CMakeFiles/alps.dir/common/os.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/alps.dir/common/os.cc.s"
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/src/common/os.cc -o CMakeFiles/alps.dir/common/os.cc.s

src/CMakeFiles/alps.dir/common/os.cc.o.requires:

.PHONY : src/CMakeFiles/alps.dir/common/os.cc.o.requires

src/CMakeFiles/alps.dir/common/os.cc.o.provides: src/CMakeFiles/alps.dir/common/os.cc.o.requires
	$(MAKE) -f src/CMakeFiles/alps.dir/build.make src/CMakeFiles/alps.dir/common/os.cc.o.provides.build
.PHONY : src/CMakeFiles/alps.dir/common/os.cc.o.provides

src/CMakeFiles/alps.dir/common/os.cc.o.provides.build: src/CMakeFiles/alps.dir/common/os.cc.o


src/CMakeFiles/alps.dir/common/rich_backtrace.cc.o: src/CMakeFiles/alps.dir/flags.make
src/CMakeFiles/alps.dir/common/rich_backtrace.cc.o: ../src/common/rich_backtrace.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object src/CMakeFiles/alps.dir/common/rich_backtrace.cc.o"
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/alps.dir/common/rich_backtrace.cc.o -c /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/src/common/rich_backtrace.cc

src/CMakeFiles/alps.dir/common/rich_backtrace.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/alps.dir/common/rich_backtrace.cc.i"
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/src/common/rich_backtrace.cc > CMakeFiles/alps.dir/common/rich_backtrace.cc.i

src/CMakeFiles/alps.dir/common/rich_backtrace.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/alps.dir/common/rich_backtrace.cc.s"
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/src/common/rich_backtrace.cc -o CMakeFiles/alps.dir/common/rich_backtrace.cc.s

src/CMakeFiles/alps.dir/common/rich_backtrace.cc.o.requires:

.PHONY : src/CMakeFiles/alps.dir/common/rich_backtrace.cc.o.requires

src/CMakeFiles/alps.dir/common/rich_backtrace.cc.o.provides: src/CMakeFiles/alps.dir/common/rich_backtrace.cc.o.requires
	$(MAKE) -f src/CMakeFiles/alps.dir/build.make src/CMakeFiles/alps.dir/common/rich_backtrace.cc.o.provides.build
.PHONY : src/CMakeFiles/alps.dir/common/rich_backtrace.cc.o.provides

src/CMakeFiles/alps.dir/common/rich_backtrace.cc.o.provides.build: src/CMakeFiles/alps.dir/common/rich_backtrace.cc.o


src/CMakeFiles/alps.dir/layers/extentmap.cc.o: src/CMakeFiles/alps.dir/flags.make
src/CMakeFiles/alps.dir/layers/extentmap.cc.o: ../src/layers/extentmap.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object src/CMakeFiles/alps.dir/layers/extentmap.cc.o"
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/alps.dir/layers/extentmap.cc.o -c /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/src/layers/extentmap.cc

src/CMakeFiles/alps.dir/layers/extentmap.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/alps.dir/layers/extentmap.cc.i"
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/src/layers/extentmap.cc > CMakeFiles/alps.dir/layers/extentmap.cc.i

src/CMakeFiles/alps.dir/layers/extentmap.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/alps.dir/layers/extentmap.cc.s"
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/src/layers/extentmap.cc -o CMakeFiles/alps.dir/layers/extentmap.cc.s

src/CMakeFiles/alps.dir/layers/extentmap.cc.o.requires:

.PHONY : src/CMakeFiles/alps.dir/layers/extentmap.cc.o.requires

src/CMakeFiles/alps.dir/layers/extentmap.cc.o.provides: src/CMakeFiles/alps.dir/layers/extentmap.cc.o.requires
	$(MAKE) -f src/CMakeFiles/alps.dir/build.make src/CMakeFiles/alps.dir/layers/extentmap.cc.o.provides.build
.PHONY : src/CMakeFiles/alps.dir/layers/extentmap.cc.o.provides

src/CMakeFiles/alps.dir/layers/extentmap.cc.o.provides.build: src/CMakeFiles/alps.dir/layers/extentmap.cc.o


src/CMakeFiles/alps.dir/layers/sizeclass.cc.o: src/CMakeFiles/alps.dir/flags.make
src/CMakeFiles/alps.dir/layers/sizeclass.cc.o: ../src/layers/sizeclass.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object src/CMakeFiles/alps.dir/layers/sizeclass.cc.o"
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/src && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/alps.dir/layers/sizeclass.cc.o -c /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/src/layers/sizeclass.cc

src/CMakeFiles/alps.dir/layers/sizeclass.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/alps.dir/layers/sizeclass.cc.i"
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/src/layers/sizeclass.cc > CMakeFiles/alps.dir/layers/sizeclass.cc.i

src/CMakeFiles/alps.dir/layers/sizeclass.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/alps.dir/layers/sizeclass.cc.s"
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/src && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/src/layers/sizeclass.cc -o CMakeFiles/alps.dir/layers/sizeclass.cc.s

src/CMakeFiles/alps.dir/layers/sizeclass.cc.o.requires:

.PHONY : src/CMakeFiles/alps.dir/layers/sizeclass.cc.o.requires

src/CMakeFiles/alps.dir/layers/sizeclass.cc.o.provides: src/CMakeFiles/alps.dir/layers/sizeclass.cc.o.requires
	$(MAKE) -f src/CMakeFiles/alps.dir/build.make src/CMakeFiles/alps.dir/layers/sizeclass.cc.o.provides.build
.PHONY : src/CMakeFiles/alps.dir/layers/sizeclass.cc.o.provides

src/CMakeFiles/alps.dir/layers/sizeclass.cc.o.provides.build: src/CMakeFiles/alps.dir/layers/sizeclass.cc.o


# Object files for target alps
alps_OBJECTS = \
"CMakeFiles/alps.dir/common/assert_nd.cc.o" \
"CMakeFiles/alps.dir/common/assorted_func.cc.o" \
"CMakeFiles/alps.dir/common/debug_options.cc.o" \
"CMakeFiles/alps.dir/common/externalizable.cc.o" \
"CMakeFiles/alps.dir/common/error_stack.cc.o" \
"CMakeFiles/alps.dir/common/log.cc.o" \
"CMakeFiles/alps.dir/common/os.cc.o" \
"CMakeFiles/alps.dir/common/rich_backtrace.cc.o" \
"CMakeFiles/alps.dir/layers/extentmap.cc.o" \
"CMakeFiles/alps.dir/layers/sizeclass.cc.o"

# External object files for target alps
alps_EXTERNAL_OBJECTS =

src/libalps.so: src/CMakeFiles/alps.dir/common/assert_nd.cc.o
src/libalps.so: src/CMakeFiles/alps.dir/common/assorted_func.cc.o
src/libalps.so: src/CMakeFiles/alps.dir/common/debug_options.cc.o
src/libalps.so: src/CMakeFiles/alps.dir/common/externalizable.cc.o
src/libalps.so: src/CMakeFiles/alps.dir/common/error_stack.cc.o
src/libalps.so: src/CMakeFiles/alps.dir/common/log.cc.o
src/libalps.so: src/CMakeFiles/alps.dir/common/os.cc.o
src/libalps.so: src/CMakeFiles/alps.dir/common/rich_backtrace.cc.o
src/libalps.so: src/CMakeFiles/alps.dir/layers/extentmap.cc.o
src/libalps.so: src/CMakeFiles/alps.dir/layers/sizeclass.cc.o
src/libalps.so: src/CMakeFiles/alps.dir/build.make
src/libalps.so: third-party/libbacktrace/libbacktrace.a
src/libalps.so: third-party/yaml-cpp-0.5.2/libyaml-cpp.a
src/libalps.so: src/CMakeFiles/alps.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Linking CXX shared library libalps.so"
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/src && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/alps.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/CMakeFiles/alps.dir/build: src/libalps.so

.PHONY : src/CMakeFiles/alps.dir/build

src/CMakeFiles/alps.dir/requires: src/CMakeFiles/alps.dir/common/assert_nd.cc.o.requires
src/CMakeFiles/alps.dir/requires: src/CMakeFiles/alps.dir/common/assorted_func.cc.o.requires
src/CMakeFiles/alps.dir/requires: src/CMakeFiles/alps.dir/common/debug_options.cc.o.requires
src/CMakeFiles/alps.dir/requires: src/CMakeFiles/alps.dir/common/externalizable.cc.o.requires
src/CMakeFiles/alps.dir/requires: src/CMakeFiles/alps.dir/common/error_stack.cc.o.requires
src/CMakeFiles/alps.dir/requires: src/CMakeFiles/alps.dir/common/log.cc.o.requires
src/CMakeFiles/alps.dir/requires: src/CMakeFiles/alps.dir/common/os.cc.o.requires
src/CMakeFiles/alps.dir/requires: src/CMakeFiles/alps.dir/common/rich_backtrace.cc.o.requires
src/CMakeFiles/alps.dir/requires: src/CMakeFiles/alps.dir/layers/extentmap.cc.o.requires
src/CMakeFiles/alps.dir/requires: src/CMakeFiles/alps.dir/layers/sizeclass.cc.o.requires

.PHONY : src/CMakeFiles/alps.dir/requires

src/CMakeFiles/alps.dir/clean:
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/src && $(CMAKE_COMMAND) -P CMakeFiles/alps.dir/cmake_clean.cmake
.PHONY : src/CMakeFiles/alps.dir/clean

src/CMakeFiles/alps.dir/depend:
	cd /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/src /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/src /home/chenjuan/mnemosyne/mnemosyne-gcc/usermode/library/pmalloc/include/alps/build/src/CMakeFiles/alps.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/CMakeFiles/alps.dir/depend

