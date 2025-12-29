# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(StudentManagementSystem
    VERSION 1.0.0
    DESCRIPTION "Student Management System"
    LANGUAGES CXX
)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # GCC/Clang/MinGW - 指定UTF-8编码
    if(WIN32)
        # Windows下MinGW需要额外设置
        add_compile_options(-fexec-charset=UTF-8)
        add_compile_options(-finput-charset=UTF-8)
        message(STATUS "Enabled UTF-8 encoding for MinGW")
    else()
        # Linux/macOS默认UTF-8
        add_compile_options(-finput-charset=UTF-8)
        message(STATUS "Enabled UTF-8 encoding for GCC/Clang")
    endif()
endif()

# 输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

message(STATUS "Project directory: ${CMAKE_CURRENT_SOURCE_DIR}")

# ==================== Compiler Configuration ====================
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    message(STATUS "Compiler: MSVC")
    add_compile_options(/W4 /MP /utf-8)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(/O2)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(/Zi /Od)
    endif()

elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(STATUS "Compiler: GCC/MinGW")
    add_compile_options(-Wall -Wextra)
    
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(-O2)
    elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-g -O0)
    endif()
    
    if(WIN32)
        add_compile_options(-municode)
        add_link_options(-municode)
    endif()
endif()

# Windows specific definitions
if(WIN32)
    add_definitions(-D_WIN32_WINNT=0x0A00)
endif()


set(SOURCE_FILES
    main.cpp
    src/student.cpp
    src/io.cpp
)

set(HEADER_FILES
    include/student.hpp
    include/io.hpp
)


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


add_executable(${PROJECT_NAME} ${SOURCE_FILES})


target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message(STATUS "")
message(STATUS "========================================")
message(STATUS "Configuration Summary")
message(STATUS "========================================")
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Source Files: ${SOURCE_FILES}")
message(STATUS "Header Files: ${HEADER_FILES}")
message(STATUS "========================================")