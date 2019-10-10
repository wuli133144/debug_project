

#######################install curl########################

option(CURL_ON "start build curl" OFF)
message(STATUS "=================${PROJECT_SOURCE_DIR}===================")
if(CURL_ON)
    set(CURL_FILES)
    set(CURL_SRC_DIR curl-7.57.0)
    set(CURL_FILE_DIR ${CURL_SRC_DIR})

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
    set(JSON_FILE_DIR ${JSON_SRC_DIR})

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
    set(JEMALLOC_FILE_DIR ${JEMALLOC_SRC_DIR})
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

#add_subdirectory(libevent)
#add_subdirectory(openssl)
#add_subdirectory(json-c-0.9)
#add_subdirectory(jemalloc-4.2.1)
