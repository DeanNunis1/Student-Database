#ifndef BSTDB_STUDENT_H
#define BSTDB_STUDENT_H

#include <string>
#include <iostream>

using namespace std;

class Student
{
public:
    int studentID;
    string name;
    string level;
    string majorName;
    double gpa;
    int advisor;

    // Constructor
    Student(int studentId, const string &name, std::string &level,
            std::string &pMajor, double gpa, int advisor)
        : studentID(studentId)
        , name(name)
        , level(level)
        , majorName(pMajor)
        , gpa(gpa)
        , advisor(advisor)
    {}

    // Initialize from file
    Student(istream &stream)
        : studentID(), gpa(), advisor()
    {
        deserialize(stream);
    }


    // Equality operator
    bool operator==(const Student &other) const
    {
        return studentID == other.studentID;
    }

    // Less than operator
    bool operator<(const Student &other) const
    {
        return studentID < other.studentID;
    }

    // Greater than operator
    bool operator>(const Student &other) const
    {
        return studentID > other.studentID;
    }

    // Write Student data to output stream
    void serialize(std::ostream &stream) const
    {
        stream << studentID << "\n"
               << name << "\n"
               << level << "\n"
               << majorName << "\n"
               << gpa << "\n"
               << advisor << "\n";
    }

    // Read student data from input stream
    void deserialize(std::istream &stream)
    {
        string line;

        getline(stream, line);
        studentID = stoi(line); // string to int

        getline(stream, name);
        getline(stream, level);
        getline(stream, majorName);
        getline(stream, line);
        gpa = stod(line);       // string to double

        getline(stream, line);
        advisor = stoi(line);  // string to int
    }

};

// For cout << student
ostream& operator<<(ostream &stream, const Student &student)
{
    stream << "Student ID: " << student.studentID << "\n";
    stream << "Name:       " << student.name << "\n";
    stream << "Level:      " << student.level << "\n";
    stream << "Major:      " << student.majorName << "\n";
    stream << "gpa:        " << student.gpa << "\n";
    stream << "Advisor ID: " << student.advisor << "\n";
    return stream;
}


// Comparison functions between integer and student.
// Used by Bst.

bool operator==(int id, const Student &student)
{
    return id == student.studentID;
}

bool operator<(int id, const Student &student)
{
    return id < student.studentID;
}

bool operator>(int id, const Student &student)
{
    return id > student.studentID;
}

#endif //BSTDB_STUDENT_H
