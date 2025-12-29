#ifndef STUDENT_HPP
#define STUDENT_HPP

#include <string>
#include <vector>

// 学生结构体定义
struct Student {
    std::string id;
    std::string name;
    char gender;
    int age;
    std::string department;
    std::string major;
    std::string className;
    
    // 成绩
    double math;
    double cpp;
    double english;
    double linearAlgebra;
    double political;
    
    // 计算字段
    double totalScore;
    double averageScore;
    int rank;
    
    // 构造函数
    Student();
    Student(const std::string& id, const std::string& name, char gender = 'M', int age = 18);
    
    // 成员函数
    void calculateScores();
    std::string toString() const;
    static Student fromString(const std::string& str);
    void display() const;
};

// 学生管理系统类
class StudentManager {
private:
    std::vector<Student> students;
    void updateRanks();
    
public:
    // 学生管理操作
    bool addStudent(const Student& student);
    bool deleteStudent(const std::string& id);
    bool updateStudent(const std::string& id, const Student& newStudent);
    Student* findStudent(const std::string& id);
    std::vector<Student> getAllStudents() const;
    std::vector<Student> findStudentsByCondition(const std::string& field, const std::string& value);
    
    // 统计功能
    struct Statistics {
        int totalStudents = 0;
        double avgMath = 0.0;
        double avgCpp = 0.0;
        double avgEnglish = 0.0;
        double avgLinearAlgebra = 0.0;
        double avgPolitical = 0.0;
        double overallAverage = 0.0;
        int passCount = 0;
        int failCount = 0;
    };
    
    Statistics getStatistics() const;
    void showFailingStudents() const;
    
    // 排序功能
    void sortStudents(const std::string& by, bool ascending = true);
    
    // 工具函数
    size_t getCount() const;
    void clear();
    void setStudents(const std::vector<Student>& newStudents);  // 确保这个声明存在
};

#endif // STUDENT_HPP