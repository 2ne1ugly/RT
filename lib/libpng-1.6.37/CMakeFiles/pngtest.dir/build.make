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
CMAKE_COMMAND = /nfs/2018/z/zfaria/.brew/Cellar/cmake/3.13.2/bin/cmake

# The command to remove a file.
RM = /nfs/2018/z/zfaria/.brew/Cellar/cmake/3.13.2/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /nfs/2018/z/zfaria/Desktop/projects/rt/lib/libpng-1.6.37

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /nfs/2018/z/zfaria/Desktop/projects/rt/lib/libpng-1.6.37

# Include any dependencies generated for this target.
include CMakeFiles/pngtest.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/pngtest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/pngtest.dir/flags.make

CMakeFiles/pngtest.dir/pngtest.c.o: CMakeFiles/pngtest.dir/flags.make
CMakeFiles/pngtest.dir/pngtest.c.o: pngtest.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/nfs/2018/z/zfaria/Desktop/projects/rt/lib/libpng-1.6.37/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/pngtest.dir/pngtest.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/pngtest.dir/pngtest.c.o   -c /nfs/2018/z/zfaria/Desktop/projects/rt/lib/libpng-1.6.37/pngtest.c

CMakeFiles/pngtest.dir/pngtest.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/pngtest.dir/pngtest.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /nfs/2018/z/zfaria/Desktop/projects/rt/lib/libpng-1.6.37/pngtest.c > CMakeFiles/pngtest.dir/pngtest.c.i

CMakeFiles/pngtest.dir/pngtest.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/pngtest.dir/pngtest.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /nfs/2018/z/zfaria/Desktop/projects/rt/lib/libpng-1.6.37/pngtest.c -o CMakeFiles/pngtest.dir/pngtest.c.s

# Object files for target pngtest
pngtest_OBJECTS = \
"CMakeFiles/pngtest.dir/pngtest.c.o"

# External object files for target pngtest
pngtest_EXTERNAL_OBJECTS =

pngtest: CMakeFiles/pngtest.dir/pngtest.c.o
pngtest: CMakeFiles/pngtest.dir/build.make
pngtest: libpng16.16.37.0.dylib
pngtest: /usr/lib/libz.dylib
pngtest: CMakeFiles/pngtest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/nfs/2018/z/zfaria/Desktop/projects/rt/lib/libpng-1.6.37/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable pngtest"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/pngtest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/pngtest.dir/build: pngtest

.PHONY : CMakeFiles/pngtest.dir/build

CMakeFiles/pngtest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/pngtest.dir/cmake_clean.cmake
.PHONY : CMakeFiles/pngtest.dir/clean

CMakeFiles/pngtest.dir/depend:
	cd /nfs/2018/z/zfaria/Desktop/projects/rt/lib/libpng-1.6.37 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /nfs/2018/z/zfaria/Desktop/projects/rt/lib/libpng-1.6.37 /nfs/2018/z/zfaria/Desktop/projects/rt/lib/libpng-1.6.37 /nfs/2018/z/zfaria/Desktop/projects/rt/lib/libpng-1.6.37 /nfs/2018/z/zfaria/Desktop/projects/rt/lib/libpng-1.6.37 /nfs/2018/z/zfaria/Desktop/projects/rt/lib/libpng-1.6.37/CMakeFiles/pngtest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/pngtest.dir/depend

