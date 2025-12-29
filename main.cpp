#include "student.hpp"
#include "io.hpp"
#include <iostream>
#include <string>

// 全局变量定义
StudentManager studentManager;
FileStorage fileStorage;

// 函数声明（确保这些函数都有实现）
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

// 安全的获取菜单选择
int getMenuChoice() {
    std::string input;
    std::getline(std::cin, input);
    
    if (input.empty()) {
        return -1;
    }
    
    try {
        return std::stoi(input);
    } catch (const std::exception& e) {
        return -1;
    }
}

// 添加学生
void addStudent() {
    DisplayHelper::clearScreen();
    std::cout << "=== Add Student ===\n\n";
    
    Student student;
    
    student.id = InputHelper::getString("Student ID: ");
    student.name = InputHelper::getString("Name: ");
    student.gender = InputHelper::getGender("Gender");
    student.age = InputHelper::getInt("Age: ", 15, 30);
    student.department = InputHelper::getString("Department: ");
    student.major = InputHelper::getString("Major: ");
    student.className = InputHelper::getString("Class: ");
    
    std::cout << "\nEnter scores (0-100):\n";
    student.math = InputHelper::getDouble("Advanced Math: ", 0, 100);
    student.cpp = InputHelper::getDouble("C++ Programming: ", 0, 100);
    student.english = InputHelper::getDouble("English: ", 0, 100);
    student.linearAlgebra = InputHelper::getDouble("Linear Algebra: ", 0, 100);
    student.political = InputHelper::getDouble("Political: ", 0, 100);
    
    // 计算总分和平均分
    student.calculateScores();
    
    // 添加到管理器
    if (studentManager.addStudent(student)) {
        std::cout << "\n✅ Student added successfully!\n";
        student.display();
    }
    
    DisplayHelper::pause();
}

// 删除学生
void deleteStudent() {
    DisplayHelper::clearScreen();
    std::cout << "=== Delete Student ===\n\n";
    
    std::string id = InputHelper::getString("Enter student ID to delete: ");
    
    // 先查找学生
    Student* student = studentManager.findStudent(id);
    if (student) {
        student->display();
        
        if (InputHelper::confirm("Are you sure you want to delete this student?")) {
            if (studentManager.deleteStudent(id)) {
                std::cout << "\n Student deleted successfully!\n";
            }
        }
    } else {
        std::cout << "\n Student with ID " << id << " not found!\n";
    }
    
    DisplayHelper::pause();
}

// 修改学生信息
void updateStudent() {
    DisplayHelper::clearScreen();
    std::cout << "=== Update Student ===\n\n";
    
    std::string oldId = InputHelper::getString("Enter student ID to update: ");
    
    Student* oldStudent = studentManager.findStudent(oldId);
    if (!oldStudent) {
        std::cout << "\n Student with ID " << oldId << " not found!\n";
        DisplayHelper::pause();
        return;
    }
    
    std::cout << "\nCurrent student information:\n";
    oldStudent->display();
    
    std::cout << "\nEnter new information (press Enter to keep current value):\n";
    
    // 创建新学生对象，复制原数据
    Student newStudent = *oldStudent;
    
    // 输入新信息
    std::string input;
    
    // ID
    std::cout << "New ID [" << oldStudent->id << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) newStudent.id = input;
    
    // Name
    std::cout << "New Name [" << oldStudent->name << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) newStudent.name = input;
    
    // Gender
    std::cout << "New Gender [" << oldStudent->gender << "] (M/F): ";
    std::getline(std::cin, input);
    if (!input.empty()) newStudent.gender = toupper(input[0]);
    
    // Age
    std::cout << "New Age [" << oldStudent->age << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) newStudent.age = std::stoi(input);
    
    // Department
    std::cout << "New Department [" << oldStudent->department << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) newStudent.department = input;
    
    // Major
    std::cout << "New Major [" << oldStudent->major << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) newStudent.major = input;
    
    // Class
    std::cout << "New Class [" << oldStudent->className << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) newStudent.className = input;
    
    // 成绩
    std::cout << "\nEnter new scores (press Enter to keep current value):\n";
    
    std::cout << "Advanced Math [" << oldStudent->math << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) newStudent.math = std::stod(input);
    
    std::cout << "C++ Programming [" << oldStudent->cpp << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) newStudent.cpp = std::stod(input);
    
    std::cout << "English [" << oldStudent->english << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) newStudent.english = std::stod(input);
    
    std::cout << "Linear Algebra [" << oldStudent->linearAlgebra << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) newStudent.linearAlgebra = std::stod(input);
    
    std::cout << "Political [" << oldStudent->political << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) newStudent.political = std::stod(input);
    
    // 重新计算成绩
    newStudent.calculateScores();
    
    // 更新学生信息
    if (studentManager.updateStudent(oldId, newStudent)) {
        std::cout << "\n Student information updated successfully!\n";
        newStudent.display();
    }
    
    DisplayHelper::pause();
}

// 查询学生
void searchStudents() {
    DisplayHelper::clearScreen();
    std::cout << "=== Search Students ===\n\n";
    
    std::cout << "Search by:\n";
    std::cout << "1. Student ID\n";
    std::cout << "2. Name\n";
    std::cout << "3. Department\n";
    std::cout << "4. Major\n";
    std::cout << "5. Class\n";
    
    int choice = InputHelper::getInt("Choose: ", 1, 5);
    
    std::string field, value;
    switch (choice) {
        case 1: field = "id"; break;
        case 2: field = "name"; break;
        case 3: field = "department"; break;
        case 4: field = "major"; break;
        case 5: field = "class"; break;
    }
    
    value = InputHelper::getString("Enter search keyword: ");
    
    auto results = studentManager.findStudentsByCondition(field, value);
    
    if (results.empty()) {
        std::cout << "\nNo students found.\n";
    } else {
        std::cout << "\nFound " << results.size() << " students:\n";
        DisplayHelper::displayStudentTable(results, true);
        
        // 显示详细信息选项
        if (InputHelper::confirm("\nView detailed information of a student?")) {
            std::string id = InputHelper::getString("Enter student ID: ");
            Student* student = studentManager.findStudent(id);
            if (student) {
                student->display();
            } else {
                std::cout << "Student not found!\n";
            }
        }
    }
    
    DisplayHelper::pause();
}

// 显示所有学生
void showAllStudents() {
    DisplayHelper::clearScreen();
    std::cout << "=== All Students ===\n\n";
    
    auto students = studentManager.getAllStudents();
    DisplayHelper::displayStudentTable(students);
    
    // 查看详细信息选项
    if (!students.empty() && InputHelper::confirm("\nView detailed information of a student?")) {
        std::string id = InputHelper::getString("Enter student ID: ");
        Student* student = studentManager.findStudent(id);
        if (student) {
            student->display();
        } else {
            std::cout << "Student not found!\n";
        }
    }
    
    DisplayHelper::pause();
}

// 显示统计信息
void showStatistics() {
    DisplayHelper::clearScreen();
    std::cout << "=== Statistics ===\n\n";
    
    auto stats = studentManager.getStatistics();
    DisplayHelper::displayStatistics(stats);
    
    DisplayHelper::pause();
}

// 排序学生
void sortStudents() {
    DisplayHelper::clearScreen();
    std::cout << "=== Sort Students ===\n\n";
    
    std::cout << "Sort by:\n";
    std::cout << "1. Student ID (Ascending)\n";
    std::cout << "2. Student ID (Descending)\n";
    std::cout << "3. Name (Ascending)\n";
    std::cout << "4. Name (Descending)\n";
    std::cout << "5. Average Score (Ascending)\n";
    std::cout << "6. Average Score (Descending)\n";
    
    int choice = InputHelper::getInt("Choose: ", 1, 6);
    
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
    std::cout << "\n Students sorted!\n";
    
    // 显示排序后的结果
    if (InputHelper::confirm("Show sorted results?")) {
        auto students = studentManager.getAllStudents();
        DisplayHelper::displayStudentTable(students);
    }
    
    DisplayHelper::pause();
}

// 备份数据
void backupData() {
    DisplayHelper::clearScreen();
    std::cout << "=== Backup Data ===\n\n";
    
    if (fileStorage.createBackup()) {
        std::cout << "\n Data backup successful!\n";
    }
    
    DisplayHelper::pause();
}

// 导入导出数据
void importExportData() {
    DisplayHelper::clearScreen();
    std::cout << "=== Import/Export Data ===\n\n";
    
    std::cout << "1. Export data to CSV\n";
    std::cout << "2. Import data from CSV\n";
    std::cout << "3. Return to main menu\n";
    
    int choice = InputHelper::getInt("Choose: ", 1, 3);
    
    if (choice == 1) {
        // 导出到CSV
        std::string filename = InputHelper::getString("Enter CSV filename (e.g., students.csv): ");
        auto students = studentManager.getAllStudents();
        
        if (fileStorage.exportToCSV(students, filename)) {
            std::cout << "\n Data export successful!\n";
        }
    } else if (choice == 2) {
        // 从CSV导入
        std::string filename = InputHelper::getString("Enter CSV filename: ");
        
        if (InputHelper::confirm("Import will overwrite current data. Continue?")) {
            auto importedStudents = fileStorage.importFromCSV(filename);
            
            if (!importedStudents.empty()) {
                studentManager.setStudents(importedStudents);
                std::cout << "\n Data import successful! Imported " << importedStudents.size() << " records\n";
            }
        }
    }
    
    DisplayHelper::pause();
}

// 保存数据
void saveData() {
    DisplayHelper::clearScreen();
    std::cout << "=== Save Data ===\n\n";
    
    auto students = studentManager.getAllStudents();
    if (fileStorage.saveStudents(students)) {
        std::cout << "\n Data saved successfully!\n";
    }
    
    DisplayHelper::pause();
}

// 重新加载数据
void reloadData() {
    DisplayHelper::clearScreen();
    std::cout << "=== Reload Data ===\n\n";
    
    if (InputHelper::confirm("Reload will lose unsaved changes. Continue?")) {
        auto loadedStudents = fileStorage.loadStudents();
        studentManager.setStudents(loadedStudents);
        std::cout << "\n Data reloaded successfully! Currently have " << studentManager.getCount() << " students\n";
    }
    
    DisplayHelper::pause();
}

// 主函数
int main() {
    // 显示欢迎信息
    DisplayHelper::showWelcome();
    
    // 加载已有数据
    std::cout << "\nLoading data..." << std::endl;
    auto loadedStudents = fileStorage.loadStudents();
    studentManager.setStudents(loadedStudents);
    std::cout << "System loaded " << studentManager.getCount() << " student records" << std::endl;
    
    DisplayHelper::pause();
    
    int choice;
    bool running = true;
    
    while (running) {
        DisplayHelper::displayMenu();
        std::cout << "Enter your choice: ";
        
        choice = getMenuChoice();
        
        switch(choice) {
            case 1: addStudent(); break;
            case 2: deleteStudent(); break;
            case 3: updateStudent(); break;
            case 4: searchStudents(); break;
            case 5: showAllStudents(); break;
            case 6: showStatistics(); break;
            case 7: studentManager.showFailingStudents(); DisplayHelper::pause(); break;
            case 8: sortStudents(); break;
            case 9: backupData(); break;
            case 10: importExportData(); break;
            case 11: saveData(); break;
            case 12: reloadData(); break;
            case 0: 
                std::cout << "\nSave data before exiting? (Y/N): ";
                if (InputHelper::confirm("Save data and exit?")) {
                    saveData();
                }
                running = false;
                std::cout << "\nThank you for using Student Management System! Goodbye!\n";
                break;
            default:
                std::cout << "Invalid choice, please try again!\n";
                DisplayHelper::pause();
        }
    }
    
    return 0;
}