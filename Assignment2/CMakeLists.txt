cmake_minimum_required(VERSION 3.12)
project(RandDistributions)
set(SOURCE_FILES main.cpp)
add_executable(RandDistributions ${SOURCE_FILES})
set_property(TARGET RandDistributions PROPERTY CXX_STANDARD 20)

find_program(CLANG_FORMAT "clang-format")

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(RandDistributions PRIVATE /W4 /permissive-)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(RandDistributions PRIVATE -Wall -Wextra -pedantic)    
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(RandDistributions PRIVATE -Wall -Wextra -pedantic)
endif()


if(CLANG_FORMAT)
	message("clang-format is located at: " ${CLANG_FORMAT})

    unset(SOURCE_FILES_PATHS)
    foreach(SOURCE_FILE ${SOURCE_FILES})
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
	add_dependencies(RandDistributions ClangFormat)
else()
	message("Clang format not found")
endif()
