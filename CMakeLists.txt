# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
project(StudentManagementSystem
    VERSION 1.0.0
    DESCRIPTION "高校学生信息管理系统"
    LANGUAGES CXX
)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置默认构建类型
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 编译器选项
if(MSVC)
    add_compile_options(/W4 /WX /MP)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
    add_compile_options(-O2)
endif()

# 生成编译数据库（用于IDE）
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 添加可执行文件
add_executable(${PROJECT_NAME}
    main.cpp
    src/student.cpp
    src/io.cpp
)

# 包含目录
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

message(STATUS "项目 ${PROJECT_NAME} 配置完成！")
message(STATUS "构建类型: ${CMAKE_BUILD_TYPE}")
message(STATUS "编译器: ${CMAKE_CXX_COMPILER_ID}")