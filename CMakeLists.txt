cmake_minimum_required(VERSION 2.6)
project(test_var)

message("start compile this prj")

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
set(REGISTER_INCDIR ${PROJECT_SOURCE_DIR})
set(SOURCE_INC_DIR ${REGISTER_INCDIR}/include)
set(REGISTER_SOURCE 
           ctor.c
           dtor.c 
           reg_heap.c 
           registry.c)
set(REGISTER_HEADERFILES 
    ${PROJECT_SOURCE_DIR}/registry/reg_heap.h 
    ${PROJECT_SOURCE_DIR}/registry/registry.h)

set(PROJECT_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(PROJECT_LIB)

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

include_directories(${REGISTER_INCDIR} ${SOURCE_INC_DIR})

file(GLOB_RECURSE libraries ${PROJECT_LIB_DIR}/*.so)
foreach(l ${libraries})
    list(APPEND PROJECT_LIB ${l})
endforeach()

add_executable(test main.c ${REGISTER_SOURCE})

target_link_libraries(test cunit m pthread ${PROJECT_LIB})

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
#######################install curl########################

option(CURL_ON "start build curl" OFF)
message(STATUS "=================${PROJECT_SOURCE_DIR}===================")
if(CURL_ON)
    set(CURL_FILES)
    set(CURL_SRC_DIR curl-7.57.0)
    set(CURL_FILE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd/${CURL_SRC_DIR})

    add_custom_target(curl ALL)
    add_custom_command(
        TARGET curl
        PRE_BUILD  
        COMMAND chmod +x buildconf 
        COMMAND ./buildconf 
        COMMAND ./configure --prefix=${PROJECT_SOURCE_DIR} --disable-static 
        COMMAND make 
        COMMAND make install
        #COMMAND ln -sf ${PROJECT_SOURCE_DIR}/include/* ${PROJECT_SOURCE_DIR}/
        #COMMAND cp -rf ${PROJECT_SOURCE_DIR}/include/curl/lib/*  ${PROJECT_SOURCE_DIR}/lib 
        WORKING_DIRECTORY ${CURL_FILE_DIR}
        COMMENT "build curl===================="
    )    
endif(CURL_ON)

##########################end###############################

#######################install json-c########################

option(JSON_ON "start build json" OFF)
message(STATUS "=================${PROJECT_SOURCE_DIR}===================")
if(JSON_ON)
    set(JSON_FILES)
    set(JSON_SRC_DIR json-c-0.9)
    set(JSON_FILE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd/${JSON_SRC_DIR})

    add_custom_target(json ALL)
    add_custom_command(
        TARGET json
        PRE_BUILD 
        COMMAND chmod +x configure  
        COMMAND ./configure --prefix=${PROJECT_SOURCE_DIR} --disable-static 
        COMMAND make 
        COMMAND make install
        #COMMAND ln -sf ${PROJECT_SOURCE_DIR}/curldest/include/* ${PROJECT_SOURCE_DIR}/
        #COMMAND cp -rf ${PROJECT_SOURCE_DIR}/include/json/lib/*  ${PROJECT_SOURCE_DIR}/lib 
        WORKING_DIRECTORY ${JSON_FILE_DIR}
        COMMENT "build json===================="
    )    
endif(JSON_ON)

##########################end###############################



install(TARGETS ngwf_db test  
        LIBRARY DESTINATION /usr/lib
        RUNTIME DESTINATION bin)

install(FILES ${REGISTER_HEADERFILES} DESTINATION /usr/include/registry)