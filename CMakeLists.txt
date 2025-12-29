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
    message(STATUS "Build type not specified, defaulting to Release")
endif()

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Display generator info
message(STATUS "CMake Generator: ${CMAKE_GENERATOR}")

# ==================== Compiler Configuration ====================
# Display compiler information
message(STATUS "Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Compiler Version: ${CMAKE_CXX_COMPILER_VERSION}")

# MSVC specific settings
if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    message(STATUS "Configuring for MSVC compiler")
    add_compile_options(/W4 /MP)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    
    # Enable UTF-8 support
    add_compile_options(/utf-8)
    
    # Optimization settings
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(/O2)
        message(STATUS "Release build: Optimization enabled")
    elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(/Zi /Od /RTC1)
        message(STATUS "Debug build: Debug symbols enabled")
    endif()

# GCC/MinGW specific settings
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(STATUS "Configuring for GCC/MinGW compiler")
    add_compile_options(-Wall -Wextra)
    
    # Optimization settings
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(-O2)
        message(STATUS "Release build: Optimization enabled")
    elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-g -O0)
        message(STATUS "Debug build: Debug symbols enabled")
    endif()
    
    # MinGW specific Unicode support
    if(WIN32)
        add_compile_options(-municode)
        add_link_options(-municode)
        message(STATUS "Windows build: Unicode support enabled")
    endif()

# Clang specific settings
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message(STATUS "Configuring for Clang compiler")
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
    message(STATUS "Windows build: Targeting Windows 10+ API")
endif()

# ==================== Project Configuration ====================
# Find source files
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "include/*.h")

# Display file information
list(LENGTH SOURCE_FILES NUM_SOURCE_FILES)
list(LENGTH HEADER_FILES NUM_HEADER_FILES)
message(STATUS "Found ${NUM_SOURCE_FILES} source files")
message(STATUS "Found ${NUM_HEADER_FILES} header files")

# Display source files (optional, for debugging)
if(NUM_SOURCE_FILES LESS 10)  # Only show if not too many
    message(STATUS "Source files:")
    foreach(src ${SOURCE_FILES})
        get_filename_component(FILENAME ${src} NAME)
        message(STATUS "  - ${FILENAME}")
    endforeach()
endif()

# Create executable
add_executable(${PROJECT_NAME}
    main.cpp
    ${SOURCE_FILES}
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Generate compile commands for IDEs
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
message(STATUS "Compile commands database: Enabled")

# ==================== Ninja Specific Optimizations ====================
if(CMAKE_GENERATOR MATCHES "Ninja")
    message(STATUS "Ninja generator detected - applying optimizations")
    
    # Enable Ninja pools for resource management
    set(CMAKE_JOB_POOLS compile=4;link=2)
    set(CMAKE_JOB_POOL_COMPILE compile)
    set(CMAKE_JOB_POOL_LINK link)
    
    # Force response files for long command lines
    set(CMAKE_NINJA_FORCE_RESPONSE_FILE ON)
    
    # Optimize dependencies
    set(CMAKE_DEPENDS_IN_PROJECT_ONLY ON)
endif()

# ==================== Installation Configuration ====================
# Install executable
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    BUNDLE DESTINATION bin
)

# Install headers (optional)
install(DIRECTORY include/ DESTINATION include)

# ==================== Summary ====================
message(STATUS "")
message(STATUS "========================================")
message(STATUS "Configuration Summary")
message(STATUS "========================================")
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Generator: ${CMAKE_GENERATOR}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "Source Files: ${NUM_SOURCE_FILES}")
message(STATUS "Header Files: ${NUM_HEADER_FILES}")
message(STATUS "Output Directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "========================================")
message(STATUS "")