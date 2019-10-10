cmake_minimum_required(VERSION 2.6)
project(test_var)

message("start compile this prj")

init_platform()
project (libobject)
config_platform()

# The version number.
set (LIBOBJECT_VERSION_MAJOR 1)
set (LIBOBJECT_VERSION_MINOR 0)
# Set the output folder where your program will be created
set (CMAKE_MACOSX_RPATH 1)

display_configs("${PLATFORM}")

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
set(current_dir ${CMAKE_CURRENT_SOURCE_DIR})
set(root_dir)
set(directory_inc "${current_dir}/include")
if(NOT EXISTS ${directory_inc})
    #message(FATAL_ERROR "not find include header files")
    message(STATUS "not find include header files")
endif()

get_filename_component(root_dir ${CMAKE_SOURCE_DIR} DIRECTORY)


if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    if(CMAKE_COMPILER_IS_GNUCC)
        # add compile paramater 
        add_compile_options( 
        -std=gnu99 -g -fPIC -Wall -Wextra
        -Wno-unused-but-set-variable 
        -Wno-unused-variable
        -Wno-unused-function
        -Wno-char-subscripts
        -Wno-unused-label
        -Wno-return-type
        -Wno-uninitialized
        -Wno-unused-parameter
        -Wno-missing-field-initializers
        -Wno-implicit-function-declaration)
    else()
        message(ERROR_FATAL,"compiler_c not support exit")
    endif(CMAKE_COMPILER_GNUCC)
endif(MSVC)

add_subdirectory(container)
add_subdirectory(3rd)
add_subdirectory(db)
add_subdirectory(protobuf)
