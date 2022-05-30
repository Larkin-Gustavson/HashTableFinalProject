#pragma once // a header guard
#ifndef HASH_TABLE_H // a header guard
#define HASH_TABLE_H // a header guard

#include <iostream> // used to output information in the Hash Table to the console
#include <string> // used as "keys" for the Hash Table
#include <cmath> // used for abs function
#include <fstream> // used for writing to a file

/*
	Author: Larkin Gustavson, Christian Gibbone, and Chadayne Westney
	Date of Creation: 12/3/2020
	Program's Purpose: To implement a student assignment tracking
					   system using a Hash Table. It will implement
					   several important methods such as within
					   the struct (Homework) Homework* clearLinkedList,
					   default constructor, parameterized constructor,
					   and void displayHomework. Within the class
					   (HashTable)the key methods implemented are
					   hashFunction, add, printTable, searchByDueDate,
					   searchByDueDateAndAssignment, searchByCourseNameAndAssignment,
					   assignmentsInProgress, searchByCourse, default constructor,
					   parameterized constructor, destructor, and remove methods.

	Collaborated with Chadayne Westney and Christian Gibbone.

	Credit goes to Dr. Zhao for the destructor implementation.

	Another source of credit goes to Paul Programming for the implementation of the Hashing Function,
	I (Larkin) got some minimal inspiration for the searchByDueDateAndAssignment and searchByCourseNameAndAssignment
	methods. His YouTube playlist can be found here:
	https://www.youtube.com/watch?v=MfhjkfocRR0&list=PLTxllHdfUq4f7-uHOpxXnBUbsuLiI9pmb

*/

// Used to store the information within the Hash Table (assignment name, course name, days due, pointer to the next entry in the Hash Table, removing an entry, adding information, and to display the information within an assignment).
struct Homework
{
	// Used to store the assignment name.
	std::string assignmentName;

	// Used to store the course name.
	std::string courseName;

	// Used to store the number of days an assignment is due (0 means today, 1 day means tomorrow, 2 days means the day after that, etc.).
	int daysDue;

	// Used to represent the next "node" or element in the Linked List.
	Homework* next;

	// Method to delete elements stored in a linked list (elements stored here are the result of a collision).
	Homework* clearLinkedList(Homework* head);

	// Method to display the contents for each "homework" item.
	void displayHomework() const;

	// Default constructor, to initialize the structure of the Hash Table.
	Homework();

	// Parameterized constructor.
	Homework(const std::string& assignmentName, const std::string& courseName, const int& daysDue);

	// The overloaded stream insertion operator (to print the contents of a Homework struct), for non pointer Homework objects.
	friend std::ostream& operator <<(std::ostream& output, const Homework& homework);

	// The overloaded stream insertion operator (to print the contents of a Homework struct), for pointer Homework objects.
	friend std::ostream& operator <<(std::ostream& output, const Homework* homework);
};

// Used to store the structure of the Hash Table (a sort of database that will hold multiple pieces of homework in it).
class HashTable
{
public:

	// A method to remove an element based on a assignment name from the Hash Table.
	void remove(const std::string assignmentName);

	// A method to see if a key exists in the Hash Table.
	bool keyExists(std::string assignmentName) const;

	// A method to print the contents of a bucket in the Hash Table.
	void printItemsInIndex(const size_t index);

	// A method to retrieve the numbers of items in a specific index in the Hash Table.
	int numberOfItemsInIndex(const size_t index);

	// A method to retrieve the current number of available buckets in the array that haven't had any elements added into them (50 spots in the array, represented by the variable s_TABLE_SIZE).
	size_t getNumberOfEmptyBuckets() const;

	// A method to see how many elements are in a certain position (based on a primary key of assignment name).
	size_t sizeInBucket(const std::string assignmentName);

	// A method to see if a Hash Table is empty (there are no entries made in the Hash Table).
	bool isEmpty() const;

	// A method to retrieve the total number of entries in the Hash Table.
	size_t getNumberOfEntries() const;

	// A method to add data based on a ideally unique key (the assignment name), it will add the assignment name, course name, and days due as "records" or rows in the Hash Table. It will also record this information into a "Assignments.txt" text file.
	void add(const std::string assignmentName, const std::string courseName, const int daysDue);

	// A method to print the "rows" of data in the Hash Table.
	void printTable() const;

	// A method to search the Hash Table based on an assignments due date (daysDue) and retrieve all associated information with it.
	Homework* searchByDueDate(int daysDue) const;

	// A method search based on a assignment name and days due.
	Homework* searchByDueDateAndAssignment(std::string assignmentName, int daysDue) const;

	// A method to search the Hash Table based on the course name (courseName) and retrieve all associated information with it.
	Homework* searchByCourseName(std::string courseName) const;

	// A method search based on a assignment name and course name.
	Homework* searchByCourseNameAndAssignment(std::string assignmentName, std::string courseName) const;

	// A method to clear a "row" or "bucket" from a Hash Table based on a key (assignment name).
	void clearBucket(const std::string assignmentName);

	// A method to update the course name.
	void updateCourseName(const std::string assignmentName, const std::string oldCourseName, const std::string newCourseName);

	// A method to update the day(s) due.
	void updateDaysDue(const std::string assignmentName, const int oldDaysDue, const int newDaysDue);

	// A method display the assignments that have either a due date of 0 or 1.
	void assignmentsInProgress() const;

	// A method to search the Hash Table based on a "primary key" working on the assumption that the "primary key" will only store one record.
	Homework* searchByAssignmentName(std::string assignmentName) const;

	// Default constructor, to initialize the default state of the Hash Table.
	HashTable();

	// Destructor, to free unused dynamic memory.
	~HashTable();

private:

	// Used to represent the size of the Hash Table, it is static so that it can be used later in other methods and that all instances of a Hash Table will have 50 spaces in them, const so that it cannot change during the programs execution, the s_ means that the variable is marked as static.
	static const size_t s_TABLE_SIZE = 50;

	// Used to represent the current number of available buckets in the array that haven't had any elements added into them (will be used to prevent the addition of elements beyond the size of the array).
	size_t numberOfEmptyBuckets = s_TABLE_SIZE;

	// Used to represent the number of entries in the Hash Table.
	size_t numberOfEntries = 0;

	// This will represent the Hash Table itself. Comprised of a "unique key" (assignmentName), a courseName, and a daysDue (if necessary due to collisions, it will store linked lists to hold the values that collided). Each index in this array represents a "bucket" that may contain one or more entries in each bucket.
	Homework* hashingTable[s_TABLE_SIZE];

	// A method to "hash" an assignment name, which will act as the "primary key" for the Hash Table. It will ideally create a unique index, based on a key it is supplied. For security measures this method should be marked as private.
	int hashFunction(std::string assignmentName) const;
};

#endif // !HASH_TABLE_H
