project(test)
cmake_minimum_required(VERSION 2.8)

set(current_dir ${CMAKE_CURRENT_SOURCE_DIR})
set(root_dir)
set(directory_inc "${current_dir}/include")
get_filename_component(root_dir ${CMAKE_SOURCE_DIR} DIRECTORY)

include_directories(${root_dir}/include)

link_directories(${root_dir}/lib)

aux_source_directory(./ LIST_SOURCE)

add_executable(test ${LIST_SOURCE})

target_link_libraries(test json event object jemalloc ssl)