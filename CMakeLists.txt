cmake_minimum_required(VERSION 2.8)
project(test_var)

message("start compile this prj")
#include(mk/util.cmake)
#init_platform()
project (libobject)
#config_platform()

# The version number.
set (LIBOBJECT_VERSION_MAJOR 1)
set (LIBOBJECT_VERSION_MINOR 0)
# Set the output folder where your program will be created
set (CMAKE_MACOSX_RPATH 1)

#display_configs("${PLATFORM}")

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
set(current_dir ${CMAKE_CURRENT_SOURCE_DIR})
set(root_dir)
set(directory_inc "${current_dir}/include")
get_filename_component(root_dir ${CMAKE_SOURCE_DIR} DIRECTORY)

add_subdirectory(container)
add_subdirectory(3rd)
#add_subdirectory(db)
#add_subdirectory(protobuf)
