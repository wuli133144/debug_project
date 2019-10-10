#openssl-1.0.1e
option(OPENSSL_ON "start build event" ON)
message(STATUS "=================${PROJECT_SOURCE_DIR}===================")
if(OPENSSL_ON)
    set(OPENSSL_FILES)
    set(OPENSSL_SRC_DIR openssl-1.0.1e)
    set(OPENSSL_FILE_DIR ${OPENSSL_SRC_DIR})

    set(openssl_dir ${CMAKE_CURRENT_SOURCE_DIR})
    get_filename_component(openssl_root_dir ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)

    add_custom_target(openssl ALL)
    add_custom_command(
        TARGET openssl
        PRE_BUILD  
        #COMMAND libtoolize -f
        COMMAND chmod +x config
        COMMAND ./config shared zlib  --prefix=${openssl_root_dir} 
        #COMMAND autoreconf
        #COMMAND automake --add-missing
        COMMAND make 
        COMMAND make install
        #COMMAND ln -sf ${PROJECT_SOURCE_DIR}/include/* ${PROJECT_SOURCE_DIR}/
        #COMMAND cp -rf ${PROJECT_SOURCE_DIR}/include/curl/lib/*  ${PROJECT_SOURCE_DIR}/lib 
        WORKING_DIRECTORY ${openssl_dir}/${OPENSSL_FILE_DIR}
        COMMENT "build curl==================="
    )    
endif(OPENSSL_ON)

#######################install curl########################

option(EVENT_ON "start build event" OFF)
message(STATUS "=================${PROJECT_SOURCE_DIR}===================")
if(EVENT_ON)
    set(EVENT_FILES)
    set(EVENT_SRC_DIR libevent-2.0.21-stable)
    set(EVENT_FILE_DIR ${EVENT_SRC_DIR})

    set(event_dir ${CMAKE_CURRENT_SOURCE_DIR})
    get_filename_component(event_root_dir ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)

    add_custom_target(event ALL)
    add_custom_command(
        TARGET event
        PRE_BUILD  
        #COMMAND libtoolize -f
        COMMAND chmod +x configure  
        COMMAND ./configure --prefix=${event_root_dir} --disable-static 
        #COMMAND autoreconf
        #COMMAND automake --add-missing
        COMMAND make 
        COMMAND make install
        #COMMAND ln -sf ${PROJECT_SOURCE_DIR}/include/* ${PROJECT_SOURCE_DIR}/
        #COMMAND cp -rf ${PROJECT_SOURCE_DIR}/include/curl/lib/*  ${PROJECT_SOURCE_DIR}/lib 
        WORKING_DIRECTORY ${event_dir}/${EVENT_FILE_DIR}
        COMMENT "build curl==================="
    )    
endif(EVENT_ON)

##########################end###############################

#######################install curl########################

option(CURL_ON "start build curl" OFF)
message(STATUS "=================${PROJECT_SOURCE_DIR}===================")
if(CURL_ON)
    set(CURL_FILES)
    set(CURL_SRC_DIR curl-7.57.0)
    set(CURL_FILE_DIR ${CURL_SRC_DIR})

    set(curl_dir ${CMAKE_CURRENT_SOURCE_DIR})
    get_filename_component(curl_root_dir ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)

    add_custom_target(curl ALL)
    add_custom_command(
        TARGET curl
        PRE_BUILD  
        COMMAND chmod +x buildconf 
        COMMAND ./buildconf 
        COMMAND ./configure --prefix=${curl_root_dir} --disable-static 
        COMMAND make 
        COMMAND make install
        #COMMAND ln -sf ${PROJECT_SOURCE_DIR}/include/* ${PROJECT_SOURCE_DIR}/
        #COMMAND cp -rf ${PROJECT_SOURCE_DIR}/include/curl/lib/*  ${PROJECT_SOURCE_DIR}/lib 
        WORKING_DIRECTORY ${curl_dir}/${CURL_FILE_DIR}
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
    set(json_dir ${CMAKE_CURRENT_SOURCE_DIR})
    get_filename_component(json_root_dir ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
    #set(CMAKE_INSTALL_PREFIX ${container_root_dir})
    message(STATUS "==json root===${json_root_dir}")
    add_custom_target(json ALL)
    add_custom_command(
        TARGET json
        PRE_BUILD 
        COMMAND libtoolize -f
        COMMAND chmod +x configure  
        COMMAND ./configure --prefix=${json_root_dir} --disable-static 
        COMMAND autoreconf
        #COMMAND mv configure.in configure.ac 
        COMMAND make 
        COMMAND make install
        #COMMAND ln -sf ${PROJECT_SOURCE_DIR}/curldest/include/* ${PROJECT_SOURCE_DIR}/
        #COMMAND cp -rf ${PROJECT_SOURCE_DIR}/include/json/lib/*  ${PROJECT_SOURCE_DIR}/lib 
        WORKING_DIRECTORY ${json_dir}/${JSON_SRC_DIR}
        COMMENT "=============build json===================="
    )    
endif(JSON_ON)

##########################end###############################


#######################install jemalloc########################

option(JEMALLOC_ON "start build JEmalloc" OFF)
message(STATUS "=================${PROJECT_SOURCE_DIR}===================")
if(JEMALLOC_ON)
    set(JEMALLOC_FILES)
    set(JEMALLOC_SRC_DIR jemalloc-4.2.1)
    set(JEMALLOC_FILE_DIR ${JEMALLOC_SRC_DIR})

    set(jemalloc_dir ${CMAKE_CURRENT_SOURCE_DIR})
    get_filename_component(jemalloc_root_dir ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
    message(STATUS "==========jemalloc=====${jemalloc_dir}")

    add_definitions(-DJEMALLOC_NO_DEMANGLE)
    add_custom_target(jemalloc ALL)
    add_custom_command(
        TARGET jemalloc
        PRE_BUILD
        COMMAND chmod -R 777 ${jemalloc_dir}/${JEMALLOC_FILE_DIR}
        COMMAND ./configure --prefix=${jemalloc_root_dir} --disable-static 
        COMMAND make 
        COMMAND make install
        #COMMAND ln -sf ${PROJECT_SOURCE_DIR}/curldest/include/* ${PROJECT_SOURCE_DIR}/
        #COMMAND cp -rf ${PROJECT_SOURCE_DIR}/include/json/lib/*  ${PROJECT_SOURCE_DIR}/lib 
        WORKING_DIRECTORY ${jemalloc_dir}/${JEMALLOC_SRC_DIR}
        COMMENT "build JEMALLOC===================="
    )    
endif(JEMALLOC_ON)

##########################end###############################

#add_subdirectory(libevent)
#add_subdirectory(openssl)
#add_subdirectory(json-c-0.9)
#add_subdirectory(jemalloc-4.2.1)
