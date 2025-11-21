cmake_minimum_required(VERSION 3.16)
project(cimpl LANGUAGES C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(
    CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/
)
set(CIMPL_SOURCES
    src/cimpl.c
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

target_include_directories(cimpl PUBLIC ${PROJECT_SOURCE_DIR}/include/)
target_include_directories(cimpl_static PUBLIC ${PROJECT_SOURCE_DIR}/include/)

file(GLOB EXAMPLE_SOURCES "examples/*.c")
foreach(EXAMPLE_SOURCE ${EXAMPLE_SOURCES})
    get_filename_component(EXAMPLE_NAME ${EXAMPLE_SOURCE} NAME_WE)
    add_executable(${EXAMPLE_NAME} ${EXAMPLE_SOURCE})
    target_link_libraries(${EXAMPLE_NAME}
        # Uncomment to link compiled shared library
        #cimpl
        # Uncomment to link compiled static library
        #cimpl_static
        m
    )
    target_include_directories(${EXAMPLE_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include/)
endforeach()

file(GLOB TEST_SOURCES "tests/*.c")
foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    target_link_libraries(${TEST_NAME}
        # Uncomment to link compiled shared library
        #cimpl
        # Uncomment to link compiled static library
        #cimpl_static
        m
    )
    target_include_directories(${TEST_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/include/)
endforeach()
