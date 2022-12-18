// ====================================================================================
// Name                 : Project 2 Shawn Murphy.cpp
// Author               : Shawn Murphy
// Version              : 1.0
// Description          : Project two in C++. Reads CSV and prints the required info
// ====================================================================================


#include <iostream>
#include <fstream>
#include "CSVparser.hpp"

using namespace std;

//===============================================================================
//Global Definitions visible to all Methods and classes
//===============================================================================

/*
Each course contains an ID, Name and a Vector that holds the courses prereqs
*/


// define structure to hold course info
struct Course {
    string courseNum;
    string courseName;
    vector<string> coursePrereq;

    Course() {}
};

// Internal structure for tree node
struct Node {
    Course course;
    Node* left;
    Node* right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }
    // initialize with a course
    Node(Course aCourse) :
        Node() {
        this->course = aCourse;
    }
};
//===============================================================================
// BST class definition
//===============================================================================
/*
*Define class containing members and methods to
*implement a binary search tree
*/

class CourseTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void printSampleSched(Node* node);
    void printCourseInfo(Node* node, string courseNum);


public:
    CourseTree();
    virtual ~CourseTree();
    void DeleteRecursive(Node* node);
    void Insert(Course course);
    int NumPrereqCourses(Course course);
    void PrintSampleSched();
    void PrintCourseInfo(string courseNum);
};
//Construct BST and initialize root
CourseTree::CourseTree() {
    root = nullptr;
}

/*
Deconstruct BST
*/
CourseTree::~CourseTree() {
    DeleteRecursive(root);
}
/*
Deconstruct BST nodes recursively
used by deconstructor
*/
void CourseTree::DeleteRecursive(Node* node) {
    if (node) {
        DeleteRecursive(node->left);
        DeleteRecursive(node->right);
        delete node;
    }
}

//Node insert in BST

void CourseTree::Insert(Course course) {
    //if the root is not initialized
    if (root == nullptr) {
        //iniitialize with current course
        root = new Node(course);
    }
    else {
        //otherwise add to tree through method
        this->addNode(root, course);
    }
}
//Return the number of Prereq a course has
int CourseTree::NumPrereqCourses(Course course) {
    int count = 0;
    for (unsigned int i = 0; i < course.coursePrereq.size(); i++) {
        if (course.coursePrereq.at(i).length() > 0) {
            count++;
        }
    }
    return count;
}

//Pass BST root to private method

void CourseTree::PrintSampleSched() {
    this->printSampleSched(root);
}
//Pass BST root and courseID to be found and display if found
void CourseTree::PrintCourseInfo(string courseNum) {
    this->printCourseInfo(root, courseNum);
}

/*
Uses public insert method
calculates and sets alphanumeric location,
if current child is nullptr adds the node to the BST,
otherwise keeps traversing BST until nullptr is found.
*/
void CourseTree::addNode(Node* node, Course course) {
    //current course number is less than the current nodes course number
    if (node->course.courseNum.compare(course.courseNum) > 0) {
        if (node->left == nullptr) {
            node->left = new Node(course);
        }
        else {
            this->addNode(node->left, course);
        }
    }
    else {
        if (node->right == nullptr) {
            node->right = new Node(course);
        }
        else {
            this->addNode(node->right, course);
        }
    }
}
//prints loaded courses in order
void CourseTree::printSampleSched(Node* node) {
    if (node != nullptr) {
        printSampleSched(node->left);
        std::cout << node->course.courseNum << ", " << node->course.courseName << endl;
        printSampleSched(node->right);
    }
    return;
}

//Display passed course and it's prereqs
void CourseTree::printCourseInfo(Node* curr, string courseNum) {
    //Search BST until corresponding courseNum found or bottom is reached
    while (curr != nullptr) {
        //if passed course number matches current course number
        if (curr->course.courseNum.compare(courseNum) == 0) {
            std::cout << endl << curr->course.courseNum << ", " << curr->course.courseName << endl;
            unsigned int size = NumPrereqCourses(curr->course);
            std::cout << "Prerequisite(s): ";
            //if prereqs are found output in proper format
            unsigned int i = 0;
            for (i = 0; i < size; i++) {
                std::cout << curr->course.coursePrereq.at(i);
                if (i != size - 1) {
                    std::cout << ", ";
                }

            }
            // no prereqs found notifier
            if (i == 0) {
                std::cout << "No prerequisites found.";
            }
            std::cout << endl;
            return;
        }
        //courseNum smaller than current traverse left
        else if (courseNum.compare(curr->course.courseNum) < 0) {
            curr = curr->left;
        }
        // passed courseNum is larger than current
        else {
            curr = curr->right;
        }
        //null ptr reached without finding course
        std::cout << "Course " << courseNum << " not found." << endl;
    }
}

//===============================================================================
//Static methods for testing
//===============================================================================

//Load Courses from cin choice for file
bool loadCourses(string csvPath, CourseTree* coursesTree) {
    //Open course file and parse lines
    try {
        ifstream courseFile;
        courseFile.open(csvPath.c_str());

        if (courseFile.is_open()) {
            while (!courseFile.eof()) {
                //vector holds each csv the course holds
                vector<string> courseInf;
                string courseData;

                getline(courseFile, courseData);
                while (courseData.length() > 0) {
                    unsigned int comma = courseData.find(',');
                    if (comma < 100) {
                        courseInf.push_back(courseData.substr(0, comma));
                        courseData.erase(0, comma + 1);
                    }
                    else {
                        courseInf.push_back(courseData.substr(0, courseData.length()));
                        courseData = "";
                    }
                }
                //load separated values into a course and add into BST then close
                Course course;
                course.courseNum = courseInf[0];
                course.courseName = courseInf[1];

                for (unsigned int i = 2; i < courseInf.size(); i++) {
                    course.coursePrereq.push_back(courseInf[i]);
                }
                coursesTree->Insert(course);
            }
            courseFile.close();

            return true;
        }
    }
    catch (csv::Error& e) {
        cerr << e.what() << endl;
    }
    return false;
}

//===============================================================================
//Main Method
//===============================================================================
int main(int argc, char* argv[])
{
    // Processing command line argument
    string csvPath, courseId;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        break;
    case 3:
        csvPath = argv[1];
        courseId = argv[2];
        break;
    default:
        csvPath = "";
        break;
    }
    //Define BST and Welcome User
    CourseTree* coursesTree = nullptr;
    std::cout << "\nWelcome to your course planner!\n" << endl;

    //Convert user choice to string and first char to int preventing invalid data
    string choice = "0";
    int userChoice = choice[0] - '0';

    //if not 9 or exit
    while (userChoice != 9) {
        std::cout << " 1. Load Data Structure" << endl;
        std::cout << " 2. Print Course List" << endl;
        std::cout << " 3. Print Course" << endl;
        std::cout << " 9. Exit" << endl;
        std::cout << "\nWhich action would you like to perform? ";
        cin >> choice;
    


        // Check for if user input is double digit
        if (choice.length() == 1) {
            userChoice = choice[0] - '0';
        }
        else {
            userChoice = 0;
        }
        bool succeed = 1;

        //Performs user choice from menu
        switch (userChoice) {
            /*Instantiate BST
            get file path
            load courses into BST
            */
        case 1:
            if (coursesTree == nullptr) {
                coursesTree = new CourseTree();
                if (csvPath.length() == 0) {
                    std::cout << "Enter the file name that contains the course data: ";
                    cin >> csvPath;
                }
                succeed = loadCourses(csvPath, coursesTree);
                if (succeed) {
                    std::cout << "Courses have been loaded.\n" << endl;
                }
                else {
                    std::cout << " File not found.\n" << endl;
                    coursesTree = nullptr;
                }
                csvPath = "";
                break;
            }
            // Check if BST exists and outputs an example schedule.
        case 2:
            if ((coursesTree != nullptr) && succeed) {
                std::cout << "Here is an example Schedule:\n" << endl;
                coursesTree->PrintSampleSched();
                std::cout << endl;
            }
            else {
                std::cout << "Load courses first. (option 1)\n" << endl;
            }
            break;
        case 3:
            if ((coursesTree != nullptr) && succeed) {
                if (courseId.length() == 0) {
                    std::cout << "What course do you want to know about? ";
                    cin >> courseId;
                    for (auto& userChoice : courseId) userChoice = toupper(userChoice);
                }
                coursesTree->PrintCourseInfo(courseId);
                std::cout << endl;
            }
            else {
                std::cout << "Load courses first. (option 1)\n" << endl;

            }
            courseId = "";
            break;
        default:
            if (userChoice != 9) {
                std::cout << choice << " is not a valid option\n" << endl;

            }
            break;
        }
    }
    std::cout << "\nThank you for using this course planner!" << endl;

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

