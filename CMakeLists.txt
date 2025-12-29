cmake_minimum_required(VERSION 3.10)
project(StudentManagementSystem)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
    main.cpp
    src/student.cpp
    src/io.cpp
)

# 包含目录
target_include_directories(${PROJECT_NAME} PRIVATE include)

# 生成编译数据库
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# MinGW特定设置
if(MINGW)
    # 保留控制台子系统设置
    set_target_properties(${PROJECT_NAME} PROPERTIES
        LINK_FLAGS "-Wl,-subsystem,console"
    )
endif()