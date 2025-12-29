@echo off
echo === 学生信息管理系统构建脚本 ===
echo 使用 Ninja 构建系统

REM 检查依赖
where cmake >nul 2>nul
if %errorlevel% neq 0 (
    echo 错误: 找不到 cmake
    echo 请从 https://cmake.org/download/ 下载安装
    pause
    exit /b 1
)

where ninja >nul 2>nul
if %errorlevel% neq 0 (
    echo 错误: 找不到 ninja
    echo 请从 https://github.com/ninja-build/ninja/releases 下载
    echo 下载后放入 PATH 环境变量中
    pause
    exit /b 1
)

REM 创建构建目录
if not exist "build" mkdir build
cd build

REM 配置项目
echo 配置项目...
cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release

REM 构建项目
echo 构建项目...
ninja

echo.
echo ✅ 构建成功！
echo 可执行文件: %CD%\bin\StudentManagementSystem.exe
echo.
echo 运行命令:
echo   bin\StudentManagementSystem.exe
echo.
pause