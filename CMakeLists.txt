# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(StudentManagementSystem
    VERSION 1.0.0
    DESCRIPTION "Student Management System"
    LANGUAGES CXX
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Display current source directory
message(STATUS "Source directory: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "Build directory: ${CMAKE_BINARY_DIR}")

# ==================== Compiler Configuration ====================
# MSVC settings
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    message(STATUS "Compiler: MSVC")
    add_compile_options(/W4 /MP /utf-8)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(/O2)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(/Zi /Od)
    endif()

# GCC/MinGW settings
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(STATUS "Compiler: GCC/MinGW")
    add_compile_options(-Wall -Wextra)
    
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(-O2)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-g -O0)
    endif()
    
    # MinGW specific Unicode support
    if(WIN32)
        add_compile_options(-municode)
        add_link_options(-municode)
    endif()

# Clang settings
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message(STATUS "Compiler: Clang")
    add_compile_options(-Wall -Wextra)
    
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(-O2)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-g -O0)
    endif()
endif()

# Windows specific definitions
if(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0A00)
endif()

# ==================== Project Configuration ====================
# List source files explicitly to avoid GLOB issues
set(SOURCE_FILES
    main.cpp
    src/student.cpp
    src/io.cpp
)

set(HEADER_FILES
    include/student.h
    include/io.h
)

# Check if files exist
foreach(file ${SOURCE_FILES})
    if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${file})
        message(WARNING "Source file not found: ${file}")
    else()
        message(STATUS "Found source file: ${file}")
    endif()
endforeach()

foreach(file ${HEADER_FILES})
    if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${file})
        message(WARNING "Header file not found: ${file}")
    else()
        message(STATUS "Found header file: ${file}")
    endif()
endforeach()

# Create executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Include directories - FIXED: Use absolute paths
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Display include paths
get_target_property(INCLUDE_DIRS ${PROJECT_NAME} INCLUDE_DIRECTORIES)
message(STATUS "Include directories for ${PROJECT_NAME}:")
foreach(dir ${INCLUDE_DIRS})
    message(STATUS "  ${dir}")
endforeach()

# Generate compile commands for IDEs
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ==================== Summary ====================
message(STATUS "")
message(STATUS "========================================")
message(STATUS "Configuration Summary")
message(STATUS "========================================")
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "Source Files: ${SOURCE_FILES}")
message(STATUS "========================================")