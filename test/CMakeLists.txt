project(test)
cmake_minimum_required(VERSION 2.8)

set(current_dir ${CMAKE_CURRENT_SOURCE_DIR})
set(root_dir)
set(directory_inc "${current_dir}/include")
get_filename_component(root_dir ${CMAKE_SOURCE_DIR} DIRECTORY)


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


add_definitions(-DLINUX_USER_MODE)

include_directories(${root_dir}/include)

link_directories(${root_dir}/lib)

aux_source_directory(./ LIST_SOURCE)

add_executable(test ${LIST_SOURCE})

target_link_libraries(test json event container jemalloc ssl m pthread)