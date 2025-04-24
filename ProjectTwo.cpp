//
//  main.cpp
//  CS300 Project 2
//
//  Created by Bryan Dailey on 4/16/25.
//

#include <algorithm>
#include <climits>
#include <iostream>
#include <string> // atoi
#include <time.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <unordered_set>


//define structure to hold course data
struct Course {
    std::string courseNumber;
    std::string courseName;
    std::vector<std::string> preReq;

    //default constructor
    Course() {
        this->courseNumber = "";
        this->courseName = "";
        this->preReq = {};
    }

    //initialize a course with data
    Course(std::string courseNumber, std::string courseName, std::vector<std::string> preReq) {
        this->courseNumber = courseNumber;
        this->courseName = courseName;
        this->preReq = preReq;
    }
};

//Internal structure for binarysearchtree
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
        course = aCourse;
    }
};


/*
Define a class BinarySearchTree that stores and organizing courses.
*/
class BinarySearchTree {
private:
    Node* root;
    void deleteTree(Node* node);
    void addNode(Node* node, Course course);
    void inOrder(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void printCourses();
    void Insert(Course course);
    Course Search(std::string courseNumber);


};
/*
*Constructor
*/
BinarySearchTree::BinarySearchTree() {
    //root is equal to nullptr
    root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
    deleteTree(root);
    root = nullptr;
}

/*
Helper for deleting all nodes in a tree used by destructor.
*/
void BinarySearchTree::deleteTree(Node* node) {
    //if node is null exit 
    if (node == nullptr) {
        return;
    }
    //if there is a left node travese left 
    if (node->left != nullptr) {
        deleteTree(node->left);
    }
    //if there is a right node travese right
    if (node->right != nullptr) {
        deleteTree(node->right);
    }
    delete node;
}
/*
Helper for adding nodes to a tree for courses.
*/
void BinarySearchTree::addNode(Node* node, Course course) {

    // if node is larger then add to left
    if (node->course.courseNumber.compare(course.courseNumber) > 0) {
        // if no left node
        if (node->left == nullptr) {
            // this node becomes left
            node->left = new Node(course);
        }
        // else recurse down the left node
        else {
            addNode(node->left, course);
        }
    }
    // else
    else {
        // if no right node
        if (node->right == nullptr) {
            // this node becomes right
            node->right = new Node(course);
        }
        //else
        else {
            // recurse down the right node
            addNode(node->right, course);
        }
    }

}
/*
Helper for printing courses in order
*/
void BinarySearchTree::inOrder(Node* node) {
    //if node is not equal to null ptr
    if (node != nullptr) {
        //InOrder not left
        inOrder(node->left);
        //output Course Number and Course Name
        std::cout << node->course.courseNumber << ", " << node->course.courseName << std::endl;
        //InOrder right
        inOrder(node->right);
    }
}

/*
Print all loaded courses 
*/
void BinarySearchTree::printCourses() {
    // call inOrder fuction and pass root 
    inOrder(root);
}

/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course course) {
    // if root equarl to null ptr
    if (root == nullptr) {
        // root is equal to new node course
        root = new Node(course);
    }
    // else
    else {
        // add Node root and bid
        addNode(root, course);
    }
}

/**
 * Search for a course
 */
Course BinarySearchTree::Search(std::string courseNumber) {
    // set current node equal to root
    Node* currentNode = root;

    // keep looping downwards until bottom reached or matching bidId found
    while (currentNode != nullptr) {
        // if match found, return current bid
        if (currentNode->course.courseNumber.compare(courseNumber) == 0) {
            return currentNode->course;
        }

        // if bid is smaller than current node then traverse left
        if (courseNumber.compare(currentNode->course.courseNumber) < 0) {
            currentNode = currentNode->left;
        }
        // else larger so traverse right
        else {
            currentNode = currentNode->right;
        }
    }
    Course course;
    return course;
}
/*
Search for courses and print the information to the screen
*/
std::string searchCourse(BinarySearchTree* courses, std::string courseNumber) {
    Course course;
    std::string returnValue;
    std::string preReqs;
    //Search tree for the course 
    course = courses->Search(courseNumber);
    // get all preReqs and add to temporary string
    for (int i = 0; i < course.preReq.size(); i++) {
        preReqs = preReqs + course.preReq[i] + " ";
    }
    //Output Course Number, Course Name and PreReqs
    returnValue = course.courseNumber + ", " + course.courseName + "\nPrerequisites: " + preReqs + "\n";
    //Return the string
    return returnValue;
}
/*
Load all courses to the tree from the file
*/
void loadCourse(BinarySearchTree* courses, std::string fileName) {
    //Load file
    std::ifstream file(fileName);

    //open file
    if (file.is_open()) {
        std::string line;

        //get next line 
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string token;
            std::string courseNumber;
            std::string courseName;
            std::vector<std::string> preReqs;

            //get first token and assign to course number 
            if (std::getline(ss, token, ',')) {
                courseNumber = token;
            }

            //get next token and assign to course name
            if (std::getline(ss, token, ',')) {
                courseName = token;
            }

            //add the remaining tokens to preReqs 
            while (std::getline(ss, token, ',')) {
                preReqs.push_back(token);
            }

            //create new course using the tokens.
            Course newCourse(courseNumber, courseName, preReqs);
            //add course to the binary tree
            courses->Insert(newCourse);
            
        }
    }
}

/*
Validate document follows the correct format
*/
bool validateDocument(std::string fileName) {
    //load file
    std::ifstream file(fileName);
    std::unordered_set<std::string> existingCourses;
    bool valid = true;
    std::string line;

    //load file 
    if (file.is_open()) {
        //get next line
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string token; 
            std::vector<std::string> courseData;
            //add all tokens to a vector
            while (std::getline(ss, token, ',')) {
                courseData.push_back(token);
            }
            //if there are less than 2 tokens return false 
            if (courseData.size() < 2) {
                valid = false;
                break;
            }
            // get current courseID
            std::string currentCourseID = courseData[0];
            //for all prerequisites 
            for(int i = 2; i < courseData.size(); ++i) {
                // if no match is found return false
                if (existingCourses.find(courseData[i]) == existingCourses.end()) {
                    valid = false;
                    break;
                }
            }
            //add course number to list of existing courses
            existingCourses.insert(token);

        }
        //close file
        file.close();
    }
    
    return valid;
}



int main(int argc, const char * argv[]) {
    int choice = 0;
    std::string fileName;
    BinarySearchTree* courses;
    courses = new BinarySearchTree();
    while (choice != 9) {
        std::cout << "Menu:" << std::endl;
        std::cout << "  1. Load Data Structure." << std::endl;
        std::cout << "  2. Print Course List." << std::endl;
        std::cout << "  3. Print Course." << std::endl;
        std::cout << "  9. Exit" << std::endl;
        std::cout << "Enter choice: ";
        std::cin >> choice;

            switch (choice) {

            case 1:

                    std::cout << "What is the file to load? ";
                    std::cin >> fileName;
                    loadCourse(courses, fileName);
                    /*
                    if(validateDocument(fileName)){
                        std::cout << "Your file " << fileName << " is valid." << std::endl;
                        loadCourse(courses, fileName);
                    }
                    else{
                        std::cout << "Invalid file." << std::endl;
                    }
                    */
                break;

            case 2:
                std::cout << "Here are the current courses: " << std::endl;
                courses->printCourses();
                break;

            case 3:
                std::cout << "What course do you want to know about? ";
                std::string search;
                std::cin >> search;
                std::string course = searchCourse(courses, search);
                std::cout << course;

                break;

            }
        }

    std::cout << "Good bye." << std::endl;
    return 0;
}
