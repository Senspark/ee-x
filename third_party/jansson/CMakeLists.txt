cmake_minimum_required(VERSION 3.1)

project(jansson)

set(LIB_SRC 
    dump.c
    error.c
    hashtable.c
    load.c
    memory.c
    pack_unpack.c
    strbuffer.c
    strconv.c
    utf.c
    value.c
)

set(LIB_HEADERS
    hashtable.h
    jansson_config.h
    jansson_private.h
    jansson.h
    strbuffer.h
    utf.h
)

add_library(${PROJECT_NAME} STATIC ${LIB_SRC} ${LIB_HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC
    .
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)



