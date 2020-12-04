#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Faculty.h"
#include "Student.h"
#include "bst.h"

using namespace std;

// Helper functions
template <typename T>
T readInput(const string &prompt, const string &expectedTypeName);
string readString(const string &prompt);
void readDataFromFile(Bst<Faculty> *masterFaculty, Bst<Student> *masterStudent);
void writeDataToFile(Bst<Faculty> *masterFaculty, Bst<Student> *masterStudent);
int readNewId(const string &prompt, Bst<Faculty> *masterFaculty, Bst<Student> *masterStudent);
int showMenu();

// Command Processor
bool command_01(Bst <Faculty> *masterFaculty, Bst <Student> *masterStudent);
bool command_02(Bst <Faculty> *masterFaculty, Bst <Student> *masterStudent);
bool command_03(Bst <Faculty> *masterFaculty, Bst <Student> *masterStudent);
bool command_04(Bst <Faculty> *masterFaculty, Bst <Student> *masterStudent);
bool command_05(Bst <Faculty> *masterFaculty, Bst <Student> *masterStudent);
bool command_06(Bst <Faculty> *masterFaculty, Bst <Student> *masterStudent);
bool command_07(Bst <Faculty> *masterFaculty, Bst <Student> *masterStudent);
bool command_08(Bst <Faculty> *masterFaculty, Bst <Student> *masterStudent);
bool command_09(Bst <Faculty> *masterFaculty, Bst <Student> *masterStudent);
bool command_10(Bst <Faculty> *masterFaculty, Bst <Student> *masterStudent);
bool command_11(Bst <Faculty> *masterFaculty, Bst <Student> *masterStudent);
bool command_12(Bst <Faculty> *masterFaculty, Bst <Student> *masterStudent);

int main()
{
    // Initialize new empty bst trees.
    Bst <Faculty> *masterFaculty = new Bst<Faculty>;
    Bst <Student> *masterStudent = new Bst<Student>;

    // Tmp bst trees. Used for rollback.
    Bst <Faculty> *cachedFaculty = NULL;
    Bst <Student> *cachedStudent = NULL;

    // Read faculty and student data from files and insert into bst.
    readDataFromFile(masterFaculty, masterStudent);

    // Show menu and process choices
    int choice;
    while ( (choice = showMenu()) != 14 ) {

        if ( choice < 1 || choice > 14 ) {
            cout << "Error: Invalid choice\n";
            continue;
        }

        // Create copy of db before executing the action
        if ( choice != 13 ) {
            delete cachedFaculty;
            delete cachedStudent;
            cachedFaculty = new Bst<Faculty> ( *masterFaculty );
            cachedStudent = new Bst<Student> ( *masterStudent );
        }

        bool dbChanged = false;

        // Process choice
        switch (choice) {
            case 1:
                dbChanged = command_01(masterFaculty, masterStudent);
                break;
            case 2:
                dbChanged = command_02(masterFaculty, masterStudent);
                break;
            case 3:
                dbChanged = command_03(masterFaculty, masterStudent);
                break;
            case 4:
                dbChanged = command_04(masterFaculty, masterStudent);
                break;
            case 5:
                dbChanged = command_05(masterFaculty, masterStudent);
                break;
            case 6:
                dbChanged = command_06(masterFaculty, masterStudent);
                break;
            case 7:
                dbChanged = command_07(masterFaculty, masterStudent);
                break;
            case 8:
                dbChanged = command_08(masterFaculty, masterStudent);
                break;
            case 9:
                dbChanged = command_09(masterFaculty, masterStudent);
                break;
            case 10:
                dbChanged = command_10(masterFaculty, masterStudent);
                break;
            case 11:
                dbChanged = command_11(masterFaculty, masterStudent);
                break;
            case 12:
                dbChanged = command_12(masterFaculty, masterStudent);
                break;
            case 13:

                if ( !cachedFaculty || !cachedStudent ) {
                    cout << "Error: Previous action did not change database structure.\n";
                }

                // Rollback.
                // set master to cached version of db.
                else {
                    delete masterFaculty;
                    delete masterStudent;

                    masterFaculty = cachedFaculty;
                    masterStudent = cachedStudent;
                    cachedFaculty = NULL;
                    cachedStudent = NULL;

                    cout << "Success: Rollback Success\n";
                }

                dbChanged = false;
                break;
        }

        // Delete the cached copy because the action did not change the database.
        if ( !dbChanged ) {
            delete cachedFaculty;
            delete cachedStudent;
            cachedFaculty = NULL;
            cachedStudent = NULL;
        }

        cout << "\n\n";
    }

    writeDataToFile(masterFaculty, masterStudent);

    delete masterFaculty;
    delete masterStudent;
    delete cachedFaculty;
    delete cachedStudent;

}

// Reads space separated string from the user and returns it.
string readString(const string &prompt)
{
    while ( true ) {
        cout << prompt;

        // Read a line
        string line;
        getline(cin, line);

        if ( !line.empty() )
            return line;

        cout << "Error: Cannot be empty\n";
    }
}

// Reads a value of type T from cin and returns it.
// If user enters wrong value, an error message is printed and the
// user is asked again for the value.
template <typename T>
T readInput(const string &prompt, const string &expectedTypeName)
{
    while ( true ) {

        cout << prompt;

        // Read a line
        string line;
        getline(cin, line);

        // Create a string stream
        istringstream stream (line);

        // Read from stream
        T value;
        if ( stream >> value )
            return value;

        // Error
        else
            cout << "Error: Wrong Input. Expected " << expectedTypeName << ".\n";
    }
}


// Shows the menu and prompts the user for a choice and returns the selected choice.
int showMenu()
{
    cout << "Menu:\n"
         << " 1. Print all student info\n"
         << " 2. Print all faculty info\n"
         << " 3. Search student\n"
         << " 4. Search faculty\n"
         << " 5. Print student's faculty advisor\n"
         << " 6. Print faculty's advisees\n"
         << " 7. Add a new student\n"
         << " 8. Delete a student\n"
         << " 9. Add a faculty member\n"
         << "10. Delete a faculty\n"
         << "11. Change student advisor\n"
         << "12. Remove an advisee from a faculty\n"
         << "13. Rollback\n"
         << "14. Exit\n\n";

    return readInput<int>("Your choice: ", "int");
}

// Open facultyTable and studentTable files and initialize the tree.
// If any files do not exists the function just returns.
void readDataFromFile(Bst<Faculty> *masterFaculty, Bst<Student> *masterStudent)
{
    ifstream facultyTable ("facultyTable");
    ifstream studentTable ("studentTable");

    // File do not exists
    if (!facultyTable.is_open() || !studentTable.is_open()) {
        return;
    }

    int numFaculty;
    int numStudents;

    string line;

    // Read faculties from file and insert into the bst.
    getline(facultyTable, line);
    numFaculty = stoi(line);
    for (int i = 0; i < numFaculty; ++i) {
        Faculty faculty(facultyTable);
        masterFaculty->insert(faculty);
    }

    // Read students from file and insert into the bst.
    getline(studentTable, line);
    numStudents = stoi(line);
    for (int i = 0; i < numStudents; ++i) {
        Student student(studentTable);
        masterStudent->insert(student);
    }

}

// Write masterFaculty and masterStudent to file
void writeDataToFile(Bst<Faculty> *masterFaculty, Bst<Student> *masterStudent)
{
    ofstream facultyTable ("facultyTable");
    ofstream studentTable ("studentTable");

    // Error opening
    if (!facultyTable.is_open() || !studentTable.is_open()) {
        cerr << "Error: Saving data to files\n";
        return;
    }

    // Write faculty data
    facultyTable << masterFaculty->size() << "\n";
    Bst<Faculty>::iterator it1 = masterFaculty->begin();
    while ( it1 != masterFaculty->end() ) {
        (*it1).serialize(facultyTable);
        ++it1;
    }

    // Write student data
    studentTable << masterStudent->size() << "\n";
    Bst <Student>::iterator it2 = masterStudent->begin();
    while ( it2 != masterStudent->end() ) {
        (*it2).serialize(studentTable);
        ++it2;
    }
}


// Read an id from the user.
// If the id is being used by any student or faculty, then the user is notified and prompted again
// for the id.
int readNewId(const string &prompt, Bst<Faculty> *masterFaculty, Bst<Student> *masterStudent)
{
    while ( true ) {

        // Read id from user
        int id = readInput<int>(prompt, "int");

        Bst <Faculty>::iterator it1 = masterFaculty->search( id );
        Bst <Student>::iterator it2 = masterStudent->search( id );

        // Id not found in masterFaculty or masterStudent
        if ( id >= 0 && (it1 == masterFaculty->end() && it2 == masterStudent->end()) )
            return id;

        if ( id < 0 )
            cout << "Error: Id cannot be negative.\n";
        else
            cout << "Error: Id is already being used.\n";
    }

}

// Reads a student id from the user. If a student with that
// id does not exists then the user is asked again for the student id.
int readValidStudentId(const string &prompt, Bst<Student> *masterStudent)
{
    while ( true ) {
        int studentId = readInput<int>(prompt, "int");

        // Check if faculty exists
        Bst<Student>::iterator it = masterStudent->search(studentId);

        // Found
        if (it != masterStudent->end() )
            return studentId;

        cout << "Error: Student with this id does not exists.\n";
    }
}

// Reads a faculty id from the user. If a faculty with that
// id does not exists then the user is asked again for the faculty id.
int readValidFacultyId(const string &prompt, Bst<Faculty> *masterFaculty)
{
    while ( true ) {
        int facultyId = readInput<int>(prompt, "int");

        // Check if faculty exists
        Bst<Faculty>::iterator it = masterFaculty->search(facultyId);

        // Found
        if ( it != masterFaculty->end() )
            return facultyId;

        cout << "Error: Faculty with this id does not exists.\n";
    }
}


// Process commands

// Print all the student and their info
bool command_01(Bst <Faculty> *masterFaculty, Bst <Student> *masterStudent)
{
    Bst <Student>::iterator it = masterStudent->begin();

    cout << "\nNumber of students: " << masterStudent->size() << "\n";

    if ( masterStudent->size() )
        cout << "\n";

    while ( it != masterStudent->end() ) {
        cout << *it << "\n";
        ++it;
    }

    return false;
}


// Print all the faculty and their info
bool command_02(Bst <Faculty> *masterFaculty, Bst <Student> *masterStudent)
{
    Bst <Faculty>::iterator it = masterFaculty->begin();

    cout << "\nNumber of faculty: " << masterFaculty->size() << "\n";

    if ( masterFaculty->size() )
        cout << "\n";

    while ( it != masterFaculty->end() ) {
        cout << *it << "\n";
        ++it;
    }

    return false;
}

// Ask for student id and print student info.
bool command_03(Bst <Faculty> *masterFaculty, Bst <Student> *masterStudent)
{
    int studentID = readInput<int>("Enter Student ID: ", "int");

    // Search for student
    Bst<Student>::iterator it = masterStudent->search(studentID);

    // Not found
    if ( it == masterStudent->end() ) {
        cout << "Student Not Found.\n";
    }

    // Print info
    else {
        cout << *it << "\n";
    }

    return false;
}

// Ask for faculty id and print faculty info.
bool command_04(Bst <Faculty> *masterFaculty, Bst <Student> *masterStudent)
{
    int facultyID = readInput<int>("Enter Faculty ID: ", "int");

    // Search for faculty
    Bst<Faculty>::iterator it = masterFaculty->search(facultyID);

    // Not found
    if ( it == masterFaculty->end() ) {
        cout << "Faculty Not Found.\n";
    }

    // Print info
    else {
        cout << *it << "\n";
    }

    return false;
}

// Print student faculty info
bool command_05(Bst <Faculty> *masterFaculty, Bst <Student> *masterStudent)
{
    int studentID = readInput<int>("Enter Student ID: ", "int");

    // Search for student
    Bst<Student>::iterator it = masterStudent->search(studentID);

    // Not found
    if ( it == masterStudent->end() ) {
        cout << "Student Not Found.\n";
    }

    else {
        // search for faculty
        Bst<Faculty>::iterator fit = masterFaculty->search( (*it).advisor );

        // Print info
        cout << (*it).name << " Advisor details\n";
        cout << *fit << "\n";
    }

    return false;
}

// Ask for faculty id and print info of all their advises
bool command_06(Bst <Faculty> *masterFaculty, Bst <Student> *masterStudent)
{
    int facultyID = readInput<int>("Enter Faculty ID: ", "int");

    // Search for faculty
    Bst<Faculty>::iterator it = masterFaculty->search(facultyID);

    // Not found
    if ( it == masterFaculty->end() ) {
        cout << "Faculty Not Found.\n";
    }

    // Print faculty advises
    else {
        cout << "\nNumber of advises = " << (*it).advisees.size() << "\n";

        if ( !(*it).advisees.empty() )
            cout << "\n";

        // Loop through faculty advises
        for (int i = 0; i < (*it).advisees.size(); ++i) {
            Bst <Student>::iterator sit = masterStudent->search( (*it).advisees[i] );
            cout << *sit << "\n";
        }

    }

    return false;
}


// Add a new Student
bool command_07(Bst <Faculty> *masterFaculty, Bst <Student> *masterStudent)
{
    if ( masterFaculty->size() == 0 ) {
        cout << "Error: First add a faculty.\n";
        return false;
    }

    // Read info from user
    int studentID = readNewId("Student ID: ", masterFaculty, masterStudent);
    string name =  readString("Name: ");
    string level = readString("Level: ");
    string major = readString("Major: ");
    double gpa   = readInput<double>("GPA: ", "double");

    // Read faculty id
    int advisor = readInput<int>("Advisor ID: ", "int");
    if ( masterFaculty->search( advisor ) == masterFaculty->end() ) {
        cout << "Error: Advisor not found\n";
        return false;
    }

    // Insert in to bst
    Student student(studentID, name, level, major, gpa, advisor);
    masterStudent->insert(student);

    // Add the advisee to the faculty
    Bst <Faculty>::iterator it = masterFaculty->search( advisor );
    (*it).advisees.push_back( studentID );

    cout << "Success: Student Added\n";
    return true;
}

// Delete student
bool command_08(Bst <Faculty> *masterFaculty, Bst <Student> *masterStudent)
{
    if ( masterStudent->size() == 0 ) {
        cout << "Error: No Students in database.\n";
        return false;
    }

    // Read student id;
    int studentId = readInput<int>("Student ID: ", "int");
    Bst <Student>::iterator it = masterStudent->search( studentId );
    if ( it == masterStudent->end() ) {
        cout << "Error: Student not found\n";
        return false;
    }

    // Get advisor
    int facultyId = (*it).advisor;

    // Remove student
    masterStudent->remove(it);

    // Find the advisor for this student
    Bst <Faculty>::iterator fit = masterFaculty->search(facultyId);

    // Remove the student id from the faculty advisees
    vector<int>::iterator vit = std::find((*fit).advisees.begin(), (*fit).advisees.end(), studentId);
    (*fit).advisees.erase(vit);

    cout << "Success: Student deleted\n";
    return true;
}

// Add a faculty
bool command_09(Bst <Faculty> *masterFaculty, Bst <Student> *masterStudent)
{
    // Read info
    int facultyId = readNewId("Faculty ID: ", masterFaculty, masterStudent);
    string name = readString("Name: ");
    string level = readString("Level: ");
    string department = readString("Department: ");

    // Insert
    Faculty faculty (facultyId, name, level, department);
    masterFaculty->insert(faculty);

    cout << "Success: Faculty Added\n";
    return true;
}

// Delete faculty
bool command_10(Bst <Faculty> *masterFaculty, Bst <Student> *masterStudent)
{
    if ( masterFaculty->size() == 0 ) {
        cout << "Error: No faculty in database.\n";
        return false;
    }

    // Read faculty id
    int facultyId = readInput<int>("Faculty ID: ", "int");
    Bst <Faculty>::iterator it = masterFaculty->search( facultyId );
    if ( it == masterFaculty->end() ) {
        cout << "Error: Faculty not found\n";
        return false;
    }

    if ( !(*it).advisees.empty() ) {
        cout << "Error: Cannot delete faculty. First change faculty advisees advisors.\n";
        return false;
    }

    masterFaculty->remove( it );

    cout << "Success: Faculty Deleted\n";

    return true;
}

// Change student advisor
bool command_11(Bst <Faculty> *masterFaculty, Bst <Student> *masterStudent)
{
    if ( masterStudent->size() == 0 ) {
        cout << "Error: No students in database.\n";
        return false;
    }

    // Read student id;
    int studentId = readInput<int>("Student ID: ", "int");
    Bst <Student>::iterator sit = masterStudent->search( studentId );
    if ( sit == masterStudent->end() ) {
        cout << "Error: Student not found\n";
        return false;
    }

    // Read faculty id
    int facultyId = readInput<int>("Faculty ID: ", "int");
    Bst <Faculty>::iterator fit2 = masterFaculty->search( facultyId );
    if ( fit2 == masterFaculty->end() ) {
        cout << "Error: Faculty not found\n";
        return false;
    }

    // iterator to student old advisor
    Bst <Faculty>::iterator fit1 = masterFaculty->search( (*sit).advisor );

    // Remove the student id from old advisor
    vector<int>::iterator vit = std::find((*fit1).advisees.begin(), (*fit1).advisees.end(), studentId);
    (*fit1).advisees.erase(vit);

    // Add student id to new advisort
    (*fit2).advisees.push_back(studentId);

    // Change student advisor
    (*sit).advisor = facultyId;

    cout << "Success: Student Advisor changed\n";

    return true;
}

// Remove advisee from a faculty
bool command_12(Bst <Faculty> *masterFaculty, Bst <Student> *masterStudent)
{
    cout << "Error: Removing an advisee will break referential integrity.\n";
    return false;
}
