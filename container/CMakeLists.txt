
project(container)
set(container_dir ${CMAKE_CURRENT_SOURCE_DIR})
get_filename_component(container_root_dir ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)

set(Source_Files)
include_directories(
                    ${container_dir}/include/ 
                    ${container_dir}/include/libobject
                    ${container_dir}/include/libobject/core 
                    ${container_dir}/include/libobject/io
                    ${container_dir}/include/libobject/system  
                    )

file(GLOB_RECURSE ${Source_Files} ${container_dir}/*.c)
add_library(container SHARE ${Source_Files})
#include_directories(${container_dir}/include)

install(TARGETS container  LIBRARY DESTINATION lib)
install(DIRECTORY ${container_dir}/include/ DESTINATION include)
                    