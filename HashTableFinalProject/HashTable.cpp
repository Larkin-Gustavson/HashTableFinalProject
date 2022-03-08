#include "HashTable.h" // include all the struct (Homework) and class (HashTable)information here
using namespace std;

/*
	Author: Larkin Gustavson
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
					   displayAllInProgressAssignments, searchByCourse, default constructor,
					   parameterized constructor, destructor, and remove methods.

	Collaborated with Chadayne Westney and Christian Gibbone.

	Credit go to Dr. Zhao, for helping us with the constructors (for Homework and Hash Table),
	destructor, add method, and displayHomework method.

	Another source of credit goes to Paul Programming for the implementation of the Hashing Function,
	I (Larkin) got some minimal inspiration for the searchByDueDateAndAssignment and searchByCourseNameAndAssignment
	methods. His YouTube playlist can be found here:
	https://www.youtube.com/watch?v=MfhjkfocRR0&list=PLTxllHdfUq4f7-uHOpxXnBUbsuLiI9pmb

*/

// struct (Homework) member methods (default constructor, parametrized constructor,
// clearLinkedList, and displayHomework) defined here

// O(1) for this method (constructor)

Homework::Homework() // default constructor
	: assignmentName{ "" }, courseName{ "" }, daysDue{ -1 }, next{ nullptr }
{
}

// O(1) for this method (constructor)

Homework::Homework(const string assignmentName, const string courseName, const int daysDue) // parameterized constructor
	: assignmentName{ assignmentName }, courseName{ courseName }, daysDue{ daysDue }, next{ nullptr } // using the constructor initializer list to speed up program speed and the {} is used for narrowing, it will give a compiler warning if narrowing occurs (conversion of larger data type to a smaller data type)
{
}

// O(1) for this method

void Homework::displayHomework() const // method to display the contents for each "homework item"
{
	cout << "Assignment Name = " << assignmentName << ", Course name = " << courseName << ", Day(s) due = " << daysDue << endl;
}

// tail recursion used here (Part I). Implemented as a "helper" method for another method later (remove)

Homework* Homework::clearLinkedList(Homework* head) // a method to delete the elements ONLY in the linked lists, not the buckets themselves
{
	Homework* currentIndex; // used to traverse the linked list

	if (head == nullptr) // base case
	{
		return head; // exits recursion
	}

	// if it is not at the base case, do the following
	currentIndex = head->next; // moves to the next "node" in the linked list

	delete head; // delete the previous node in the linked list

	return clearLinkedList(currentIndex); // recursive call, using the new "shortened" list
}

/*
* HashTable class member methods(hashFunction, add, printTable, remove, searchByDueDate,
* searchByCourse, default constructor, destructor, etc.) defined here
*
*/

// O(n) for this method

int HashTable::hashFunction(string assignmentName) const // a method used to create a unique hash value that will distinguish each "row" or record from one another
{
	int hash = 0; // initialize the hash value to be 0

	int index; // used to store the index of the array or Hash Table

	for (int i = 0; i < assignmentName.length(); i++) // for as long as the string is
	{
		hash = hash + (int)assignmentName[i] * 26; // get each character in the string, convert it to ASCII, then multiply it by 26 (to try to reduce the likelihood of collisions), and then add the previously calculated hash value
	}

	index = hash % s_TABLE_SIZE; // get the previously calculated hash value and mod it by the table size

	return abs(index); // return the index (used to specify the spot in the array that the information will hold), we don't want negative indexes so we want to take the absolute value of the index (to guarantee a positive generated index)
}

// O(n) for this method

void HashTable::remove(const string assignmentName) // a method to remove an element from the Hash Table (most of this method implementation was used from Paul Programming).
{
	if (isEmpty()) // if the Hash Table is empty report this to the user
	{
		cout << "Error: The Hash Table is empty" << endl;

		return;
	}

	if (!keyExists(assignmentName)) // if the key does not exist in the Hash Table, report this to the user
	{
		cout << "Error: The key does not exist in the Hash Table" << endl;

		return;
	}

	int index = hashFunction(assignmentName); // get the index (or bucket location) of the where the assignment could be

	Homework* deletedNode; // this will represent the node to be deleted

	Homework* forwardPtr; // this will represent the node ahead of the to be deleted node

	Homework* trailingPtr; // this will represent the node behind the to be deleted node

	if (hashingTable[index]->assignmentName == assignmentName && hashingTable[index]->next == nullptr) // if the element contains the desired assignment name and is the only element in the bucket do the following
	{
		clearBucket(assignmentName);

		return;
	}

	else if (hashingTable[index]->assignmentName == assignmentName) // if the desired assignment name is found in the first item in the bucket, however there are more elements in the bucket, do the following
	{
		deletedNode = hashingTable[index]; // set the to be deleted node equal to the index where the assignment name is found
		hashingTable[index] = hashingTable[index]->next; // move to the second position in the bucket

		delete deletedNode; // delete the first node in the bucket

		numberOfEntries--; // since we have removed an enemy, decrease the total number of entries in the Hash Table

		cout << assignmentName << " was removed from the Hash Table" << endl;

		return;
	}

	// if the bucket contains multiple items in the bucket, however the first item in the bucket does not contain the desired assignment name do the following
	forwardPtr = hashingTable[index]->next; // move to the second position in the bucket

	trailingPtr = hashingTable[index]; // make this point to the first position in the bucket

	while (forwardPtr->assignmentName != assignmentName) // while we haven't found the correct assignment name, keep moving through the bucket (we know that the key does exist in the Hash Table because the keyExists method had to have returned true in order to hit this block of code)
	{
		trailingPtr = forwardPtr; // move the previous pointer up one element to whatever the forward pointer is pointing to

		forwardPtr = forwardPtr->next; // move the forward pointer one spot up in the bucket
	}

	if (forwardPtr->assignmentName == assignmentName) // if we have found the correct assignment name in the bucket, do the following
	{
		deletedNode = forwardPtr; // since the assignment name was found, set the to be deleted node to whatever the forward pointer is pointing to

		forwardPtr = forwardPtr->next; // move the forward pointer one element up in the bucket
		trailingPtr->next = forwardPtr; // making the trailing pointer over the to be deleted node and to whatever the forward pointer is pointing to

		delete deletedNode; // delete the node

		cout << assignmentName << " was removed from the Hash Table" << endl;

		numberOfEntries--; // since we have deleted an element from the Hash Table, decrease the total number of entries in the Hash Table

		return;
	}
}

// O(n) for this method

bool HashTable::keyExists(string assignmentName) const // a method to see if a key exists in the Hash Table.
{
	int index = hashFunction(assignmentName); // get the position in the Hash Table

	Homework* currentIndex = hashingTable[index]; // set the current position to be the

	if (isEmpty()) // if it is empty return that the key does not exist (false)
	{
		return false;
	}

	while (currentIndex != nullptr) // while we aren't at the end of the list
	{
		if (currentIndex->assignmentName == assignmentName) // if the assignment name stored in the current node is the same as the passed in, then the key does exist (return true)
		{
			return true;
		}

		currentIndex = currentIndex->next; // move to the next position in the bucket
	}

	return false; // if we hit this point, we know that the key does not exist in the bucket
}

// O(n) for this method

void HashTable::printItemsInIndex(const size_t index)
{
	if (isEmpty()) // if the Hash Table is empty, report to the user the following message
	{
		cout << "The Hash Table is empty" << endl;

		return;
	}

	Homework* currentIndex = hashingTable[index]; // set the position in the array of buckets to be whatever was passed in by the user

	if (index < 0 || index > s_TABLE_SIZE) // if the index passed in is out of bounds of the array of buckets, report this error to the user
	{
		cout << index << " passed in is not with in the bounds of the Hash Table" << endl;

		return;
	}

	// if the index passed in by the user is within the bounds, do the following
	while (currentIndex != nullptr) // while we are not at the end of the bucket
	{
		currentIndex->displayHomework(); // show to the user the contents of the entries in each entry in the bucket

		currentIndex = currentIndex->next; // move to the next item in the bucket
	}
}

// O(n) for this method

int HashTable::numberOfItemsInIndex(const size_t index) // method to count how many items are in a specific bucket (index of the array)
{
	int itemCounter = 0; // used to count the number of entries in a specific position

	if (isEmpty()) // if the Hash table is empty print 0;
	{
		return itemCounter;
	}

	Homework* currentIndex = hashingTable[index]; // set the position in the array of buckets to be whatever was passed in by the user

	if (index < 0 || index > s_TABLE_SIZE) // if the index passed in is greater then the size of the array of buckets, report this error to the user
	{
		cout << index << " passed in is not with in the bounds of the Hash Table" << endl;

		return itemCounter;
	}

	while (currentIndex != nullptr) // while we are not at the end of the bucket
	{
		itemCounter++; // increment the counter

		currentIndex = currentIndex->next; // move to the next item in the bucket
	}

	return itemCounter; // return the number of items in this particular bucket
}

// O(1) for this method

size_t HashTable::getCapacity() const { return capacity; } // return the current capacity. A method to retrieve the current capacity in the Hash Table (how many empty buckets there are in the Hash Table)

// O(n) for this method

size_t HashTable::sizeInBucket(const string assignmentName) // a method to count how many entries are made in the Hash Table at a certain bucket position (a primary key of an assignment name)
{
	if (isEmpty()) // if the Hash Table is empty, report that the Hash Table is empty and that the number of entries in this particular bucket is zero
	{
		cout << "Error: The Hash Table is empty" << endl;

		return 0;
	}

	int index = hashFunction(assignmentName); // get the index of the buck to "look at"

	Homework* currentIndex = hashingTable[index]; // set the current position to be that index (the first item in the bucket at this position)

	size_t numberOfEntries = 0; // this will represent the amount of entries found in this current bucket

	if (!keyExists(assignmentName)) // if the key doesn't exist at the position, return that there are zero entries in the Hash Table
	{
		return numberOfEntries;
	}

	while (currentIndex != nullptr) // while we are not at the end of the Hash Table
	{
		numberOfEntries++; // increase the number of entries found in the bucket by one

		currentIndex = currentIndex->next; // move to the next position in the Hash Table
	}

	return numberOfEntries;
}

// O(1) for this method

bool HashTable::isEmpty() const // a method to see if a Hash Table is empty or not
{
	if (numberOfEntries == 0) // if there are no entries made in the Hash Table, we know that the entire Hash Table is empty
	{
		return true;
	}

	return false; // if the number of entries is anything greater then 0, then we know that there is at least one entry in the Hash Table, and is consequently not empty
}

// O(1) for this method

size_t HashTable::getNumberOfEntries() const // method to retrieve the total number of entries made in the Hash Table
{
	if (isEmpty()) // if the Hash Table is empty, report that the bucket is empty and that the number of entries in the Hash Table is zero
	{
		cout << "Error: The Hash Table is empty" << endl;

		return 0;
	}

	return numberOfEntries; // return the number of entries stored in the private member variable used to represent the total number of entries in the Hash Table
}

// O(n) for this method

void HashTable::add(const string assignmentName, const string courseName, const int daysDue) // method to add information into the Hash Table
{
	ofstream outFile; // creating the ability to write to a file

	outFile.open("Assignments.txt", ios::app); // opening a file called "Assignments.txt, then if the same file is opened again, append the new contents to the original file

	int index = hashFunction(assignmentName); // get the index of the assignmentName (which will be used as the key)

	Homework* currentIndex = hashingTable[index]; // set the current position to be whatever the calculated position is in the array. AKA the first element in that specific bucket (as indicated by the variable index)

	if (capacity > 0) // if there is space in the underlying array to add an element, do the following
	{
		if (currentIndex == nullptr) // if there isn't an element in the linked list, add the information passed in by the user to the top of the bucket
		{
			hashingTable[index] = new Homework(assignmentName, courseName, daysDue); // create a new HW entry with the passed in information from the user

			outFile << assignmentName << " " << courseName << " " << daysDue << endl; // write to the file, the following information

			numberOfEntries++; // since a new entry has been made, increment the number of entries in the Hash Table by one

			capacity--; // since an element has been added, the capacity of the Hash Table must be decreased

			outFile.close(); // close the file, to prevent errors
		}

		else // else do the following
		{
			Homework* newNode = new Homework(assignmentName, courseName, daysDue); // create a new HW entry with the passed in information from the user

			outFile << endl;

			while (currentIndex->next != nullptr) // go until we reach the end of the list (meaning that we reach the end of the bucket)
			{
				currentIndex = currentIndex->next; // move the next position in the linked list
			}

			currentIndex->next = newNode; // link the current node, to the new node (which represents the last node in the list)

			numberOfEntries++; // since a new entry has been made, increment the number of entries in the Hash Table by one

			outFile << assignmentName << " " << courseName << " " << daysDue << endl; // write to the file the following information

			outFile.close(); // close the file, to prevent errors
		}
	}

	else // if the capacity reaches the maximum allowed of if the user tries to add an element into the array that cannot be chained or that the capacity is reached, display the following message
	{
		cout << "Error: capacity of " << s_TABLE_SIZE << " has already been reached" << endl;
	}
}

// O(n^2) for this method

void HashTable::printTable() const // method to print the contents of the Hash Table
{
	if (isEmpty()) // if the Hash Table is empty, report that the Hash Table is empty
	{
		cout << "Error: The Hash Table is empty" << endl;

		return;
	}

	for (int i = 0; i < s_TABLE_SIZE; i++) // go until the max table size (50)
	{
		Homework* currentIndex = hashingTable[i]; // current represents the head

		while (currentIndex != nullptr) // while there are elements in the linked list
		{
			cout << "Inside bucket " << i << " "; // display the bucket number that hold the information from the Hash Table
			currentIndex->displayHomework(); // print whatever is inside the current homework object (assignmentName, courseName, daysDue)

			if (currentIndex->next != nullptr) // check if there is another node in linked list at the same index (collision)
			{
				currentIndex = currentIndex->next; // moves to next node in the linked list

				cout << "\t";
			}

			else // if the next "node" is null, break out of the while loop
			{
				break;
			}
		}
	}
}

// O(n), this is also assuming that there are no collisions or that there are not duplicate "primary keys" in the Hash Table
// such as with the test data it only prints under the assignment name "Test 0", Course Name = Chemistry, and Day(s) Due = 0

void HashTable::searchByAssignmentName(string assignmentName) const // method to search the Hash Table based on a specific "primary key"
{
	if (isEmpty()) // if the Hash Table is empty, report it to the user
	{
		cout << "Error: The Hash Table is empty" << endl;

		return;
	}

	int index = hashFunction(assignmentName); // get the index of the buck to "look at"

	Homework* currentIndex = hashingTable[index]; // set the current position to be that index (the first item in the bucket at this position)

	bool foundInformation = false; // variable to flag if the information was found in the Hash Table

	while (currentIndex != nullptr) // while we are not at the end of the Hash Table
	{
		if (currentIndex->assignmentName == assignmentName) // if the assignment name being looked at is the same provided by the user do the following
		{
			foundInformation = true; // mark the information as being found

			cout << "Assignment Name = " << currentIndex->assignmentName << endl;
			cout << "Course Name = " << currentIndex->courseName << endl;
			cout << "Day(s) Due = " << currentIndex->daysDue << endl;
		}

		currentIndex = currentIndex->next; // move to the next position in the Hash Table
	}

	if (foundInformation == false) // if the information was never marked as being found, display the following method
	{
		cout << "The assignment " << assignmentName << " was not found in the Hash Table" << endl;
	}
}

// O(n^2) - assuming that in progress means assignments that are due today or tomorrow (0 = today or 1 = tomorrow)

void HashTable::assignmentsInProgress() const
{
	if (isEmpty()) // if the Hash Table is empty, report it to the user
	{
		cout << "Error: The Hash Table is empty" << endl;

		return;
	}

	cout << "The current assignment(s) due in 0 or 1 day(s) are the following:" << endl;

	for (int i = 0; i < s_TABLE_SIZE; i++)
	{
		Homework* currentIndex = hashingTable[i]; // current represents the head of each linked list stored at each index of the Hash Table

		while (currentIndex != nullptr)
		{
			if (currentIndex->daysDue == 0 || currentIndex->daysDue == 1) // since this method is working under the assumption that the in progress assignments mean daysDue is either zero or one, then we need to filter all of the daysDue that are not 0 or 1
			{
				currentIndex->displayHomework(); // display the homework, that matches the above criteria
			}

			if (currentIndex->next != nullptr) // check if there is another node in linked list
			{
				currentIndex = currentIndex->next; // moves to next node in link list
			}
			else // if the next "node" is null, break out of the while loop
			{
				break;
			}
		}
	}
}

// tail recursion used here (Part II), using the Tail Recursion Method from Part I

void HashTable::clearBucket(const string assignmentName) // a method to remove the assignment name and its associated information from the Hash Table
{
	if (isEmpty()) // if the Hash Table is empty, report it to the user
	{
		cout << "Error: The Hash Table is empty" << endl;

		return;
	}

	int index = hashFunction(assignmentName); // get the index to where this information could be stored

	if (hashingTable[index] == nullptr) // if the generated hash value is null, display this error message
	{
		cout << "The item cannot be deleted, it is not present in the Hash Table" << endl;
	}

	else // otherwise, call the clearLinkedList method on the generated index (based on the assignmentName) from the user
	{
		size_t numberOfEntriesInBucket = sizeInBucket(assignmentName); // getting to total number of entries in the bucket that we are about to delete

		numberOfEntries = numberOfEntries - numberOfEntriesInBucket; // subtract the to be deleted number of entries in the bucket from the original amount of entries in the Hash Table, to get the actual number of buckets after the deleted buckets are moved

		capacity++;

		hashingTable[index]->clearLinkedList(hashingTable[index]); // the desired location to be deleted, is the index calculated from the user (assignmentName)
		hashingTable[index] = nullptr; // this was used, so that the deleted memory could be used at a later time for another hash value

		cout << assignmentName << " was removed from the Hash Table" << endl;
	}
}

// O(n^2) for this method

void HashTable::searchByDueDate(int dueDate) const // a method to search the Hash Table for all information that is associated with a due date
{
	if (isEmpty()) // if the Hash Table is empty, report it to the user
	{
		cout << "Error: The Hash Table is empty" << endl;

		return;
	}

	cout << "The Following Assignments are due: " << endl;

	for (int i = 0; i < s_TABLE_SIZE; i++) // look through the entire Hash Table
	{
		Homework* currentIndex = this->hashingTable[i]; // set the current node to be the beginning of the Hash Table

		while (currentIndex != nullptr) // while we are not at the end of the Hash Table (also look at the linked lists to see if there were any colliding elements)
		{
			if (currentIndex->daysDue == dueDate) // if the due date stored in the current node, is the same as the one entered by the user
			{
				currentIndex->displayHomework(); // display the information that is in the current visited node
			}

			currentIndex = currentIndex->next; // if there isn't a match, move to the next node
		}
	}
}

// O(n) for this method

void HashTable::searchByDueDateAndAssignment(string assignmentName, int daysDue) const // a method to search the Hash Table for all information that is associated with an assignment and a specific due date, based on a primary key (assignmentName)
{
	if (isEmpty()) // if the Hash Table is empty, report it to the user
	{
		cout << "Error: The Hash Table is empty" << endl;

		return;
	}

	cout << "The Following information about " << assignmentName << " and with the due date of " << daysDue << " was found in the Hash Table:" << endl;

	int index = hashFunction(assignmentName); // getting the index for which the assignmentName should be found in

	Homework* currentIndex = hashingTable[index]; // setting the current "node" to be the size of the calculated Hash value of the key (assignmentName)

	bool foundInformation = false; // used to signify if the information, based on the key (assignmentName) was found

	while (currentIndex != nullptr) // while we are not at the end of the Hash Table (including the linked list portions)
	{
		if (currentIndex->assignmentName == assignmentName && currentIndex->daysDue == daysDue) // if the assignment name in the current node and the courseName found is the same as the assignmentName and the courseName provided by the user, do the following
		{
			cout << "Assignment Name = " << currentIndex->assignmentName << endl; // display the assignmentName
			cout << "Course Name = " << currentIndex->courseName << endl; // display the courseName
			cout << "Day(s) Due = " << currentIndex->daysDue << endl; // display the daysDue

			foundInformation = true; // since the information was found, mark it is as true
		}

		currentIndex = currentIndex->next; // if the information wasn't found, move to the next "node"
	}

	if (foundInformation == false) // if the information was never found, display this error message
	{
		cout << "The information for " << assignmentName << " and " << daysDue << " was not found in the Hash Table" << endl;
	}
}

// O(n^2) for this method

void HashTable::searchByCourseName(string courseName) const // a method to search the Hash Table by the name of the course and retrieve its associated information
{
	if (isEmpty()) // if the Hash Table is empty, report it to the user
	{
		cout << "Error: The Hash Table is empty" << endl;

		return;
	}

	cout << "\n\n" << "The Following information about " << courseName << " was found:" << endl;

	for (int i = 0; i < s_TABLE_SIZE; i++) // look through the entire Hash Table
	{
		Homework* currentIndex = this->hashingTable[i]; // set the current node to be the beginning of the Hash Table

		while (currentIndex != nullptr) // while we are not at the end of the Hash Table (also look at the linked lists to see if there were any colliding elements)
		{
			if (currentIndex->courseName == courseName) // if the course name stored in the current node, is the same as the one entered by the user
			{
				currentIndex->displayHomework(); // display the information that is in the current visited node
			}

			currentIndex = currentIndex->next; // if there isn't a match, move to the next node
		}
	}
}

// O(n) for this method

void HashTable::searchByCourseNameAndAssignment(string assignmentName, string courseName) const // a method to search the Hash Table by the name of the course and a unique primary key (assignmentName) and retrieve its associated information
{
	if (isEmpty()) // if the Hash Table is empty, report it to the user
	{
		cout << "Error: The Hash Table is empty" << endl;

		return;
	}

	cout << "The Following information about " << assignmentName << " and " << courseName << " was found in the Hash Table:" << endl;

	int index = hashFunction(assignmentName); // getting the index for which the assignmentName should be found in

	Homework* currentIndex = hashingTable[index]; // setting the current "node" to be the size of the calculated Hash value of the key (assignmentName)

	bool foundInformation = false; // used to signify if the information, based on the key (assignmentName) was found

	while (currentIndex != nullptr) // while we are not at the end of the Hash Table (including the linked list portions)
	{
		if (currentIndex->assignmentName == assignmentName && currentIndex->courseName == courseName) // if the assignment name in the current node and the courseName found is the same as the assignmentName and the courseName provided by the user, do the following
		{
			cout << "Assignment Name = " << currentIndex->assignmentName << endl; // display the assignmentName
			cout << "Course Name = " << currentIndex->courseName << endl; // display the courseName
			cout << "Day(s) due = " << currentIndex->daysDue << endl; // display the daysDue

			foundInformation = true; // since the information was found, mark it is as true
		}

		currentIndex = currentIndex->next; // if the information wasn't found, move to the next "node"
	}

	if (foundInformation == false) // if the information was never found in the Hash Table, display this error message
	{
		cout << "The information for " << assignmentName << " and " << courseName << " was not found in the Hash Table" << endl;
	}
}

// O(n) for this method

void HashTable::updateCourseName(const string assignmentName, const string oldCourseName, const string newCourseName) // method to update the information in a unique row (assignmentName), it will update the courseName
{
	if (isEmpty()) // if the Hash Table is empty, report it to the user
	{
		cout << "Error: The Hash Table is empty" << endl;

		return;
	}

	int index = hashFunction(assignmentName); // get the position of the Hash Table that will contain the assignmentName (as the key) and store the information

	if (hashingTable[index]->assignmentName == assignmentName && hashingTable[index]->courseName == oldCourseName) // if the index stored in the Hash Table contains the same assignment name as the one passed in by the user and the course name stored in the same index is the same as the new course name passed in by the user, do the following
	{
		hashingTable[index]->courseName = newCourseName; // set the course name stored in the index to be the new course name passed in by the user
	}

	else if (hashingTable[index]->assignmentName != assignmentName) // this condition gets hit if there was a collision and the first element in the bucket in the Hash Table is not the entered in assignment name
	{
		Homework* currentIndex = hashingTable[index]; // setting the current "node" to be the size of the calculated Hash value of the key (assignmentName)

		while (currentIndex != nullptr) // while we are not at the end of the bucket (or list), keep looping
		{
			if (currentIndex->assignmentName == assignmentName && currentIndex->courseName == oldCourseName) // if the assignment name and the old day(s) due in the current node are the same as the old course name, do the following
			{
				currentIndex->courseName = newCourseName; // change the old course name to be the new course name
			}

			currentIndex = currentIndex->next; // if the current node does not contain the desired information, move to the next node
		}
	}

	else // if the assignment or course name in the index, doesn't match the name passed in by the user then display the following error message
	{
		cout << "No assignment or course name exists for " << assignmentName << " or " << oldCourseName << " in the Hash Table " << endl;
	}
}

// O(n) for this method

void HashTable::updateDaysDue(const string assignmentName, const int oldDaysDue, const int newDaysDue) // a method to update the daysDue value, with a given primary key (assignmentName) and a desired updated daysDue value
{
	if (isEmpty()) // if the Hash Table is empty, report it to the user
	{
		cout << "Error: The Hash Table is empty" << endl;

		return;
	}

	int index = hashFunction(assignmentName); // generate a position of the Hash Table that could contain the primary key (assignmentName) and the daysDue value

	if (hashingTable[index]->assignmentName == assignmentName && hashingTable[index]->daysDue == oldDaysDue) // if the assignmentName is the same (in the bucket) as the one passed in by the user and the daysDue value stored in the bucket, then do the following
	{
		hashingTable[index]->daysDue = newDaysDue; // set the daysDue to be assigned to the newDaysDue
	}

	else if (hashingTable[index]->assignmentName != assignmentName) // this condition gets hit if there was a collision and the first element in the bucket in the Hash Table is not the entered in assignment name
	{
		Homework* currentIndex = hashingTable[index]; // setting the current "node" to be the size of the calculated Hash value of the key (assignmentName)

		while (currentIndex != nullptr) // while we are not at the end of the bucket (or list), keep looping
		{
			if (currentIndex->assignmentName == assignmentName && currentIndex->daysDue == oldDaysDue) // if we find the node that has the correct assignment name and day(s) due, do the following
			{
				currentIndex->daysDue = newDaysDue; // changing the old days due that was in the node to be the entered in days due
			}

			currentIndex = currentIndex->next; // move to the next node in the linked list if the current node doesn't contain the desired information
		}
	}

	else // if the assignment name or daysDue value passed in by the user, doesn't match what is in the Hash Table, display the following error message
	{
		cout << "No assignment or days due value exists for " << assignmentName << " or " << oldDaysDue << " day(s) due in the Hash Table" << endl;
	}
}

// O(n) for this method

HashTable::HashTable() // default constructor
{
	for (int i = 0; i < s_TABLE_SIZE; i++)
	{
		hashingTable[i] = nullptr; // initialize each position in the array (the Hash Table) to be null
	}
}

// O(n^2) for this method

HashTable::~HashTable() // destructor, free up the memory used that is no longer needed
{
	for (int i = 0; i < s_TABLE_SIZE; i++)
	{
		for (int j = 0; i < s_TABLE_SIZE; i++)
		{
			delete hashingTable[i];
		}
	}
}