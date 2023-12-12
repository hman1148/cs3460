cmake_minimum_required(VERSION 3.12)

set(PROJECT_NAME ArrayPerformance)
project(ArrayPerformance)

set(SOURCE_FILES 
    sortutils.cpp)

set(HEADER_FILES sortutils.hpp)
set(UNIT_TEST_FILES TestPerformance.cpp)

set(PROJECT_NAME ArrayPerformance)
set(UNIT_TEST_RUNNER UnitTestRunner)

## MAIN TARGET 
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} main.cpp)
add_executable(${UNIT_TEST_RUNNER} ${HEADER_FILES} ${SOURCE_FILES} ${UNIT_TEST_FILES})

# Use standard C++ 20
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${UNIT_TEST_RUNNER} PROPERTY CXX_STANDARD 20)

find_program(CLANG_FORMAT "clang-format")

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/STACK:10000000")
    set_target_properties(${UNIT_TEST_RUNNER} PROPERTIES LINK_FLAGS "/STACK:10000000")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Wl,--stack,10000000)
    target_compile_options(${UNIT_TEST_RUNNER} PRIVATE -Wall -Wextra -pedantic -Wl,--stack,10000000)

elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

if(CLANG_FORMAT)
	message("clang-format is located at: " ${CLANG_FORMAT})

    unset(SOURCE_FILES_PATHS)
    foreach(SOURCE_FILE ${SOURCE_FILES} ${HEADER_FILES} ${UNIT_TEST_FILES})
        get_source_file_property(WHERE ${SOURCE_FILE} LOCATION)
        set(SOURCE_FILES_PATHS ${SOURCE_FILES_PATHS} ${WHERE})
    endforeach()

    add_custom_target(
        ClangFormat
        COMMAND ${CLANG_FORMAT}
        -i
        -style=file
        ${SOURCE_FILES_PATHS}
    )
	add_dependencies(${PROJECT_NAME} ClangFormat)
else()
	message("Clang format not found")
endif()



# Add Google Test
include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY      https://github.com/google/googletest.git
    GIT_TAG             v1.14.0
    )

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

target_link_libraries(${UNIT_TEST_RUNNER} gtest_main)