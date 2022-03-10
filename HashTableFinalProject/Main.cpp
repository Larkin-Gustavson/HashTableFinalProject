#include "HashTable.h" // include all the information about the Homework struct and the HashTable class
using namespace std;

/*
	Author: Larkin Gustavson
	Date of Creation: 12/3/2020
	Program's Purpose: To implement a student assignment tracking
					   system using a Hash Table.It will implement
					   several important methods such as within
					   the struct (Homework) Homework* clearLinkedList,
					   default constructor, parameterized constructor,
					   and void displayHomework. Within the class
					   (HashTable)the key methods implemented are
					   hashFunction, add, printTable, searchByDueDate,
					   searchByDueDateAndAssignment, searchByCourseNameAndAssignment,
					   displayAllInProgressAssignments, searchByCourse, default constructor,
					   parameterized constructor, destructor, and remove methods.

	Collaborated with Chadayne Westney and Christian Gibbone.

	Credit goes to Dr. Zhao for the destructor implementation.

	Another source of credit goes to Paul Programming for the implementation of the Hashing Function,
	I (Larkin) got some minimal inspiration for the searchByDueDateAndAssignment and searchByCourseNameAndAssignment
	methods. His YouTube playlist can be found here:
	https://www.youtube.com/watch?v=MfhjkfocRR0&list=PLTxllHdfUq4f7-uHOpxXnBUbsuLiI9pmb

*/

int main()
{
	HashTable hashTable; // instantiating an object of class HashTable type, called hashTable

	// testing the getNumberOfEmptyBuckets method

	cout << "Initial Capacity = " << hashTable.getNumberOfEmptyBuckets() << endl;

	/*
		Intentionally causing a collision (two or more elements produce the same index,
		meaning that if mishandled there will be a loss of data)
		adding the following information to the Hash Table.
	*/

	hashTable.add("Test 0", "Math", 0);
	hashTable.add("Test 0", "Chemistry", 0);

	// adding the following information to the Hash Table

	hashTable.add("Test 1", "Science", 1);
	hashTable.add("Test 2", "Liberal arts", 2);
	hashTable.add("Test 3", "Math", 3);
	hashTable.add("Test 4", "Science", 4);
	hashTable.add("Test 5", "Math", 5);
	hashTable.add("Test 6", "C++", 6);
	hashTable.add("Test 7", "Engineering", 7);
	hashTable.add("Test 8", "Art", 8);
	hashTable.add("Test 9", "Farming", 9);
	hashTable.add("Test 10", "Social Studies", 10);
	hashTable.add("Test 47", "Physics", 0);
	hashTable.add("Test 1", "Math", 1);
	hashTable.add("Test 1", "English", 1);

	// printing the table

	hashTable.printTable();

	cout << endl;
	cout << endl;
	cout << endl;

	// testing the getNumberOfEmptyBuckets method

	cout << "Capacity before removal = " << hashTable.getNumberOfEmptyBuckets() << endl;

	// testing the getNumberOfEntries method

	cout << "Number of entries before removal = " << hashTable.getNumberOfEntries() << endl;

	cout << endl;
	cout << endl;
	cout << endl;

	// testing the remove method

	hashTable.remove("Test 100");

	cout << "Number of empty buckets after removing a element(s) " << hashTable.getNumberOfEmptyBuckets() << endl;

	cout << "Number of entries in the Table after removing a element(s) " << hashTable.getNumberOfEntries() << endl;

	hashTable.printTable();

	cout << "Number of empty buckets after adding elements " << hashTable.getNumberOfEmptyBuckets() << endl;

	// testing the clearBucket method

	hashTable.clearBucket("Test 47");

	// print the updated table after removal of the element with the assignment name "Test 47"

	hashTable.printTable();

	// testing the getCapacity method after a deleting an element

	cout << "Number of empty buckets after removing a element " << hashTable.getNumberOfEmptyBuckets() << endl;

	// searching by the "primary key" (the assignment name)

	hashTable.searchByAssignmentName("Test 1");

	cout << endl;

	// getting the total number of entries in the Hash Table

	cout << "The number of Key Value pairs in the Hash Table = " << hashTable.getNumberOfEntries() << endl;

	// seeing if the Hash Table is empty

	if (hashTable.isEmpty())
	{
		cout << "The Hash Table is empty" << endl;
	}

	else
	{
		cout << "The Hash Table is NOT empty" << endl;
	}

	// testing the printItemsInIndex method

	hashTable.printItemsInIndex(22);

	// testing the sizeInBucket method

	cout << hashTable.sizeInBucket("0") << endl;

	// testing the keyExists method

	if (hashTable.keyExists("Test 47"))
	{
		cout << "The key exists" << endl;
	}

	else
	{
		cout << "The key does not exist" << endl;
	}

	return 0;
}