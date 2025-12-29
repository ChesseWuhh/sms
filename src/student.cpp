#include "student.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <sstream>


//Student 类实现

Student::Student() : gender('M'), age(18), math(0), cpp(0), english(0), 
                    linearAlgebra(0), political(0), totalScore(0), 
                    averageScore(0), rank(0) {}

Student::Student(const std::string& id, const std::string& name, 
                 char gender, int age) 
    : id(id), name(name), gender(gender), age(age),
      math(0), cpp(0), english(0), linearAlgebra(0), political(0),
      totalScore(0), averageScore(0), rank(0) {}

void Student::calculateScores() {
    totalScore = math + cpp + english + linearAlgebra + political;
    averageScore = totalScore / 5.0;
}

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

Student Student::fromString(const std::string& str) {
    Student student;
    std::istringstream iss(str);
    std::string token;
    std::vector<std::string> tokens;
    
    while (std::getline(iss, token, '|')) {
        tokens.push_back(token);
    }
    
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

void Student::display() const {
    std::cout << "\n========== 学生信息 ==========\n";
    std::cout << "学号: " << id << "\n";
    std::cout << "姓名: " << name << "\n";
    std::cout << "性别: " << (gender == 'M' ? "男" : "女") << "\n";
    std::cout << "年龄: " << age << "\n";
    std::cout << "院系: " << department << "\n";
    std::cout << "专业: " << major << "\n";
    std::cout << "班级: " << className << "\n";
    std::cout << "\n-------- 成绩信息 --------\n";
    std::cout << "高等数学: " << math << "\n";
    std::cout << "C++程序设计: " << cpp << "\n";
    std::cout << "大学英语: " << english << "\n";
    std::cout << "线性代数: " << linearAlgebra << "\n";
    std::cout << "思想政治: " << political << "\n";
    std::cout << "\n-------- 统计信息 --------\n";
    std::cout << "总分: " << totalScore << "\n";
    std::cout << "平均分: " << averageScore << "\n";
    std::cout << "排名: " << rank << "\n";
    std::cout << "============================\n";
}

//StudentManager 类实现

void StudentManager::updateRanks() {
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

bool StudentManager::addStudent(const Student& student) {
    // 检查学号是否重复
    for (const auto& s : students) {
        if (s.id == student.id) {
            std::cout << "错误: 学号 " << student.id << " 已存在！\n";
            return false;
        }
    }
    
    students.push_back(student);
    updateRanks();
    return true;
}

bool StudentManager::deleteStudent(const std::string& id) {
    auto it = std::remove_if(students.begin(), students.end(),
        [&](const Student& s) { return s.id == id; });
    
    if (it != students.end()) {
        students.erase(it, students.end());
        updateRanks();
        return true;
    }
    
    std::cout << "错误: 找不到学号为 " << id << " 的学生！\n";
    return false;
}

bool StudentManager::updateStudent(const std::string& id, const Student& newStudent) {
    for (auto& student : students) {
        if (student.id == id) {
            // 检查新学号是否与其他学生冲突
            if (id != newStudent.id) {
                for (const auto& s : students) {
                    if (s.id == newStudent.id) {
                        std::cout << "错误: 学号 " << newStudent.id << " 已存在！\n";
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
    
    std::cout << "错误: 找不到学号为 " << id << " 的学生！\n";
    return false;
}

Student* StudentManager::findStudent(const std::string& id) {
    for (auto& student : students) {
        if (student.id == id) {
            return &student;
        }
    }
    return nullptr;
}

std::vector<Student> StudentManager::getAllStudents() const {
    return students;
}

std::vector<Student> StudentManager::findStudentsByCondition(
    const std::string& field, const std::string& value) {
    
    std::vector<Student> result;
    
    for (const auto& student : students) {
        bool match = false;
        
        switch(field){
            case "id":
                match = (student.id.find(value) != std::string::npos);
                break;
            case "name":
                match = (student.name.find(value) != std::string::npos);
                break;
            case "department":
                match = (student.department.find(value) != std::string::npos);
                break;
            case "major":
                match = (student.major.find(value) != std::string::npos);
                break;
            case "class":
                match = (student.className.find(value) != std::string::npos);
                break;
        }
        
        if (match) {
            result.push_back(student);
        }
    }
    
    return result;
}

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

void StudentManager::showFailingStudents() const {
    std::cout << "\n========== 不及格学生名单 ==========\n";
    bool hasFailing = false;
    
    for (const auto& student : students) {
        if (student.averageScore < 60.0) {
            std::cout << "学号: " << student.id 
                     << ", 姓名: " << student.name
                     << ", 平均分: " << student.averageScore << "\n";
            hasFailing = true;
        }
    }
    
    if (!hasFailing) {
        std::cout << "所有学生都及格了！\n";
    }
    std::cout << "====================================\n";
}

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
        if (!ascending) {
            updateRanks();
        }
    }
}

size_t StudentManager::getCount() const {
    return students.size();
}

void StudentManager::clear() {
    students.clear();
}

