
#project(container)
#cmake_minimum_required(VERSION 2.8)
set(container_dir ${CMAKE_CURRENT_SOURCE_DIR})
get_filename_component(container_root_dir ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)

set(CMAKE_INSTALL_PREFIX ${container_root_dir})

if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else(MSVC)
    # lots of warnings and all warnings as errors
    if(CMAKE_COMPILER_IS_GNUCC)
        # add compile paramater 
        add_compile_options(
        -std=gnu99 -g -fPIC -Wall -Wextra
        -Wno-unused-but-set-variable
        -Wno-return-type
        -Wno-unused-variable
        -Wno-unused-function
        -Wno-char-subscripts
        -Wno-unused-label
        -Wno-return-type
        -Wno-sign-compare
        -Wno-pointer-to-int-cast
        #-Wno-dev
        -fno-builtin
        #-Werror
        #-Wno-sometimes-uninitialized
        #-Wno-sometimes-uninitialized
        -Wno-uninitialized
        #-Wno-int-conversion
        -Wno-implicit-function-declaration
        -Wno-uninitialized
        -Wno-unused-parameter
        -Wno-missing-field-initializers
        -Wno-implicit-function-declaration)
    else(CMAKE_COMPILER_IS_GNUCC)
        message(ERROR_FATAL,"compiler_c not support exit")
    endif(CMAKE_COMPILER_GNUCC)
endif(MSVC)


include_directories(
                    ${container_dir}/include/ 
                    ${container_dir}/include/libobject
                    )

message(STATUS "=====directory:${container_dir}==========")
message(STATUS "=====directory_root:${container_root_dir}==========")
file(GLOB_RECURSE Source_Files ${container_dir}/*.c)
#message(STATUS "=====container:${Source_Files}==========")
add_library(container SHARED  ${Source_Files})
#include_directories(${container_dir}/include)
#set_target_properties(container PROPERTIES LINKER_LANGUAGE C)

install(TARGETS container  LIBRARY DESTINATION lib)
install(DIRECTORY ${container_dir}/include/ DESTINATION include)
                    