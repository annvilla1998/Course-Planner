# Course-Planner

## Project Overview

Course-Planner is a C++ application designed to help students and advisors plan academic courses. It reads course data from a file, stores courses in a hash table for fast lookup, and models prerequisite relationships using a graph structure.

### Key Features

- **Fast Course Lookup:** Uses a hash table for O(1) retrieval of course information.
- **Graph-Based Prerequisites:** Represents course prerequisites as a graph, allowing traversal and dependency checks.
- **Efficient Sorting:** Implements merge sort for reliable and fast course sorting.
- **File-Based Input:** Loads course data from a text file (`courses.txt`).

### Example Course Data

The `courses.txt` file contains course numbers, names, and prerequisites, e.g.:

```
CSCI300,Introduction to Algorithms,CSCI200,MATH201
CSCI350,Operating Systems,CSCI300
CSCI101,Introduction to Programming in C++,CSCI100
CSCI100,Introduction to Computer Science
CSCI301,Advanced Programming in C++,CSCI101
CSCI400,Large Software Development,CSCI301,CSCI350
CSCI200,Data Structures,CSCI101
```

### Technologies Used

- C++
- Standard Library: `unordered_map`, `vector`, `list`, file I/O

---