@echo off
echo === 学生管理系统构建脚本 ===

REM 清理并创建构建目录
if exist build rmdir /s /q build
mkdir build
cd build

REM 配置项目
echo 配置项目...
cmake .. -G "Visual Studio 16 2019"

REM 构建项目
echo 构建项目...
cmake --build . --config Release

echo.
echo 构建成功！
echo 可执行文件: %CD%\bin\Release\sms.exe
echo.
echo 运行命令:
echo   bin\Release\sms.exe
echo.
pause