cmake_minimum_required(VERSION 2.6)
project(test_var)

message("start compile this prj")


include (mk/util.cmake)
init_platform()
project (libobject)
config_platform()

# The version number.
set (LIBOBJECT_VERSION_MAJOR 1)
set (LIBOBJECT_VERSION_MINOR 0)
# Set the output folder where your program will be created
set (CMAKE_MACOSX_RPATH 1)
# Debug/Release settings
#set (CMAKE_BUILD_TYPE "Debug")

display_configs("${PLATFORM}")

#find_package(PkgConfig)
#find_package(opengl)
#find_package(CURL)

option (MODULE_UI "ui module" OFF)
option (MODULE_BUS "bus module" OFF)



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

set(PROJECT_LIB_DIR ${PROJECT_SOURCE_DIR}/lib)
set(PROJECT_LIB cunit m pthread)

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
#link_libraries()
file(GLOB libraries ${PROJECT_LIB_DIR}/*.so)
foreach(l ${libraries})
    #list(APPEND PROJECT_LIB ${l})
    link_libraries(${l})
    message(STATUS "=====================lib ${l}=====================")
endforeach()

link_directories(${PROJECT_SOURCE_DIR}/lib/)
add_executable(test main.c ${REGISTER_SOURCE})

target_link_libraries(test  ${PROJECT_LIB})

 
add_library(ngwf_db SHARED ${REGISTER_SOURCE})
target_link_libraries(ngwf_db cunit m pthread)


#foreach 

######################libobject#####################


find_source_files(Source_Files)
find_main_file(Main_File)

if (Main_File)
    list(REMOVE_ITEM Source_Files ${Main_File} )
    message("-- Main file path: ${Main_File}")
endif ()

if (MODULE_UI)
    message("-- Turn ui module on")
    #find_package(SDL2 REQUIRED)
    SET(ExternalLibs ${ExternalLibs} SDL2 SDL2_ttf)
else ()
    message(WARNING "-- Turn ui module off-----------")
    find_module_files(Exclude_UI_Module_Files ui)
    exclude_files(Source_Files "${Source_Files}" "${Exclude_UI_Module_Files}")
endif ()

if (MODULE_BUS)
    message("-- Turn bus module on")
    find_package(SDL2 REQUIRED)
else ()
    message("-- Turn bus module off")
    find_module_files(Exclude_BUS_Module_Files bus)
    exclude_files(Source_Files "${Source_Files}" "${Exclude_BUS_Module_Files}")
endif ()

ADD_LIBRARY(object SHARED ${Source_Files})

if(EXISTS "${ExternalLibs}")
    message(STATUS "depend_lib:${ExternalLibs}")
    target_link_libraries(object ${ExternalLibs})
else()
    message(STATUS "not found depend_lib ExternalLibs")
    SET(ExternalLibs ${ExternalLibs} pthread m json)
    target_link_libraries(object ${ExternalLibs})
endif(EXISTS "${ExternalLibs}")

#add_executable(test_object ${Main_File})
#target_link_libraries(test_object object)

SET_TARGET_PROPERTIES (object PROPERTIES VERSION 1.2 SOVERSION 1)

#install (TARGETS test_object DESTINATION bin)
install (DIRECTORY ${LIBRARY_DIR_PATH} DESTINATION lib)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/src/include/libobject DESTINATION include)
#####################################################


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


#######################install jemalloc########################

option(JEMALLOC_ON "start build JEmalloc" OFF)
message(STATUS "=================${PROJECT_SOURCE_DIR}===================")
if(JEMALLOC_ON)
    set(JEMALLOC_FILES)
    set(JEMALLOC_SRC_DIR jemalloc-4.2.1)
    set(JEMALLOC_FILE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd/${JEMALLOC_SRC_DIR})
    add_definitions(-DJEMALLOC_NO_DEMANGLE)
    add_custom_target(jemalloc ALL)
    add_custom_command(
        TARGET jemalloc
        PRE_BUILD
        COMMAND chmod -R 777 ${JEMALLOC_FILE_DIR}
        COMMAND ./configure --prefix=${PROJECT_SOURCE_DIR} --disable-static 
        COMMAND make 
        COMMAND make install
        #COMMAND ln -sf ${PROJECT_SOURCE_DIR}/curldest/include/* ${PROJECT_SOURCE_DIR}/
        #COMMAND cp -rf ${PROJECT_SOURCE_DIR}/include/json/lib/*  ${PROJECT_SOURCE_DIR}/lib 
        WORKING_DIRECTORY ${JEMALLOC_FILE_DIR}
        COMMENT "build JEMALLOC===================="
    )    
endif(JEMALLOC_ON)

##########################end###############################


install(TARGETS ngwf_db test  
        LIBRARY DESTINATION /usr/lib
        RUNTIME DESTINATION bin)

install(FILES ${REGISTER_HEADERFILES} DESTINATION /usr/include/registry)