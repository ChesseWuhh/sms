#include "io.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <limits>
#include <filesystem>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <ctime>
#include <algorithm>

namespace fs = std::filesystem;

extern StudentManager studentManager;
extern FileStorage fileStorage;

//InputHelper 实现

std::string InputHelper::getString(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, input);
        
        // 移除首尾空格
        input.erase(0, input.find_first_not_of(" \t\n\r\f\v"));
        input.erase(input.find_last_not_of(" \t\n\r\f\v") + 1);
        
        if (!input.empty()) {
            return input;
        }
        std::cout << "输入不能为空，请重新输入！\n";
    }
}

int InputHelper::getInt(const std::string& prompt, int min, int max) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            if (value >= min && value <= max) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return value;
            }
        }
        std::cout << "请输入 " << min << " 到 " << max << " 之间的整数！\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

double InputHelper::getDouble(const std::string& prompt, double min, double max) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            if (value >= min && value <= max) {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                return value;
            }
        }
        std::cout << "请输入 " << min << " 到 " << max << " 之间的数值！\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

char InputHelper::getGender(const std::string& prompt) {
    char gender;
    while (true) {
        std::cout << prompt << " (M-男/F-女): ";
        std::cin >> gender;
        gender = toupper(gender);
        
        if (gender == 'M' || gender == 'F') {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return gender;
        }
        std::cout << "请输入 M 或 F！\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

bool InputHelper::confirm(const std::string& message) {
    std::cout << message << " (Y/N): ";
    char choice;
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return toupper(choice) == 'Y';
}

//FileStorage 实现

FileStorage::FileStorage() {
    ensureDataDirectory();
}

void FileStorage::ensureDataDirectory() {
    std::string dataDir = "data";
    if (!fs::exists(dataDir)) {
        fs::create_directory(dataDir);
    }
}

void FileStorage::setDataFile(const std::string& path) {
    dataFile = path;
    ensureDataDirectory();
}

bool FileStorage::saveStudents(const std::vector<Student>& students) {
    std::ofstream file(dataFile);
    if (!file.is_open()) {
        std::cerr << "错误: 无法打开文件 " << dataFile << " 进行写入！\n";
        return false;
    }
    
    // 写入数据头
    file << "# Student Management System Data File\n";
    file << "# Format: id|name|gender|age|department|major|class|math|cpp|english|linearAlgebra|political|totalScore|averageScore|rank\n";
    
    // 写入每个学生的数据
    for (const auto& student : students) {
        file << student.toString() << "\n";
    }
    
    file.close();
    std::cout << "数据已保存到 " << dataFile << " (" << students.size() << " 条记录)\n";
    return true;
}

std::vector<Student> FileStorage::loadStudents() {
    std::vector<Student> students;
    
    std::ifstream file(dataFile);
    if (!file.is_open()) {
        std::cout << "数据文件不存在，将创建新文件。\n";
        return students;
    }
    
    std::string line;
    int lineCount = 0;
    
    while (std::getline(file, line)) {
        lineCount++;
        
        // 跳过空行和注释行
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        try {
            Student student = Student::fromString(line);
            students.push_back(student);
        } catch (const std::exception& e) {
            std::cerr << "警告: 第 " << lineCount << " 行数据格式错误\n";
        }
    }
    
    file.close();
    std::cout << "从 " << dataFile << " 加载了 " << students.size() << " 条学生记录\n";
    return students;
}

bool FileStorage::createBackup() {
    if (!fs::exists(dataFile)) {
        std::cout << "没有数据需要备份\n";
        return false;
    }
    
    // 生成备份文件名
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&time);
    
    std::ostringstream oss;
    oss << "data/backup_students_"
        << (tm.tm_year + 1900) << "-"
        << std::setfill('0') << std::setw(2) << (tm.tm_mon + 1) << "-"
        << std::setw(2) << tm.tm_mday << "_"
        << std::setw(2) << tm.tm_hour << "-"
        << std::setw(2) << tm.tm_min << ".txt";
    
    std::string backupFile = oss.str();
    
    try {
        fs::copy_file(dataFile, backupFile, fs::copy_options::overwrite_existing);
        std::cout << "数据已备份到 " << backupFile << "\n";
        return true;
    } catch (const fs::filesystem_error& e) {
        std::cerr << "备份失败\n";
        return false;
    }
}

bool FileStorage::exportToCSV(const std::vector<Student>& students, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "错误: 无法创建文件 " << filename << "\n";
        return false;
    }
    
    // CSV头部
    file << "学号,姓名,性别,年龄,院系,专业,班级,高等数学,C++程序设计,大学英语,线性代数,思想政治,总分,平均分,排名\n";
    
    // 数据行
    for (const auto& student : students) {
        file << student.id << ","
             << student.name << ","
             << student.gender << ","
             << student.age << ","
             << student.department << ","
             << student.major << ","
             << student.className << ","
             << student.math << ","
             << student.cpp << ","
             << student.english << ","
             << student.linearAlgebra << ","
             << student.political << ","
             << student.totalScore << ","
             << student.averageScore << ","
             << student.rank << "\n";
    }
    
    file.close();
    std::cout << "数据已导出到 " << filename << " (" << students.size() << " 条记录)\n";
    return true;
}

std::vector<Student> FileStorage::importFromCSV(const std::string& filename) {
    std::vector<Student> students;
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "错误: 无法打开文件 " << filename << "\n";
        return students;
    }
    
    std::string line;
    int lineCount = 0;
    
    while (std::getline(file, line)) {
        lineCount++;
        
        // 跳过空行和标题行
        if (line.empty() || lineCount == 1) {
            continue;
        }
        
        try {
            // CSV解析
            std::istringstream iss(line);
            std::vector<std::string> tokens;
            std::string token;
            
            while (std::getline(iss, token, ',')) {
                tokens.push_back(token);
            }
            
            if (tokens.size() >= 15) {
                Student student;
                student.id = tokens[0];
                student.name = tokens[1];
                student.gender = tokens[2][0];
                student.age = std::stoi(tokens[3]);
                student.department = tokens[4];
                student.major = tokens[5];
                student.className = tokens[6];
                student.math = std::stod(tokens[7]);
                student.cpp = std::stod(tokens[8]);
                student.english = std::stod(tokens[9]);
                student.linearAlgebra = std::stod(tokens[10]);
                student.political = std::stod(tokens[11]);
                student.totalScore = std::stod(tokens[12]);
                student.averageScore = std::stod(tokens[13]);
                student.rank = std::stoi(tokens[14]);
                student.calculateScores();
                
                students.push_back(student);
            }
        } catch (const std::exception& e) {
            std::cerr << "警告: 第 " << lineCount << " 行导入失败\n";
        }
    }
    
    file.close();
    std::cout << "从 " << filename << " 导入了 " << students.size() << " 条学生记录\n";
    return students;
}

//DisplayHelper 实现

void DisplayHelper::clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void DisplayHelper::displayStudentTable(const std::vector<Student>& students, bool showAll) {
    if (students.empty()) {
        std::cout << "\n暂无学生记录。\n";
        return;
    }
    
    // 限制显示数量
    size_t displayCount = showAll ? students.size() : std::min(students.size(), size_t(20));
    
    std::cout << "\n========== 学生列表 ==========\n";
    std::cout << "显示 " << displayCount << " / " << students.size() << " 条记录\n\n";
    
    // 表头
    std::cout << std::left 
              << std::setw(12) << "学号"
              << std::setw(10) << "姓名"
              << std::setw(6) << "性别"
              << std::setw(6) << "年龄"
              << std::setw(15) << "专业"
              << std::setw(8) << "平均分"
              << std::setw(6) << "排名"
              << "\n";
    
    std::cout << std::string(65, '-') << "\n";
    
    // 表格内容
    for (size_t i = 0; i < displayCount; i++) {
        const auto& student = students[i];
        std::cout << std::left 
                  << std::setw(12) << (student.id.length() > 11 ? student.id.substr(0, 10) + "..." : student.id)
                  << std::setw(10) << (student.name.length() > 9 ? student.name.substr(0, 8) + "..." : student.name)
                  << std::setw(6) << (student.gender == 'M' ? "男" : "女")
                  << std::setw(6) << student.age
                  << std::setw(15) << (student.major.length() > 14 ? student.major.substr(0, 13) + "..." : student.major)
                  << std::setw(8) << std::fixed << std::setprecision(1) << student.averageScore
                  << std::setw(6) << student.rank
                  << "\n";
    }
    
    if (!showAll && students.size() > 20) {
        std::cout << "\n... 还有 " << (students.size() - 20) << " 条记录未显示\n";
    }
    
    std::cout << "===============================\n";
}

void DisplayHelper::displayStatistics(const StudentManager::Statistics& stats) {
    std::cout << "\n========== 统计信息 ==========\n";
    std::cout << "学生总数: " << stats.totalStudents << " 人\n";
    std::cout << "及格人数: " << stats.passCount << " 人\n";
    std::cout << "不及格人数: " << stats.failCount << " 人\n";
    std::cout << "及格率: " << std::fixed << std::setprecision(1) 
              << (stats.totalStudents > 0 ? (stats.passCount * 100.0 / stats.totalStudents) : 0)
              << "%\n\n";
    
    std::cout << "各科平均分:\n";
    std::cout << "  高等数学: " << std::fixed << std::setprecision(2) << stats.avgMath << " 分\n";
    std::cout << "  C++程序设计: " << stats.avgCpp << " 分\n";
    std::cout << "  大学英语: " << stats.avgEnglish << " 分\n";
    std::cout << "  线性代数: " << stats.avgLinearAlgebra << " 分\n";
    std::cout << "  思想政治: " << stats.avgPolitical << " 分\n";
    std::cout << "\n总体平均分: " << stats.overallAverage << " 分\n";
    std::cout << "===============================\n";
}

void DisplayHelper::displayMenu() {
    clearScreen();
    std::cout << "========================================\n";
    std::cout << "      学生信息管理系统 v1.0\n";
    std::cout << "========================================\n";
    std::cout << "1. 添加学生 ADD\n";
    std::cout << "2. 删除学生 DEL\n";
    std::cout << "3. 修改学生信息 UPDATE\n";
    std::cout << "4. 查询学生 SERCH\n";
    std::cout << "5. 显示所有学生 SHOW\n";
    std::cout << "6. 成绩统计 STAT\n";
    std::cout << "7. 显示不及格学生 FAIL\n";
    std::cout << "8. 排序学生 SORT\n";
    std::cout << "9. 保存数据\n";
    std::cout << "10. 重新加载数据\n";
    std::cout << "退出系统请输入EXIT\n";
    std::cout << "========================================\n";
}

void DisplayHelper::showWelcome() {
    clearScreen();
    std::cout << "========================================\n";
    std::cout << "      欢迎使用学生信息管理系统\n";
    std::cout << "========================================\n";
    std::cout << "功能特点:\n";
    std::cout << "  • 学生信息的增删改查\n";
    std::cout << "  • 成绩管理和统计分析\n";
    std::cout << "  • 数据持久化存储\n";
    std::cout << "  • 支持1000+条记录\n";
    std::cout << "  • 简洁的CLI界面\n";
    std::cout << "========================================\n";
}

void DisplayHelper::pause() {
    std::cout << "\n按回车键继续...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

// 添加学生
void addStudent() {
    DisplayHelper::clearScreen();
    std::cout << "=== 添加学生 ===\n\n";
    
    Student student;
    
    student.id = InputHelper::getString("学号: ");
    student.name = InputHelper::getString("姓名: ");
    student.gender = InputHelper::getGender("性别");
    student.age = InputHelper::getInt("年龄: ", 15, 30);
    student.department = InputHelper::getString("院系: ");
    student.major = InputHelper::getString("专业: ");
    student.className = InputHelper::getString("班级: ");
    
    std::cout << "\n请输入成绩 (0-100):\n";
    student.math = InputHelper::getDouble("高等数学: ", 0, 100);
    student.cpp = InputHelper::getDouble("C++程序设计: ", 0, 100);
    student.english = InputHelper::getDouble("大学英语: ", 0, 100);
    student.linearAlgebra = InputHelper::getDouble("线性代数: ", 0, 100);
    student.political = InputHelper::getDouble("思想政治: ", 0, 100);
    
    student.calculateScores();
    
    if (studentManager.addStudent(student)) {
        std::cout << "\n✅ 学生添加成功！\n";
        student.display();
    }
    
    DisplayHelper::pause();
}

// 删除学生
void deleteStudent() {
    DisplayHelper::clearScreen();
    std::cout << "=== 删除学生 ===\n\n";
    
    std::string id = InputHelper::getString("请输入要删除的学生学号: ");
    
    Student* student = studentManager.findStudent(id);
    if (student) {
        student->display();
        
        if (InputHelper::confirm("确定要删除这个学生吗？")) {
            if (studentManager.deleteStudent(id)) {
                std::cout << "\n✅ 学生删除成功！\n";
            }
        }
    } else {
        std::cout << "\n❌ 找不到学号为 " << id << " 的学生！\n";
    }
    
    DisplayHelper::pause();
}

// 修改学生信息
void updateStudent() {
    DisplayHelper::clearScreen();
    std::cout << "=== 修改学生信息 ===\n\n";
    
    std::string oldId = InputHelper::getString("请输入要修改的学生学号: ");
    
    Student* oldStudent = studentManager.findStudent(oldId);
    if (!oldStudent) {
        std::cout << "\n❌ 找不到学号为 " << oldId << " 的学生！\n";
        DisplayHelper::pause();
        return;
    }
    
    std::cout << "\n当前学生信息:\n";
    oldStudent->display();
    
    std::cout << "\n请输入新的信息（直接回车保持原值）:\n";
    
    // 创建新学生对象，复制原数据
    Student newStudent = *oldStudent;
    
    // 输入新信息
    std::string input;
    
    // 学号
    std::cout << "新学号 [" << oldStudent->id << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) newStudent.id = input;
    
    // 姓名
    std::cout << "新姓名 [" << oldStudent->name << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) newStudent.name = input;
    
    // 性别
    std::cout << "新性别 [" << oldStudent->gender << "] (M/F): ";
    std::getline(std::cin, input);
    if (!input.empty()) newStudent.gender = toupper(input[0]);
    
    // 年龄
    std::cout << "新年龄 [" << oldStudent->age << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) newStudent.age = std::stoi(input);
    
    // 院系
    std::cout << "新院系 [" << oldStudent->department << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) newStudent.department = input;
    
    // 专业
    std::cout << "新专业 [" << oldStudent->major << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) newStudent.major = input;
    
    // 班级
    std::cout << "新班级 [" << oldStudent->className << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) newStudent.className = input;
    
    // 成绩
    std::cout << "\n请输入新的成绩（直接回车保持原值）:\n";
    
    std::cout << "高等数学 [" << oldStudent->math << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) newStudent.math = std::stod(input);
    
    std::cout << "C++程序设计 [" << oldStudent->cpp << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) newStudent.cpp = std::stod(input);
    
    std::cout << "大学英语 [" << oldStudent->english << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) newStudent.english = std::stod(input);
    
    std::cout << "线性代数 [" << oldStudent->linearAlgebra << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) newStudent.linearAlgebra = std::stod(input);
    
    std::cout << "思想政治 [" << oldStudent->political << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) newStudent.political = std::stod(input);
    
    // 重新计算成绩
    newStudent.calculateScores();
    
    // 更新学生信息
    if (studentManager.updateStudent(oldId, newStudent)) {
        std::cout << "\n✅ 学生信息修改成功！\n";
        newStudent.display();
    }
    
    DisplayHelper::pause();
}

// 查询学生
void searchStudents() {
    DisplayHelper::clearScreen();
    std::cout << "=== 查询学生 ===\n\n";
    
    std::cout << "选择查询方式:\n";
    std::cout << "1. 按学号查询\n";
    std::cout << "2. 按姓名查询\n";
    std::cout << "3. 按院系查询\n";
    std::cout << "4. 按专业查询\n";
    std::cout << "5. 按班级查询\n";
    
    int choice = InputHelper::getInt("请选择: ", 1, 5);
    
    std::string field, value;
    switch (choice) {
        case 1: field = "id"; break;
        case 2: field = "name"; break;
        case 3: field = "department"; break;
        case 4: field = "major"; break;
        case 5: field = "class"; break;
    }
    
    value = InputHelper::getString("请输入查询关键字: ");
    
    auto results = studentManager.findStudentsByCondition(field, value);
    
    if (results.empty()) {
        std::cout << "\n未找到符合条件的学生。\n";
    } else {
        std::cout << "\n找到 " << results.size() << " 名学生:\n";
        DisplayHelper::displayStudentTable(results, true);
        
        // 显示详细信息选项
        if (InputHelper::confirm("\n是否查看某个学生的详细信息？")) {
            std::string id = InputHelper::getString("请输入学生学号: ");
            Student* student = studentManager.findStudent(id);
            if (student) {
                student->display();
            } else {
                std::cout << "未找到该学生！\n";
            }
        }
    }
    
    DisplayHelper::pause();
}

// 显示所有学生
void showAllStudents() {
    DisplayHelper::clearScreen();
    std::cout << "=== 所有学生列表 ===\n\n";
    
    auto students = studentManager.getAllStudents();
    DisplayHelper::displayStudentTable(students);
    
    // 查看详细信息选项
    if (!students.empty() && InputHelper::confirm("\n是否查看某个学生的详细信息？")) {
        std::string id = InputHelper::getString("请输入学生学号: ");
        Student* student = studentManager.findStudent(id);
        if (student) {
            student->display();
        } else {
            std::cout << "未找到该学生！\n";
        }
    }
    
    DisplayHelper::pause();
}

// 显示统计信息
void showStatistics() {
    DisplayHelper::clearScreen();
    std::cout << "=== 统计信息 ===\n\n";
    
    auto stats = studentManager.getStatistics();
    DisplayHelper::displayStatistics(stats);
    
    DisplayHelper::pause();
}

// 排序学生
void sortStudents() {
    DisplayHelper::clearScreen();
    std::cout << "=== 排序学生 ===\n\n";
    
    std::cout << "选择排序方式:\n";
    std::cout << "1. 按学号升序\n";
    std::cout << "2. 按学号降序\n";
    std::cout << "3. 按姓名升序\n";
    std::cout << "4. 按姓名降序\n";
    std::cout << "5. 按平均分升序\n";
    std::cout << "6. 按平均分降序\n";
    
    int choice = InputHelper::getInt("请选择: ", 1, 6);
    
    std::string by;
    bool ascending;
    
    switch (choice) {
        case 1: by = "id"; ascending = true; break;
        case 2: by = "id"; ascending = false; break;
        case 3: by = "name"; ascending = true; break;
        case 4: by = "name"; ascending = false; break;
        case 5: by = "score"; ascending = true; break;
        case 6: by = "score"; ascending = false; break;
    }
    
    studentManager.sortStudents(by, ascending);
    std::cout << "\n✅ 学生排序完成！\n";
    
    // 显示排序后的结果
    if (InputHelper::confirm("是否显示排序后的结果？")) {
        auto students = studentManager.getAllStudents();
        DisplayHelper::displayStudentTable(students);
    }
    
    DisplayHelper::pause();
}

// 备份数据
void backupData() {
    DisplayHelper::clearScreen();
    std::cout << "=== 数据备份 ===\n\n";
    
    if (fileStorage.createBackup()) {
        std::cout << "\n✅ 数据备份成功！\n";
    }
    
    DisplayHelper::pause();
}

// 数据导入导出
void importExportData() {
    DisplayHelper::clearScreen();
    std::cout << "=== 数据导入/导出 ===\n\n";
    
    std::cout << "1. 导出数据到CSV\n";
    std::cout << "2. 从CSV导入数据\n";
    std::cout << "3. 返回主菜单\n";
    
    int choice = InputHelper::getInt("请选择: ", 1, 3);
    
    if (choice == 1) {
        // 导出到CSV
        std::string filename = InputHelper::getString("请输入CSV文件名（如: students.csv）: ");
        auto students = studentManager.getAllStudents();
        
        if (fileStorage.exportToCSV(students, filename)) {
            std::cout << "\n✅ 数据导出成功！\n";
        }
    } else if (choice == 2) {
        // 从CSV导入
        std::string filename = InputHelper::getString("请输入CSV文件名: ");
        
        if (InputHelper::confirm("导入会覆盖当前数据，是否继续？")) {
            auto importedStudents = fileStorage.importFromCSV(filename);
            
            if (!importedStudents.empty()) {
                studentManager.setStudents(importedStudents);
                std::cout << "\n✅ 数据导入成功！共导入 " << importedStudents.size() << " 条记录\n";
            }
        }
    }
    
    DisplayHelper::pause();
}

// 保存数据
void saveData() {
    DisplayHelper::clearScreen();
    std::cout << "=== 保存数据 ===\n\n";
    
    auto students = studentManager.getAllStudents();
    if (fileStorage.saveStudents(students)) {
        std::cout << "\n✅ 数据保存成功！\n";
    }
    
    DisplayHelper::pause();
}

// 重新加载数据
void reloadData() {
    DisplayHelper::clearScreen();
    std::cout << "=== 重新加载数据 ===\n\n";
    
    if (InputHelper::confirm("重新加载会丢失未保存的修改，是否继续？")) {
        auto loadedStudents = fileStorage.loadStudents();
        studentManager.setStudents(loadedStudents);
        std::cout << "\n✅ 数据重新加载成功！当前有 " << studentManager.getCount() << " 名学生\n";
    }
    
    DisplayHelper::pause();
}
