cmake_minimum_required(VERSION 2.6)
project(test_var)

message("start compile this prj")

#set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
set(REGISTER_INCDIR ${PROJECT_SOURCE_DIR})
set(REGISTER_SOURCE 
           ctor.c
           dtor.c 
           reg_heap.c 
           registry.c)
set(REGISTER_HEADERFILES 
    ${PROJECT_SOURCE_DIR}/registry/reg_heap.h 
    ${PROJECT_SOURCE_DIR}/registry/registry.h)

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

include_directories(${REGISTER_INCDIR})

add_executable(test main.c ${REGISTER_SOURCE})

target_link_libraries(test cunit m pthread)

#add_custom_target(libngwf_db.so ALL)

# set(PROTOBUFFILES)

# file(GLOB_RECURSE SOURCE_FILES ${PROJECT_SOURCE_DIR}/protobuf/*.proto)

# foreach(current_files ${SOURCE_FILES})
#     message(STATUS "========${current_files}========")    
#     set(PROTOBUF_FILES)
#     #add_custom_target(ngwf_db ALL DEPENDS ${PROTOBUF_FILES})
#     add_custom_command(OUTPUT ${PROTOBUF_FILES}
#         PRE_BUILD
#         COMMAND protoc -I=${PROJECT_SOURCE_DIR}/protobuf --cpp_out=${PROTOBUF_FILES} ${current_files}
#         DEPENDS ${current_files}
#         COMMENT "protoc build success"
#     )
    
#     list(APPEND PROTOBUFFILES ${PROTOBUF_FILES})
# endforeach()
# add_custom_target(ngwf_d1b ALL DEPENDS ${PROTOBUFFILES})    

add_library(ngwf_db SHARED ${REGISTER_SOURCE})
target_link_libraries(ngwf_db cunit m pthread)

#add_custom_target()
#add_custom_target() ,执行一条命令

#foreach 

#test protoc 
#add_custom_target(ngwf_db DEPENDS ${PROTOBUFILES})


install(TARGETS ngwf_db test  
        LIBRARY DESTINATION /usr/lib
        RUNTIME DESTINATION bin)

install(FILES ${REGISTER_HEADERFILES} DESTINATION /usr/include/registry)