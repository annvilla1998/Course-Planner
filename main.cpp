#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <unordered_map>

using namespace std;

/**
 * Enhanced Course Planner
 *
 * Key Enhancements:
 * - Hash Table: O(1) course lookup instead of O(n) linear search
 * - Graph Structure: Adjacency list for prerequisite relationships and BFS traversal
 * - Merge Sort: Custom O(n log n) sorting with guaranteed performance vs stdout sort with worst case O(n^2) sorting
 */



/**
 * Course Structure
 *
 * Represents a single course with its number, name, and prerequisites.
 */
struct Course
{
    string courseNumber; // Unique identifier for the course ("CS101")
    string name; // Full course name ("Introduction to Computer Science")
    vector<string> prerequisites; // List of required prerequisite course numbers
};


/**
 * Hash Table Implementation for Course Storage
 *
 * Enhancement: Replaces linear search O(n) with hash table lookup O(1)
 *
 * Uses unordered_map internally for fast course retrieval by course number.
 * Implements case-insensitive searching to match original functionality.
 *
 * Time Complexities:
 * - Insert: O(1) average case
 * - Find: O(1) average case
 * - GetAll: O(n) - returns all courses as vector
 */
class CourseHashTable {
private:
    unordered_map<string, Course> courseMap;  // Internal hash table storage
    
    /**
     * Convert string to lowercase for case-insensitive operations
     * @param str Input string to convert
     * @return Lowercase version of input string
     */
    string toLower(string str) {
        transform(str.begin(), str.end(), str.begin(), ::tolower);
        return str;
    }
    
public:
    /**
     * Insert a course into the hash table
     * @param course Course object to insert
     * Time Complexity: O(1) average case
     */
    void insert(const Course& course) {
        courseMap[toLower(course.courseNumber)] = course;
    }
    
    /**
     * Find a course by course number
     * @param courseNumber Course number to search for
     * @return Pointer to Course if found, nullptr if not found
     * Time Complexity: O(1) average case
     */
    Course* find(const string& courseNumber) {
        string lowerKey = toLower(courseNumber);
        auto it = courseMap.find(lowerKey);
        return (it != courseMap.end()) ? &it->second : nullptr;
    }
    
    /**
     * Get all courses as a vector
     * @return Vector containing all courses in the hash table
     * Time Complexity: O(n)
     */
    vector<Course> getAllCourses() {
        vector<Course> courses;
        for (const auto& pair : courseMap) {
            courses.push_back(pair.second);
        }
        return courses;
    }
    
    /**
     * Check if hash table is empty
     * @return true if empty, false otherwise
     */
    bool empty() const {
        return courseMap.empty();
    }
    
    /**
     * Get number of courses in hash table
     * @return Number of courses stored
     */
    size_t size() const {
        return courseMap.size();
    }
};


/**
 * Graph Data Structure for Prerequisite Relationships
 *
 * Enhancement: Implements adjacency list representation for complex prerequisite analysis
 *
 * Features:
 * - Adjacency List: Maps each course to its prerequisites
 * - Reverse List: Maps each course to courses that depend on it
 * - BFS Implementation: Find courses available after completing prerequisites
 */
class PrerequisiteGraph {

private:
    unordered_map<string, list<string>> adjacencyList; // course -> its prerequisites
    unordered_map<string, list<string>> reverseList; // course -> courses that depend on it
    
    /**
     * Convert string to lowercase for consistent key handling
     * @param str Input string to convert
     * @return Lowercase version of input string
     */
    string toLower(string str) {
        transform(str.begin(), str.end(), str.begin(), ::tolower);
        return str;
    }
    
public:
    /**
     * Add a course and its prerequisite relationships to the graph
     * @param course Course object containing prerequisite information
     * Time Complexity: O(p) where p is number of prerequisites
     */
    void addCourse(const Course& course) {
        string courseKey = toLower(course.courseNumber);
        
        adjacencyList[courseKey] = list<string>();
        
        for (const string& prereq : course.prerequisites) {
            string prereqKey = toLower(prereq);
            adjacencyList[courseKey].push_back(prereqKey);
            reverseList[prereq].push_back(courseKey);
        }
    }
    
    /**
     * Find courses that become available after completing a given course
     *
     * Uses Breadth-First Search (BFS) to traverse prerequisite relationships
     *
     * @param completedCourse Course number that has been completed
     * @param hashTable Reference to hash table for course lookup
     * @return Vector of course numbers that become available
     * Time Complexity: O(V + E) where V is courses, E is prerequisite relationships
     */
    vector<string> findAvailableCourses(const string& completedCourse, const CourseHashTable& hashTable) {
        vector<string> available;
        string courseKey = toLower(completedCourse);
        
        if(reverseList.find(courseKey) == reverseList.end()) {
            return available;
        }
        
        queue<string> q;
        unordered_map<string, bool> visited;
        
        for(const string& dependent : reverseList[courseKey]) {
            q.push(dependent);
        }
        
        while (!q.empty()) {
            string current = q.front();
            q.pop();
            
            if(visited[current]) continue;
            visited[current] = true;
            
            bool allPrereqsMet = true;
            for(const string& prereq : adjacencyList[current]) {
                if(toLower(prereq) != courseKey) {
                    allPrereqsMet = false;
                    break;
                }
            }
            
            if(allPrereqsMet) {
                available.push_back(current);
                
                if(reverseList.find(current) != reverseList.end()) {
                    for(const string& next : reverseList[current]) {
                        if(!visited[next]) {
                            q.push(next);
                        }
                    }
                }
            }
        }
        
        return available;
    }
    
    /**
     * Get prerequisites for a specific course
     * @param courseNumber Course to get prerequisites for
     * @return List of prerequisite course numbers
     */
    list<string> getPrerequisites(const string& courseNumber) {
        string courseKey = toLower(courseNumber);
        return adjacencyList[courseKey];
    }
};


/**
 * Custom Merge Sort Implementation
 *
 * Enhancement: Replaces std::sort with guaranteed O(n log n) performance
 *
 * - Guaranteed O(n log n) time complexity in all cases
 * - Stable sorting (maintains relative order of equal elements)
 * - Predictable performance regardless of input data distribution
 *
 * std::sort typically uses introsort which can degrade to O(n²) in worst case,
 * while merge sort maintains consistent O(n log n) performance.
 */
class MergeSort {
public:
    /**
     * Public interface for merge sort
     * @param courses Vector of courses to sort
     * @param left Starting index
     * @param right Ending index
     * Time Complexity: O(n log n) guaranteed
     * Space Complexity: O(n) for temporary arrays
     */
    static void mergeSort(vector<Course>& courses, int left, int right) {
        if(left < right) {
            int mid = left + (right - left) / 2;
            
            mergeSort(courses, left, mid);
            mergeSort(courses, mid + 1, right);
            
            merge(courses, left, mid, right);
        }
    }
    
private:
    /**
     * Merge two sorted subarrays into a single sorted array
     * @param courses Vector containing the subarrays to merge
     * @param left Starting index of left subarray
     * @param mid Ending index of left subarray
     * @param right Ending index of right subarray
     */
    static void merge(vector<Course>& courses, int left, int mid, int right) {
        int n1 = mid - left + 1;
        int n2 = right - mid;
        
        vector<Course> leftArray(n1);
        vector<Course> rightArray(n2);
        
        for(int i = 0; i < n1; i++) {
            leftArray[i] = courses[left + i];
        }
        for(int j = 0; j < n2; j++) {
            rightArray[j] = courses[mid + 1 + j];
        }
        
        int i = 0, j = 0, k = left;
        
        while (i < n1 && j < n2) {
            if(leftArray[i].courseNumber <= rightArray[j].courseNumber) {
                courses[k] = leftArray[i];
                i++;
            } else {
                courses[k] = rightArray[j];
                j++;
            }
            k++;
        }
        
        while(i <n1) {
            courses[k] = leftArray[i];
            i++;
            k++;
        }
        
        while(j < n2) {
            courses[k] = rightArray[j];
            j++;
            k++;
        }
    }
};


// Global data structures
CourseHashTable courseHashTable; // Hash table for O(1) course lookup
PrerequisiteGraph prereqGraph; // Graph for prerequisite relationships
bool dataLoaded = false; // Flag to track if courses are loaded


/**
 * Parse a comma-separated string into a vector of strings
 *
 * @param s Input string to parse
 * @param del Delimiter to split on (default: comma)
 * @return Vector of parsed string tokens
 */
vector<string> format(const string s, const string del = ",")
{
    vector<string> result;
    size_t start = 0, end = s.find(del);
    while( end != string::npos)
    {
        result.push_back(s.substr(start, end - start));
        start = end + del.size();
        end = s.find(del, start);
    }
    result.push_back(s.substr(start, end - start));
    return result;
}


/**
 * Load courses from file and populate  data structures
 *
 * Enhancements:
 * 1. Populates hash table for O(1) lookups
 * 2. Builds prerequisite graph for relationship analysis
 * 3. Sorts array using custom merge sort
 *
 * @return Vector of loaded courses
 * Time Complexity: O(n log n) due to sorting
 */
vector<Course> loadCoursesFile()
{
    ifstream fin("courses.txt", ios::in);
    vector<Course> courses;
    string line;
    bool entered = false;
    
    // Clear existing data structure
    courseHashTable = CourseHashTable();
    prereqGraph = PrerequisiteGraph();

    while (getline(fin, line))
    {
        if(line == "-1") break;
        entered = true;

        Course course;
        vector<string> info = format(line);

        course.courseNumber = info[0];
        course.name = info[1];

        for(int i = 2; i < info.size(); i++)
        {
            course.prerequisites.push_back(info[i]);
        }

        courses.push_back(course);
        
        courseHashTable.insert(course);
        prereqGraph.addCourse(course);
    }
    
    // If the while loop was never entered, this means the file was never read.
    // If it was entered but the vector is empty, the file is empty.
    if (!entered) {
            std::cout << "Could not access courses file. Please check if loaded properly." << endl;
            dataLoaded = false;
        } else if (courses.size() == 0) {
            std::cout << "Courses file appears to be empty." << endl;
            dataLoaded = false;
        } else {
            MergeSort::mergeSort(courses, 0, courses.size() - 1);
            dataLoaded = true;
            
            std::cout << "Data successfully loaded.\n" << endl;
        }
    
    fin.close();
    return courses;
}


/**
 * Print detailed information for a single course
 * @param course Course object to print
 */
void printCourse(Course course)
{
    cout << course.courseNumber << ", " << course.name << endl;
    
    if (course.prerequisites.size() == 0) {
        cout << "No prerequisites\n" << endl;
        return;
    }
    
    cout << "Prerequisites: ";
    for (size_t i = 0; i < course.prerequisites.size(); ++i) {
        std::cout << course.prerequisites[i];
        
        // Print a comma after each prerequisite except for the last one
        if (i < course.prerequisites.size() - 1) {
            std::cout << ", ";
        }
    }
    cout << "\n\n";
}


/**
 * Print all courses in alphabetical order (previously sorted in loadCoursesFile)
 * @param courses Vector of courses to print
 */
void printCourseList(vector<Course> courses)
{
    if (courses.empty()) {
        cout << "No courses loaded. Please load data first.\n" << endl;
        return;
    }
    
    for(Course course: courses)
    {
        printCourse(course);
    }
}

/**
 * Search for and display a specific course
 *
 * Enhancement: Hash table lookup O(1) instead of linear search O(n) for boosted performance
 */
void searchCourse()
{
    string courseNumber;
    cout << "What course do you want to know about? ";
    cin >> courseNumber;
    cout << endl;
    
    if (!dataLoaded) {
        cout << "Please load courses first." << endl;
    }

    Course* found = courseHashTable.find(courseNumber);
    if (found) {
        printCourse(*found);
    } else {
        cout << "Course " << courseNumber << " not found.\n" << endl;
    }
}


/**
 * Main program with 4-option menu
 *
 * Menu Options:
 * 1. Load Data Structure - Parses and populates hash table, graph, and sorted array
 * 2. Print Course List - Prints sorted courses along with prerequisites
 * 3. Print Course - Uses hash table lookup to search for and print a specific course number and prerequisites
 * 4. Exit - Terminates the program
 */
int main()
{
    vector<Course> courses;

    cout << "Welcome to the course planner." << endl;
    cout << endl;

    while (true)
    {
        int input;
        cout << "\t 1. Load Data Structure." << endl;
        cout << "\t 2. Print Course List." << endl;
        cout << "\t 3. Print Course." << endl;
        cout << "\t 4. Exit" << endl;
        cout << endl;

        cout << "What would you like to do? ";
        cin >> input;
        cout << endl;
        if(input == 4)
        {
            break;
        }
        else if (input == 1)
        {
            courses = loadCoursesFile();
        }
        else if (input == 2)
        {
            printCourseList(courses);
        }
        else if (input == 3)
        {
            searchCourse();
        }
        else
        {
            cout << input;
            cout << " is not a valid option." << endl;
        }
    }
    cout << "Thank you for using the course planner!" << endl;

    return 0;
}
