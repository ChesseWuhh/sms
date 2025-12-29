#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <functional>

// 学生结构体
struct Student {
    std::string id;         // 学号
    std::string name;       // 姓名
    char gender;           // 性别: M/F
    int age;               // 年龄
    std::string department; // 院系
    std::string major;     // 专业
    std::string className; // 班级
    
    // 成绩
    double math;           // 高等数学
    double cpp;           // C++程序设计
    double english;       // 大学英语
    double linearAlgebra; // 线性代数
    double political;     // 思想政治
    
    // 计算字段
    double totalScore;    // 总分
    double averageScore;  // 平均分
    int rank;            // 排名
    
    // 构造函数
    Student();
    Student(const std::string& id, const std::string& name, 
            char gender = 'M', int age = 18);
    
    // 方法声明
    void calculateScores();
    std::string toString() const;
    static Student fromString(const std::string& str);
    void display() const;
};

// 学生管理系统类声明
class StudentManager {
private:
    std::vector<Student> students;
    void updateRanks();
    
public:
    // 统计结构体
    struct Statistics {
        int totalStudents;
        double avgMath;
        double avgCpp;
        double avgEnglish;
        double avgLinearAlgebra;
        double avgPolitical;
        double overallAverage;
        int passCount;
        int failCount;
    };
    
    // 更新数据操作
    bool addStudent(const Student& student);
    bool deleteStudent(const std::string& id);
    bool updateStudent(const std::string& id, const Student& newStudent);
    Student* findStudent(const std::string& id);
    std::vector<Student> getAllStudents() const;
    
    // 查询和统计
    std::vector<Student> findStudentsByCondition(
        const std::string& field, const std::string& value);
    Statistics getStatistics() const;
    void showFailingStudents() const;
    
    // 排序和工具
    void sortStudents(const std::string& by, bool ascending = true);
    size_t getCount() const;
    void clear();
};

#endif