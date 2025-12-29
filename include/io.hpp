#ifndef IO_HPP
#define IO_HPP

#include <string>
#include <vector>
#include "student.hpp"

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
    std::string dataDir;      // 确保这个私有成员存在
    std::string dataFile;
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

#endif // IO_HPP