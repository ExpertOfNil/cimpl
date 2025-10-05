cmake_minimum_required(VERSION 3.16)
project(cimpl LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(
    CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/
)
set(CIMPL_SOURCES
    src/cimpl_network.c
    src/cimpl_serial.c
    src/cimpl_string.c
)

add_library(cimpl SHARED ${CIMPL_SOURCES})
add_library(cimpl_static STATIC ${CIMPL_SOURCES})

set_target_properties(cimpl_static PROPERTIES OUTPUT_NAME cimpl)
set_target_properties(cimpl PROPERTIES
    VERSION 0.1.0
    SOVERSION 0
)
set_target_properties(cimpl cimpl_static PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib
    ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib
)

target_include_directories(cimpl PUBLIC src/)
target_include_directories(cimpl_static PUBLIC src/)

file(GLOB EXAMPLE_SOURCES "examples/*.c")
foreach(EXAMPLE_SOURCE ${EXAMPLE_SOURCES})
    get_filename_component(EXAMPLE_NAME ${EXAMPLE_SOURCE} NAME_WE)
    add_executable(${EXAMPLE_NAME} ${EXAMPLE_SOURCE})
    target_link_libraries(${EXAMPLE_NAME} cimpl)
    target_include_directories(${EXAMPLE_NAME} PRIVATE src/)
endforeach()
