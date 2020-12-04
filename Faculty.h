#ifndef BSTDB_FACULTY_H
#define BSTDB_FACULTY_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

class Faculty
{
public:
    int FacultyID;
    string name;
    string level;
    string department;
    vector <int> advisees;

    // Constructor
    Faculty(int facultyId, const string &name, const string &level, const string &department)
        : FacultyID(facultyId)
        , name(name)
        , level(level)
        , department(department)
    {}

    // Initialize from file
    Faculty(std::istream &stream)
        : FacultyID()
    {
        deserialize(stream);
    }

    // Equality operator
    bool operator==(const Faculty &other) const
    {
        return FacultyID == other.FacultyID;
    }

    // Less than operator
    bool operator<(const Faculty &other) const
    {
        return FacultyID < other.FacultyID;
    }

    // Greater than operator
    bool operator>(const Faculty &other) const
    {
        return FacultyID > other.FacultyID;
    }

    // Write Faculty data to output stream
    void serialize(std::ostream &stream) const
    {
        stream << FacultyID << "\n"
               << name << "\n"
               << level << "\n"
               << department << "\n";

        // Write advisees list
        stream << advisees.size() << "\n";

        for (int i = 0; i < advisees.size(); ++i)
            stream << advisees[i] << "\n";
    }

    // Read Faculty data from input stream
    void deserialize(std::istream &stream)
    {
        string line;

        getline(stream, line);
        FacultyID = stoi(line);  // string to int

        getline(stream, name);
        getline(stream, level);
        getline(stream, department);


        // Read advisees list
        getline(stream, line);
        int size = stoi(line);

        for (int i = 0; i < size; ++i) {
            getline(stream, line);
            int id = stoi(line);
            advisees.push_back(id);
        }
    }

};


// For cout << faculty
ostream& operator<<(ostream &stream, const Faculty &faculty)
{
    stream << "Faculty ID:   " << faculty.FacultyID << "\n";
    stream << "Name:         " << faculty.name << "\n";
    stream << "Level:        " << faculty.level << "\n";
    stream << "Department:   " << faculty.department << "\n";
    stream << "Num Advisees: " << faculty.advisees.size() << "\n";

    return stream;
}

// Comparison functions between integer and faculty.
// Used by Bst.

bool operator==(int id, const Faculty &faculty)
{
    return id == faculty.FacultyID;
}

bool operator<(int id, const Faculty &faculty)
{
    return id < faculty.FacultyID;
}

bool operator>(int id, const Faculty &faculty)
{
    return id > faculty.FacultyID;
}

#endif //BSTDB_FACULTY_H
