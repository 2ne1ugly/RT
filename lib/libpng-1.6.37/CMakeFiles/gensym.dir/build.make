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

# Utility rule file for gensym.

# Include the progress variables for this target.
include CMakeFiles/gensym.dir/progress.make

CMakeFiles/gensym: libpng.sym


libpng.sym: scripts/sym.out
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/nfs/2018/z/zfaria/Desktop/projects/rt/lib/libpng-1.6.37/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating libpng.sym"
	/nfs/2018/z/zfaria/.brew/Cellar/cmake/3.13.2/bin/cmake -E remove /nfs/2018/z/zfaria/Desktop/projects/rt/lib/libpng-1.6.37/libpng.sym
	/nfs/2018/z/zfaria/.brew/Cellar/cmake/3.13.2/bin/cmake -E copy /nfs/2018/z/zfaria/Desktop/projects/rt/lib/libpng-1.6.37/scripts/sym.out /nfs/2018/z/zfaria/Desktop/projects/rt/lib/libpng-1.6.37/libpng.sym

scripts/sym.out: scripts/sym.c
scripts/sym.out: pnglibconf.h
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/nfs/2018/z/zfaria/Desktop/projects/rt/lib/libpng-1.6.37/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Generating scripts/sym.out"
	/nfs/2018/z/zfaria/.brew/Cellar/cmake/3.13.2/bin/cmake -DINPUT=/nfs/2018/z/zfaria/Desktop/projects/rt/lib/libpng-1.6.37/scripts/sym.c -DOUTPUT=/nfs/2018/z/zfaria/Desktop/projects/rt/lib/libpng-1.6.37/scripts/sym.out -P /nfs/2018/z/zfaria/Desktop/projects/rt/lib/libpng-1.6.37/scripts/genout.cmake

pnglibconf.h: pnglibconf.out
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/nfs/2018/z/zfaria/Desktop/projects/rt/lib/libpng-1.6.37/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Generating pnglibconf.h"
	/nfs/2018/z/zfaria/.brew/Cellar/cmake/3.13.2/bin/cmake -DOUTPUT=pnglibconf.h -P /nfs/2018/z/zfaria/Desktop/projects/rt/lib/libpng-1.6.37/scripts/gensrc.cmake

pnglibconf.out: pnglibconf.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/nfs/2018/z/zfaria/Desktop/projects/rt/lib/libpng-1.6.37/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Generating pnglibconf.out"
	/nfs/2018/z/zfaria/.brew/Cellar/cmake/3.13.2/bin/cmake -DINPUT=/nfs/2018/z/zfaria/Desktop/projects/rt/lib/libpng-1.6.37/pnglibconf.c -DOUTPUT=/nfs/2018/z/zfaria/Desktop/projects/rt/lib/libpng-1.6.37/pnglibconf.out -P /nfs/2018/z/zfaria/Desktop/projects/rt/lib/libpng-1.6.37/scripts/genout.cmake

pnglibconf.c: scripts/pnglibconf.dfa
pnglibconf.c: scripts/options.awk
pnglibconf.c: pngconf.h
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/nfs/2018/z/zfaria/Desktop/projects/rt/lib/libpng-1.6.37/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Generating pnglibconf.c"
	/nfs/2018/z/zfaria/.brew/Cellar/cmake/3.13.2/bin/cmake -DOUTPUT=pnglibconf.c -P /nfs/2018/z/zfaria/Desktop/projects/rt/lib/libpng-1.6.37/scripts/gensrc.cmake

gensym: CMakeFiles/gensym
gensym: libpng.sym
gensym: scripts/sym.out
gensym: pnglibconf.h
gensym: pnglibconf.out
gensym: pnglibconf.c
gensym: CMakeFiles/gensym.dir/build.make

.PHONY : gensym

# Rule to build all files generated by this target.
CMakeFiles/gensym.dir/build: gensym

.PHONY : CMakeFiles/gensym.dir/build

CMakeFiles/gensym.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/gensym.dir/cmake_clean.cmake
.PHONY : CMakeFiles/gensym.dir/clean

CMakeFiles/gensym.dir/depend:
	cd /nfs/2018/z/zfaria/Desktop/projects/rt/lib/libpng-1.6.37 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /nfs/2018/z/zfaria/Desktop/projects/rt/lib/libpng-1.6.37 /nfs/2018/z/zfaria/Desktop/projects/rt/lib/libpng-1.6.37 /nfs/2018/z/zfaria/Desktop/projects/rt/lib/libpng-1.6.37 /nfs/2018/z/zfaria/Desktop/projects/rt/lib/libpng-1.6.37 /nfs/2018/z/zfaria/Desktop/projects/rt/lib/libpng-1.6.37/CMakeFiles/gensym.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/gensym.dir/depend

