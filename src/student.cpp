#include "student.hpp"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <cctype>

// ==================== Student 类实现 ====================

// 默认构造函数
Student::Student() : gender('M'), age(18), math(0), cpp(0), english(0), 
                     linearAlgebra(0), political(0), totalScore(0), 
                     averageScore(0), rank(0) {}

// 带参数构造函数
Student::Student(const std::string& id, const std::string& name, 
                 char gender, int age) 
    : id(id), name(name), gender(toupper(gender)), age(age),
      math(0), cpp(0), english(0), linearAlgebra(0), political(0),
      totalScore(0), averageScore(0), rank(0) {}

// 计算总分和平均分
void Student::calculateScores() {
    totalScore = math + cpp + english + linearAlgebra + political;
    averageScore = totalScore / 5.0;
}

// 转换为字符串（用于文件存储）
std::string Student::toString() const {
    std::ostringstream oss;
    oss << id << "|" << name << "|" << gender << "|" << age << "|"
        << department << "|" << major << "|" << className << "|"
        << std::fixed << std::setprecision(2)
        << math << "|" << cpp << "|" << english << "|"
        << linearAlgebra << "|" << political << "|"
        << totalScore << "|" << averageScore << "|" << rank;
    return oss.str();
}

// 从字符串解析
Student Student::fromString(const std::string& str) {
    Student student;
    std::istringstream iss(str);
    std::string token;
    std::vector<std::string> tokens;
    
    // 分割字符串
    while (std::getline(iss, token, '|')) {
        tokens.push_back(token);
    }
    
    // 解析数据（至少需要15个字段）
    if (tokens.size() >= 15) {
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
    }
    
    return student;
}

// 显示学生信息
void Student::display() const {
    std::cout << "\n========== Student Information ==========\n";
    std::cout << "Student ID: " << id << "\n";
    std::cout << "Name: " << name << "\n";
    std::cout << "Gender: " << (gender == 'M' ? "Male" : "Female") << "\n";
    std::cout << "Age: " << age << "\n";
    std::cout << "Department: " << department << "\n";
    std::cout << "Major: " << major << "\n";
    std::cout << "Class: " << className << "\n";
    std::cout << "\n-------- Course Scores --------\n";
    std::cout << "Advanced Math: " << math << "\n";
    std::cout << "C++ Programming: " << cpp << "\n";
    std::cout << "English: " << english << "\n";
    std::cout << "Linear Algebra: " << linearAlgebra << "\n";
    std::cout << "Political: " << political << "\n";
    std::cout << "\n-------- Statistics --------\n";
    std::cout << "Total Score: " << totalScore << "\n";
    std::cout << "Average Score: " << averageScore << "\n";
    std::cout << "Rank: " << rank << "\n";
    std::cout << "=======================================\n";
}

// ==================== StudentManager 类实现 ====================

// 更新所有学生的排名
void StudentManager::updateRanks() {
    if (students.empty()) return;
    
    // 按平均分降序排序
    std::sort(students.begin(), students.end(),
        [](const Student& a, const Student& b) {
            return a.averageScore > b.averageScore;
        });
    
    // 分配排名（处理并列情况）
    int currentRank = 1;
    for (size_t i = 0; i < students.size(); i++) {
        if (i > 0 && students[i].averageScore < students[i-1].averageScore) {
            currentRank = i + 1;
        }
        students[i].rank = currentRank;
    }
}

// 添加学生
bool StudentManager::addStudent(const Student& student) {
    // 检查学号是否重复
    for (const auto& s : students) {
        if (s.id == student.id) {
            std::cout << "Error: Student ID " << student.id << " already exists!\n";
            return false;
        }
    }
    
    students.push_back(student);
    updateRanks();
    return true;
}

// 删除学生
bool StudentManager::deleteStudent(const std::string& id) {
    auto it = std::remove_if(students.begin(), students.end(),
        [&](const Student& s) { return s.id == id; });
    
    if (it != students.end()) {
        students.erase(it, students.end());
        updateRanks();
        return true;
    }
    
    std::cout << "Error: Student with ID " << id << " not found!\n";
    return false;
}

// 修改学生信息
bool StudentManager::updateStudent(const std::string& id, const Student& newStudent) {
    for (auto& student : students) {
        if (student.id == id) {
            // 检查新学号是否与其他学生冲突
            if (id != newStudent.id) {
                for (const auto& s : students) {
                    if (s.id == newStudent.id) {
                        std::cout << "Error: Student ID " << newStudent.id << " already exists!\n";
                        return false;
                    }
                }
            }
            
            student = newStudent;
            student.calculateScores();
            updateRanks();
            return true;
        }
    }
    
    std::cout << "Error: Student with ID " << id << " not found!\n";
    return false;
}

// 查找学生（按学号）
Student* StudentManager::findStudent(const std::string& id) {
    for (auto& student : students) {
        if (student.id == id) {
            return &student;
        }
    }
    return nullptr;
}

// 获取所有学生
std::vector<Student> StudentManager::getAllStudents() const {
    return students;
}

// 按条件查询学生
std::vector<Student> StudentManager::findStudentsByCondition(
    const std::string& field, const std::string& value) {
    
    std::vector<Student> result;
    
    for (const auto& student : students) {
        bool match = false;
        
        if (field == "id") {
            match = (student.id.find(value) != std::string::npos);
        } else if (field == "name") {
            match = (student.name.find(value) != std::string::npos);
        } else if (field == "department") {
            match = (student.department.find(value) != std::string::npos);
        } else if (field == "major") {
            match = (student.major.find(value) != std::string::npos);
        } else if (field == "class") {
            match = (student.className.find(value) != std::string::npos);
        }
        
        if (match) {
            result.push_back(student);
        }
    }
    
    return result;
}

// 获取统计信息
StudentManager::Statistics StudentManager::getStatistics() const {
    Statistics stats = {0};
    stats.totalStudents = students.size();
    
    if (students.empty()) {
        return stats;
    }
    
    double totalMath = 0, totalCpp = 0, totalEnglish = 0;
    double totalLinearAlgebra = 0, totalPolitical = 0;
    double totalOverall = 0;
    
    for (const auto& student : students) {
        totalMath += student.math;
        totalCpp += student.cpp;
        totalEnglish += student.english;
        totalLinearAlgebra += student.linearAlgebra;
        totalPolitical += student.political;
        totalOverall += student.averageScore;
        
        if (student.averageScore >= 60.0) {
            stats.passCount++;
        } else {
            stats.failCount++;
        }
    }
    
    stats.avgMath = totalMath / students.size();
    stats.avgCpp = totalCpp / students.size();
    stats.avgEnglish = totalEnglish / students.size();
    stats.avgLinearAlgebra = totalLinearAlgebra / students.size();
    stats.avgPolitical = totalPolitical / students.size();
    stats.overallAverage = totalOverall / students.size();
    
    return stats;
}

// 显示不及格学生
void StudentManager::showFailingStudents() const {
    std::cout << "\n========== Failing Students ==========\n";
    bool hasFailing = false;
    
    for (const auto& student : students) {
        if (student.averageScore < 60.0) {
            std::cout << "ID: " << student.id 
                     << ", Name: " << student.name
                     << ", Average: " << student.averageScore << "\n";
            hasFailing = true;
        }
    }
    
    if (!hasFailing) {
        std::cout << "All students have passed!\n";
    }
    std::cout << "====================================\n";
}

// 按条件排序
void StudentManager::sortStudents(const std::string& by, bool ascending) {
    if (by == "id") {
        std::sort(students.begin(), students.end(),
            [ascending](const Student& a, const Student& b) {
                return ascending ? (a.id < b.id) : (a.id > b.id);
            });
    } else if (by == "name") {
        std::sort(students.begin(), students.end(),
            [ascending](const Student& a, const Student& b) {
                return ascending ? (a.name < b.name) : (a.name > b.name);
            });
    } else if (by == "score") {
        std::sort(students.begin(), students.end(),
            [ascending](const Student& a, const Student& b) {
                return ascending ? (a.averageScore < b.averageScore) 
                                 : (a.averageScore > b.averageScore);
            });
    }
    
    // 如果按分数排序，需要重新计算排名
    if (by == "score") {
        updateRanks();
    }
}

// 获取学生数量
size_t StudentManager::getCount() const {
    return students.size();
}

// 清空所有数据
void StudentManager::clear() {
    students.clear();
}

// 设置学生列表
void StudentManager::setStudents(const std::vector<Student>& newStudents) {
    students = newStudents;
    updateRanks();
}