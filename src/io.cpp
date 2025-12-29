#include "io.hpp"
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

// ==================== InputHelper 类实现 ====================

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
        std::cout << "Input cannot be empty, please try again!\n";
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
        std::cout << "Please enter an integer between " << min << " and " << max << "!\n";
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
        std::cout << "Please enter a number between " << min << " and " << max << "!\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

char InputHelper::getGender(const std::string& prompt) {
    char gender;
    while (true) {
        std::cout << prompt << " (M-Male/F-Female): ";
        std::cin >> gender;
        gender = toupper(gender);
        
        if (gender == 'M' || gender == 'F') {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return gender;
        }
        std::cout << "Please enter M or F!\n";
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

// ==================== FileStorage 类实现 ====================

FileStorage::FileStorage() : dataDir("data"), dataFile("data/students.txt") {
    ensureDataDirectory();
}

void FileStorage::ensureDataDirectory() {
    if (!fs::exists(dataDir)) {
        fs::create_directory(dataDir);
    }
}

void FileStorage::setDataFile(const std::string& path) {
    dataFile = path;
    dataDir = fs::path(path).parent_path().string();
    ensureDataDirectory();
}

bool FileStorage::saveStudents(const std::vector<Student>& students) {
    std::ofstream file(dataFile);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << dataFile << " for writing!\n";
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
    std::cout << "Data saved to " << dataFile << " (" << students.size() << " records)\n";
    return true;
}

std::vector<Student> FileStorage::loadStudents() {
    std::vector<Student> students;
    
    std::ifstream file(dataFile);
    if (!file.is_open()) {
        std::cout << "Data file not found, will create a new one.\n";
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
            std::cerr << "Warning: Line " << lineCount << " has invalid format: " << e.what() << "\n";
        }
    }
    
    file.close();
    std::cout << "Loaded " << students.size() << " student records from " << dataFile << "\n";
    return students;
}

bool FileStorage::createBackup() {
    if (!fs::exists(dataFile)) {
        std::cout << "No data to backup\n";
        return false;
    }
    
    // 生成备份文件名（包含时间戳）
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
        std::cout << "Data backed up to " << backupFile << "\n";
        return true;
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Backup failed: " << e.what() << "\n";
        return false;
    }
}

bool FileStorage::exportToCSV(const std::vector<Student>& students, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot create file " << filename << "\n";
        return false;
    }
    
    // CSV头部
    file << "StudentID,Name,Gender,Age,Department,Major,Class,Math,C++,English,LinearAlgebra,Political,TotalScore,AverageScore,Rank\n";
    
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
    std::cout << "Data exported to " << filename << " (" << students.size() << " records)\n";
    return true;
}

std::vector<Student> FileStorage::importFromCSV(const std::string& filename) {
    std::vector<Student> students;
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << "\n";
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
            std::cerr << "Warning: Line " << lineCount << " import failed: " << e.what() << "\n";
        }
    }
    
    file.close();
    std::cout << "Imported " << students.size() << " student records from " << filename << "\n";
    return students;
}

// ==================== DisplayHelper 类实现 ====================

void DisplayHelper::clearScreen() {
#ifdef _WIN32
    system("cls");""
#else
    system("clear");
#endif
}

void DisplayHelper::displayStudentTable(const std::vector<Student>& students, bool showAll) {
    if (students.empty()) {
        std::cout << "\nNo student records found.\n";
        return;
    }
    
    // 限制显示数量
    size_t displayCount = showAll ? students.size() : std::min(students.size(), size_t(20));
    
    std::cout << "\n========== Student List ==========\n";
    std::cout << "Showing " << displayCount << " / " << students.size() << " records\n\n";
    
    // 表头
    std::cout << std::left 
              << std::setw(12) << "Student ID"
              << std::setw(10) << "Name"
              << std::setw(6) << "Gender"
              << std::setw(6) << "Age"
              << std::setw(15) << "Major"
              << std::setw(8) << "Average"
              << std::setw(6) << "Rank"
              << "\n";
    
    std::cout << std::string(65, '-') << "\n";
    
    // 表格内容
    for (size_t i = 0; i < displayCount; i++) {
        const auto& student = students[i];
        std::cout << std::left 
                  << std::setw(12) << (student.id.length() > 11 ? student.id.substr(0, 10) + "..." : student.id)
                  << std::setw(10) << (student.name.length() > 9 ? student.name.substr(0, 8) + "..." : student.name)
                  << std::setw(6) << (student.gender == 'M' ? "M" : "F")
                  << std::setw(6) << student.age
                  << std::setw(15) << (student.major.length() > 14 ? student.major.substr(0, 13) + "..." : student.major)
                  << std::setw(8) << std::fixed << std::setprecision(1) << student.averageScore
                  << std::setw(6) << student.rank
                  << "\n";
    }
    
    if (!showAll && students.size() > 20) {
        std::cout << "\n... " << (students.size() - 20) << " more records not shown\n";
        std::cout << "Enter 'all' to show all records\n";
    }
    
    std::cout << "===============================\n";
}

void DisplayHelper::displayStatistics(const StudentManager::Statistics& stats) {
    std::cout << "\n========== Statistics ==========\n";
    std::cout << "Total Students: " << stats.totalStudents << "\n";
    std::cout << "Passed: " << stats.passCount << "\n";
    std::cout << "Failed: " << stats.failCount << "\n";
    std::cout << "Pass Rate: " << std::fixed << std::setprecision(1) 
              << (stats.totalStudents > 0 ? (stats.passCount * 100.0 / stats.totalStudents) : 0)
              << "%\n\n";
    
    std::cout << "Average Scores:\n";
    std::cout << "  Advanced Math: " << std::fixed << std::setprecision(2) << stats.avgMath << "\n";
    std::cout << "  C++ Programming: " << stats.avgCpp << "\n";
    std::cout << "  English: " << stats.avgEnglish << "\n";
    std::cout << "  Linear Algebra: " << stats.avgLinearAlgebra << "\n";
    std::cout << "  Political: " << stats.avgPolitical << "\n";
    std::cout << "\nOverall Average: " << stats.overallAverage << "\n";
    std::cout << "===============================\n";
}

void DisplayHelper::displayMenu() {
    clearScreen();
    std::cout << "========================================\n";
    std::cout << "      Student Management System v1.0\n";
    std::cout << "========================================\n";
    std::cout << "1. Add Student\n";
    std::cout << "2. Delete Student\n";
    std::cout << "3. Update Student\n";
    std::cout << "4. Search Students\n";
    std::cout << "5. Show All Students\n";
    std::cout << "6. Show Statistics\n";
    std::cout << "7. Show Failing Students\n";
    std::cout << "8. Sort Students\n";
    std::cout << "9. Backup Data\n";
    std::cout << "10. Import/Export Data\n";
    std::cout << "11. Save Data\n";
    std::cout << "12. Reload Data\n";
    std::cout << "0. Exit\n";
    std::cout << "========================================\n";
}

void DisplayHelper::showWelcome() {
    clearScreen();
    std::cout << "========================================\n";
    std::cout << "      Welcome to Student Management System\n";
    std::cout << "========================================\n";
    std::cout << "Features:\n";
    std::cout << "  • Add/Delete/Update student information\n";
    std::cout << "  • Grade management and statistics\n";
    std::cout << "  • Data persistence\n";
    std::cout << "  • Supports 1000+ records\n";
    std::cout << "  • Clean CLI interface\n";
    std::cout << "========================================\n";
    std::cout << "Press Enter to continue...";
    std::cin.get();
}

void DisplayHelper::pause() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}