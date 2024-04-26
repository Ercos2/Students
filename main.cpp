#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <thread>
#include <mutex>

struct Student {
    int id;
    std::string name;
    int age;

    Student(int id, const std::string& name, int age) : id(id), name(name), age(age) {}
};

class StudentDatabase {
private:
    std::map<int, std::shared_ptr<Student>> students;

public:
    void addStudent(int id, const std::string& name, int age) {
        if(students.find(id) != students.end())
            std::cout << "Student with this id is alredy here\n";
        else
            students[id] = std::make_shared<Student>(id, name, age);
    }

    void removeStudent(int id) {
        if(students.find(id) != students.end())
            std::cout << "Student with this id is alredy gone\n";
        else
            students.erase(id);
    }

    std::shared_ptr<Student> getStudent(int id) {
        if(students.find(id) != students.end())
            return students[id];
        else return nullptr;
    }
};

std::mutex mtx;
StudentDatabase stude_db;

void writer() {
    for (int i = 0; i < 1000; ++i) {
        std::lock_guard<std::mutex> lock(mtx);
        stude_db.addStudent(i, (i%2==1 ? "Vany" : "vika"), i * rand());
        std::cout << "Add Student: " << (i%2==1 ? "Vany" : "vika") << "\n";
    }
}

void reader() {
    for (int i = 0; i < 1000; ++i) {
        std::lock_guard<std::mutex> lock(mtx);
        if (stude_db.getStudent(i) != nullptr)
            std::cout << "Student : " << stude_db.getStudent(i)->name << "\n";
        else
            std::cout << "Nosuch student\n";
    }
}

int main() {
    std::thread t1(writer);
    std::thread t2(reader);

    t1.join();
    t2.join();

    return 0;
}
