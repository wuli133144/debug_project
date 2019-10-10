
project(container)
set(container_dir ${CMAKE_CURRENT_SOURCE_DIR})
get_filename_component(container_root_dir ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)


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
        -Wno-unused-variable
        -Wno-unused-function
        -Wno-char-subscripts
        -Wno-unused-label
        -Wno-return-type
        -Wno-dev 
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
                    ${container_dir}/include/libobject/core 
                    ${container_dir}/include/libobject/io
                    ${container_dir}/include/libobject/system  
                    )

message(STATUS "=====directory:${container_dir}==========")
file(GLOB_RECURSE Source_Files ${container_dir}/*.c)
message(STATUS "=====container:${Source_Files}==========")
add_library(container SHARED  ${Source_Files})
#include_directories(${container_dir}/include)
set_target_properties(container PROPERTIES LINKER_LANGUAGE C)

install(TARGETS container  LIBRARY DESTINATION lib)
install(DIRECTORY ${container_dir}/include/ DESTINATION include)
                    