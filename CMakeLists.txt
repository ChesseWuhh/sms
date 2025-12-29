cmake_minimum_required(VERSION 3.10)
project(sms)

# 编码
if(MSVC)
    add_compile_options(/utf-8)
else()
    add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 编译器选项
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra)
endif()

# 生成编译数据库
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 添加可执行文件
add_executable(${PROJECT_NAME}
    main.cpp
    src/student.cpp
    src/io.cpp
)

# 设置包含路径
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 显示配置信息
message(STATUS "项目名称: ${PROJECT_NAME}")
message(STATUS "头文件目录: ${CMAKE_CURRENT_SOURCE_DIR}/include")
message(STATUS "源文件目录: ${CMAKE_CURRENT_SOURCE_DIR}/src")
message(STATUS "输出目录: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")