cmake_minimum_required(VERSION 2.4.6)
include($ENV{ROS_ROOT}/core/rosbuild/rosbuild.cmake)

# Set the build type.  Options are:
#  Coverage       : w/ debug symbols, w/o optimization, w/ code-coverage
#  Debug          : w/ debug symbols, w/o optimization
#  Release        : w/o debug symbols, w/ optimization
#  RelWithDebInfo : w/ debug symbols, w/ optimization
#  MinSizeRel     : w/o debug symbols, w/ optimization, stripped binaries
set(ROS_BUILD_TYPE RelWithDebInfo)

rosbuild_init()

# Set the default path for built executables to the "bin" directory.
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# Set the default path for built libraries to the "lib" directory.
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

# Uncomment if you have defined messages.
rosbuild_genmsg()

include_directories($ENV{ROS_ROOT}/std_msgs/msg_gen/cpp/include)

# Common commands for building c++ executables and libraries.
rosbuild_add_executable(doubleInt32 src/doubleInt32.cpp)
rosbuild_add_executable(allSimpleTypes src/allSimpleTypes.cpp)
rosbuild_add_executable(complexStructure src/complexStructure.cpp)
rosbuild_add_executable(stressTest src/stressTest.cpp)
rosbuild_add_executable(simpleReplay src/simpleReplay.cpp)
rosbuild_add_executable(complexReplay src/complexReplay.cpp)

