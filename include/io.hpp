#ifndef IO_H
#define IO_H

#include <string>
#include <vector>

#include "student.hpp"

void addStudent();
void deleteStudent();
void updateStudent();
void searchStudents();
void showAllStudents();
void showStatistics();
void sortStudents();
void backupData();
void importExportData();
void saveData();
void reloadData();

// 输入辅助类
class InputHelper {
public:
    static std::string getString(const std::string& prompt);
    static int getInt(const std::string& prompt, int min, int max);
    static double getDouble(const std::string& prompt, double min, double max);
    static char getGender(const std::string& prompt);
    static bool confirm(const std::string& message);
};

// 文件存储类
class FileStorage {
private:
    std::string dataFile = "data/students.txt";
    void ensureDataDirectory();
    
public:
    FileStorage();
    void setDataFile(const std::string& path);
    bool saveStudents(const std::vector<Student>& students);
    std::vector<Student> loadStudents();
    bool createBackup();
    bool exportToCSV(const std::vector<Student>& students, const std::string& filename);
    std::vector<Student> importFromCSV(const std::string& filename);
};

// 显示辅助类
class DisplayHelper {
public:
    static void clearScreen();
    static void displayStudentTable(const std::vector<Student>& students, bool showAll = false);
    static void displayStatistics(const StudentManager::Statistics& stats);
    static void displayMenu();
    static void showWelcome();
    static void pause();
};

#endif