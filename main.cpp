#include "io.hpp"
#include "student.hpp"

#include <iostream>
#include <vector>

// 全局管理器
StudentManager studentManager;
FileStorage fileStorage;



int main() {
    // 显示欢迎信息
    DisplayHelper::showWelcome();
    std::cout << "按回车键继续...";
    std::cin.get();
    
    // 加载已有数据
    std::cout << "\n正在加载数据...\n";
    auto loadedStudents = fileStorage.loadStudents();
    for (const auto& student : loadedStudents) {
        studentManager.addStudent(student);
    }
    std::cout << "系统已加载 " << studentManager.getCount() << " 名学生数据\n";
    
    DisplayHelper::pause();
    
    int choice;
    bool running = true;
    
    while (running) {
        DisplayHelper::displayMenu();
        choice = InputHelper::getString("请选择操作: ");
        
        switch (choice) {
            case 1: addStudent(); break;
            case 2: deleteStudent(); break;
            case 3: updateStudent(); break;
            case 4: searchStudents(); break;
            case 5: showAllStudents(); break;
            case 6: showStatistics(); break;
            case 7: 
                studentManager.showFailingStudents(); 
                DisplayHelper::pause(); 
                break;
            case 8: sortStudents(); break;
            case 9: saveData(); break;
            case 10: reloadData(); break;
            case 0: 
                if (InputHelper::confirm("保存数据并退出系统？")) {
                    saveData();
                }
                running = false;
                std::cout << "\n感谢使用学生信息管理系统！再见！\n";
                break;
        }
    }
    
    return 0;
}

