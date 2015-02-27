/*
 * =====================================================================================
 *
 *       Filename:  LinkedList.c
 *
 *    Description:  Solution to Linked List Problems - Stanford CS Library
 *
 *        Version:  1.0
 *        Created:  02/13/2015 10:54:39 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Chris Kuo (ck),
 *   Organization:
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Definiton of a node in the linked list
typedef struct node {
	int data;
	struct node* next;
} List;

// Function forward declaration
int Length(struct node* head);
struct node* BuildOneTwoThree();
void Push(struct node** headRef, int data);
int CompareList(struct node *listA, struct node *listB);
void RecursiveReverse(struct node** headRef);

// Utility functions
int Length(struct node *head)
{
	int n = 0;
	while (head != NULL) {
		n++;
		head = head->next;
	}
	return n;
}

struct node* MakeNode(int data)
{
	struct node* newNode = malloc(sizeof(struct node));

	newNode->data = data;
	newNode->next = NULL;

	return newNode;
}

struct node* BuildOneTwoThree()
{
	struct node* head = NULL;
	int i;

	// Using utility function Push()
	for (i = 3; i > 0; i--) {
		// Need to count backward since we are inserting before head
		Push(&head, i);
	}
	return head;

	// Not using utility function Push()
	struct node* tail;
	// Allocate and initialize nodes
	head = malloc(sizeof(struct node));
	head->data = 1;

	tail = head;
	for (i = 1; i < 3; i++) {
		tail->next = malloc(sizeof(struct node));
		tail->next->data = i + 1;
		tail = tail->next;
	}
	return head;
}

void Push(struct node** headRef, int newData)
{
	// Standard 3-step-link-in
	// 1. Create new node
	struct node* newHead = malloc(sizeof(struct node));
	newHead->data = newData;
	// 2. Set its .next to the current head
	newHead->next = *headRef;
	// 3. Change the head point to the new node
	*headRef = newHead;
}

int CompareList(struct node *listA, struct node *listB)
{
	// Compares two list.  The two list are equal iff 
	// 1. the list have same number of nodes
	// 2. the nodes at same index contain same data values.
	// Returns 1 if listA and listB are the same.
	// Returns 0 if listA and listB are different.
	int output = 1;
	while (listA != NULL || listB != NULL) {
		if (listA == NULL) {
			output = 0;
			break;
		}
		else if (listB == NULL) {
			output = 0;
			break;
		}
		output = (listA->data == listB->data) ? 1 : 0;
		listA = listA->next;
		listB = listB->next;
	}
	return output;
}

struct node* MakeList(int data[], int len)
{
	assert(data != NULL);

	int i;
	struct node* head = NULL;
	struct node* temp = NULL;
	for (i = 0; i < len; i++) {
		if (i == 0) {	// Special case for the head
			head = malloc(sizeof(struct node));
			head->data = data[i];
			head->next = NULL;
			temp = head;
		}
		else {
			temp->next = malloc(sizeof(struct node));
			temp = temp->next;
			temp->data = data[i];
			temp->next = NULL;
		}
	}
	return head;
}

int Count(struct node* head, int searchFor)
{
	int count = 0;
	while (head != NULL) {
		if (head->data == searchFor)    count++;
		head = head->next;
	}
	return count;
}

// Given a list and an index, return the data
// in the nth node of the list.  The nodes are numbered from 0.
// Assert fails if the index is invalud (outsde 0..length-1).
int GetNth(struct node* head, int index) {
	// Check for negative index
	assert(index >= 0);
	int i;
	for (i = 0; i < index; i++) {
		assert(head != NULL);
		head = head->next;
	}
	return head->data;
}

void PrintList(struct node* list)
{
	// Print result
	printf("{");

	struct node *current = list;

	while (current != NULL) {
		printf("%d ", current->data);
		current = current->next;
	}
	printf("}");
}

void PrintListWithSuffix(struct node *list, char *suffix)
{
	PrintList(list);
	printf("%s", suffix);
}
void DeleteList(struct node** list)
{
	struct node* next;
	while (*list != NULL) {
		next = (*list)->next;
		// printf("\tFreeing node (node->data == %d)\n", (*list)->data);
		free(*list);
		*list = next;
	}
}

int Pop(struct node** list)
{
	assert(*list != NULL);

	int data = (*list)->data;
	struct node* oldHead = *list;

	*list = oldHead->next;
	// printf("\tFreeing old head node (node->data == %d)\n", oldHead->data);
	free(oldHead);

	return data;
}

void InsertNth(struct node** head, int index, int data)
{
	assert(head != NULL);
	assert(index >= 0);

	int i;
	struct node* current = *head;
	struct node* temp;

	if (index == 0) { // Edge case, index == 0
		Push(head, data);
		return;
	}

	temp = malloc(sizeof(struct node));
	temp->data = data;

	// Traverse the list to (index-1)th node
	for (i = 0; i < index - 1; i++) {
		current = current->next;
		assert(current != NULL);
	}
	temp->next = current->next;
	current->next = temp;
}

void SortedInsert(struct node** headRef, struct node* newNode)
{
	if (newNode == NULL) {
		return;
	}
	if (*headRef == NULL) {
		*headRef = newNode;
		return;
	}
	struct node* previous;

	// Assume list is sorted in ascending/increasing order
	// Edge Case: insert at index == 0
	if (newNode->data < (*headRef)->data) {
		newNode->next = *headRef;
		*headRef = newNode;
		return;
	}
	else {
		// Insert newNode right after headRef
		previous = *headRef;
		newNode->next = previous->next;
		previous->next = newNode;
	}
	while (newNode->next != NULL && newNode->data > newNode->next->data) {
		// Not in corret location yet and there are more nodes.. keep swapping    
		previous->next = newNode->next;
		newNode->next = previous->next->next;
		previous->next->next = newNode;
		previous = previous->next;
	}
}

// Given a list, rearrange its nodes so they are sorted
// in ascending order.  Uses SortedInsert()
void InsertSort(struct node** headRef)
{
	// This algorithm breaks off the head node and SortedInsert()
	// the remaining nodes one by one
	struct node* current;
	struct node* next = (*headRef)->next;
	(*headRef)->next = NULL;

	while (next != NULL) {
		current = next;
		next = current->next;
		SortedInsert(headRef, current);
	}
}

// Appends list b onto end of list a, and sets list b to NULL
void Append(struct node** aRef, struct node** bRef)
{
	if (*aRef == NULL) {
		*aRef = *bRef;
		*bRef = NULL;
		return;
	}

	struct node* current = *aRef;
	while (current->next != NULL)   current = current->next;
	// Reached end of list a
	current->next = *bRef;
	*bRef = NULL;
}

/*
 Split the nodes of the given list into front and back halves,
 and return the two lists using the reference parameters.
 If the length is odd, the extra node should go in the front list.
 If the length is 0, the front and back list is set to NULL.
 If the length is 1, the back list is set to NULL.
 */
void FrontBackSplit(struct node* source, struct node** frontRef, struct node** backRef)
{
	struct node* tail = NULL;
	struct node* prev;  // previous points to the node previous to backRef

	// The default values
	*frontRef = source;
	*backRef = NULL;

	// Special cases
	if (source == NULL) {
		return;
	}
	if (Length(source) == 1) {
		return;
	}
	// There are two pointers.  The first point to index n (current), the second point to index 2*n
	// The middle is found when second pointer is NULL or second pointer->next is NULL
	prev = source;
	*backRef = source->next;
	tail = source->next;
	while (tail != NULL && tail->next != NULL) {
		prev = *backRef;
		*backRef = (*backRef)->next;	// Advance middle pointer
		tail = tail->next->next;	// Advance tail pointer by two step
	}
	prev->next = NULL;
}


/*
Remove duplicates from a sorted (increasing order)list.
*/
void RemoveDuplicates(struct node* head)
{
	assert(head != NULL);

	struct node* temp = NULL;

	// Compare data with next node.
	// If it is duplicate, remove next node and repeat.
	// If it is not duplicate, move to next node and repeat.
	while (head->next != NULL) {
		if (head->data == head->next->data) {
			// Found duplicate
			temp = head->next->next;	// Store the node after next
			free(head->next);			// Free next node
			head->next = temp;			// Link temp node as next node
		}
		else {
			// Not duplicate. Move to next node
			head = head->next;
		}
	}
}

/*
Take the node from the front of the source, and move it to
the front of the dest.
It is an error to call this with the source list empty.
*/
void MoveNode(struct node** destRef, struct node** sourceRef)
{
	assert(destRef != NULL && sourceRef != NULL);

	struct node* temp = *sourceRef;
	*sourceRef = (*sourceRef)->next;
	temp->next = *destRef;
	*destRef = temp;
}

/*
Given the source list, split its nodes into two shorter lists.
If we number the elements 0, 1, 2, ... then all the even elements (incl. 0)
should go in the first list, and all the odd elements in the second.
The elements in the new lists may be in any order.
*/
void AlternatingSplit(struct node* source,
struct node** aRef,
struct node** bRef)
{
	assert(aRef != NULL && bRef != NULL);

	struct node* a = *aRef;
	struct node* b = *bRef;
	int i = 0;

	while (source != NULL) {
		if (i == 0) {
			MoveNode(aRef, &source);
			i = 1;
		}
		else {
			MoveNode(bRef, &source);
			i = 0;
		}
	}
}


/*
Merge the nodes of the two lists into a single list taking a node
alternately from each list, and return the new list.
*/
struct node* ShuffleMerge(struct node* a, struct node* b)
{
	struct node* temp = NULL;
	int i = 0;

	while (a != NULL && b != NULL) {  // while still have nodes on both a and b
		if (i == 0) {
			i = 1;	// flip i
			MoveNode(&temp, &a);
		}
		else if (i == 1) {
			i = 0;	// flip i
			MoveNode(&temp, &b);
		}
	}
	// Reverse the list since it is in reverse
	RecursiveReverse(&temp);
	// Either a or b is NULL
	if (a == NULL) {
		Append(&temp, &b);
	}
	else if (b == NULL) {
		Append(&temp, &a);
	}
	return temp;
}

/*
Recursively reverses the given linked list by changing its .next
pointers and its head pointer in one pass of the list.
*/
void RecursiveReverse(struct node** headRef)
{
	assert(headRef != NULL);

	// To make sure we make only one pass, each recursion will print out
	// the call
	// printf("\tCalled RecursiveReverse with *headRef = %d, data = %d\n", *headRef, (*headRef)->data);
	struct node* curr = *headRef;
	struct node* next = (*headRef)->next;
	// Continue recursion if we have not reached the end yet
	if ((*headRef)->next != NULL) {
		// Haven't reached the end yet. Keep reversing next node
		*headRef = (*headRef)->next;
		RecursiveReverse(headRef);
		// Reverse current node
		next->next = curr;
		curr->next = NULL;
	}
	// We have reach the end.  Stop recursion
}


// ******************** Test Function ********************
void SectionBreak()
{
	printf("\n--------------------\n\n");
}

void BasicsCaller()
{
	printf("Running BasicCaller()\t");
	struct node *head;
	int len;

	head = BuildOneTwoThree();  // Start with {1, 2, 3}

	Push(&head, 13);            // Push 13 on the front, yielding {13, 1, 2, 3}
	// (The '&' is because head is passed as a
	// reference pointer.)

	Push(&(head->next), 42);    // Push 42 into the second position, yielding
	// {13, 42, 1, 2, 3}.  Demonstrate use of '&' on
	// the .next field of a node.
	// (See technique #2 below.)

	len = Length(head);         // Computes that the length is 5.

	// Print result
	printf("The length of list is %d\n", len);
	printf("The list contains {");

	struct node *current = head;

	while (current != NULL) {
		printf("%d ", current->data);
		current = current->next;
	}
	printf("}\n");

	SectionBreak();
}

void CountTest()
{
	printf("Running CountTest()\t");
	int got, want = 1;
	List *myList = BuildOneTwoThree();   // build {1, 2, 3}
	got = Count(myList, 2);

	// Ceck output
	if (got != want) {
		printf("FAIL: Got %d, Want %d\n", got, want);
	}
	else {
		printf("PASS\n");
	}
	DeleteList(&myList);
}

void GetNthTest()
{
	printf("Running GetNthTest()\t");
	int got, want = 3;

	List *myList = BuildOneTwoThree();  // Build {1, 2, 3}
	got = GetNth(myList, 2);   // Returns the value 3 
	// Ceck output
	if (got != want) {
		printf("FAIL: Got %d, Want %d\n", got, want);
	}
	else {
		printf("PASS\n");
	}
	DeleteList(&myList);
}

void DeleteListTest()
{
	printf("Running DeleteListTest()\t");
	int got, want = 0;

	List *myList = BuildOneTwoThree();  // Building {1, 2, 3}
	DeleteList(&myList);  // Deletes the three nodes and sets myList to NULL
	got = Length(myList);

	// Ceck output
	if (got != want) {
		printf("FAIL: Got %d, Want %d\n", got, want);
	}
	else {
		printf("PASS\n");
	}
}

void PopTest()
{
	printf("Running PopTest()\t");
	int got, want;

	List *myList = BuildOneTwoThree();  // Build {1, 2, 3}

	want = 1;
	got = Pop(&myList);     // Deletes "1" node and returns 1
	if (got != want) {
		printf("FAIL: Got %d, Want %d\n", got, want);
	}
	want = 2;
	got = Pop(&myList);     // Deletes "1" node and returns 2
	if (got != want) {
		printf("FAIL: Got %d, Want %d\n", got, want);
	}
	want = 3;
	got = Pop(&myList);     // Deletes "3" node and returns 3
	if (got != want) {
		printf("FAIL: Got %d, Want %d\n", got, want);
	}
	want = 0;
	got = Length(myList);   // The list is now empty, so len == 0
	if (got != want) {
		printf("FAIL: Got %d, Want %d\n", got, want);
	}

	// All test cases passed, so pass
	printf("PASS\n");
}

void InsertNthTest()
{
	printf("Running InsertNthTest()\t");
	int got, want;

	List *myList = NULL;    // Start with the empty list

	InsertNth(&myList, 0, 13);  // build {13}
	InsertNth(&myList, 1, 42);  // build {13, 42}
	InsertNth(&myList, 1, 5);   // build {13, 5, 42}

	want = 13;
	got = GetNth(myList, 0);
	if (got != want) {
		printf("FAIL: Got %d, Want %d\n", got, want);
	}
	want = 5;
	got = GetNth(myList, 1);
	if (got != want) {
		printf("FAIL: Got %d, Want %d\n", got, want);
	}
	want = 42;
	got = GetNth(myList, 2);
	if (got != want) {
		printf("FAIL: Got %d, Want %d\n", got, want);
	}
	DeleteList(&myList);    // Clean up after ourselves

	printf("PASS\n");
}

void SortedInsertTest()
{
	printf("Running SortedInsertTest()\t");
	int got, want;

	List *myList = NULL;    // Start with the empty list
	struct node* temp;

	Push(&myList, 42);  // build {42}
	Push(&myList, 13);  // build {13, 42}
	Push(&myList, 5);   // build {5, 13, 42}

	want = 20;
	temp = malloc(sizeof(struct node));
	temp->data = want;
	SortedInsert(&myList, temp);  // build {5, 13, 20, 42}
	got = GetNth(myList, 2);
	if (got != want) {
		printf("FAIL: Got %d, Want %d\n", got, want);
	}
	want = 50;
	temp = malloc(sizeof(struct node));
	temp->data = want;
	SortedInsert(&myList, temp);  // build {5, 13, 20, 42}
	got = GetNth(myList, 4);
	if (got != want) {
		printf("FAIL: Got %d, Want %d\n", got, want);
	}
	want = 1;
	temp = malloc(sizeof(struct node));
	temp->data = want;
	SortedInsert(&myList, temp);  // build {5, 13, 20, 42}
	got = GetNth(myList, 0);
	if (got != want) {
		printf("FAIL: Got %d, Want %d\n", got, want);
	}

	printf("PASS\n");
	DeleteList(&myList);
}

void InsertSortTest()
{
	printf("Running InsertSortTest()\t");

	int i;
	List *myList = NULL;

	Push(&myList, 42);
	Push(&myList, 5);
	Push(&myList, -2);
	Push(&myList, 50);
	Push(&myList, 13);

	InsertSort(&myList);

	for (i = 0; i < Length(myList) - 1; i++) {
		if (GetNth(myList, i) > GetNth(myList, i + 1)) {
			printf("FAIL: Got non-sorted list ");
			PrintList(myList);
			return;
		}
	}

	printf("PASS: Got sorted list ");
	PrintListWithSuffix(myList, "\n");
	DeleteList(&myList); // Clean up
}

#define LEN 8

void AppendTest()
{
	printf("Running AppendTest()\t");
	//const int LEN = 8;  // Must be greater than 3
	int values[LEN];
	int i = 0, want = 0, got = 0;

	for (i = 0; i < LEN; i++) {
		values[i] = rand() % 50 - 25;
	}

	List  *a = NULL, *b = NULL;
	for (i = 0; i < 3; i++) { // Build the list backward since Push() inserts data at index 0
		Push(&b, values[LEN - i - 1]);
	}
	for (i = Length(b); i < LEN; i++) {
		Push(&a, values[LEN - i - 1]);
	}

	Append(&a, &b);

	// Test Case: check length
	want = LEN;
	got = Length(a);
	if (want != got) {
		printf("FAIL: Incorrect length. Got %d, Want %d\n", got, want);
		return;
	}
	// Test Case: check data
	for (i = 0; i < LEN; i++) {
		want = values[i];
		got = GetNth(a, i);
		if (want != got) {
			printf("FAIL: Index = %d. Got %d, Want %d\n", i, got, want);
			return;
		}
	}
	// Test Case: check b is NULL
	if (b != NULL) {
		printf("FAIL: b is not set to NULL\n");
		return;
	}

	DeleteList(&a);
	printf("PASS\n");
}

void FrontBackSplitTest()
{
	printf("Running FrontBackSplitTest()\t");
	// The test cases include NULL list, 1-element list, 2-element list, 3-element list
	// 4-element list, 5-element list, and 6-element list

	// Null element list

	List* myList = NULL;
	List* front = NULL;
	List* back = NULL;
	printf("\n\tRunnning 0 element test.\n");
	printf("\t");
	PrintList(myList);
	FrontBackSplit(myList, &front, &back);

	printf(" -> ");
	PrintList(front);
	printf(" + ");
	PrintListWithSuffix(back, "\n");

	// 1 Element test
	DeleteList(&myList);    // Start with NULL
	Push(&myList, 42);  // build {42}

	printf("\tRunning 1 element test.\n");
	printf("\t");
	PrintList(myList);
	FrontBackSplit(myList, &front, &back);

	printf(" -> ");
	PrintList(front);
	printf(" + ");
	PrintListWithSuffix(back, "\n");


	// 2 Element test
	DeleteList(&myList);
	myList = BuildOneTwoThree();    // Start with {1, 2, 3}
	Pop(&myList);  // build {2, 3}
	printf("\tRunning 2 element test.\n");
	printf("\t");
	PrintList(myList);
	FrontBackSplit(myList, &front, &back);

	printf(" -> ");
	PrintList(front);
	printf(" + ");
	PrintListWithSuffix(back, "\n");

	// 3 Element test
	DeleteList(&myList);
	myList = BuildOneTwoThree();    // Start with {1, 2, 3}
	printf("\tRunning 3 element test.\n");
	printf("\t");
	PrintList(myList);
	FrontBackSplit(myList, &front, &back);

	printf(" -> ");
	PrintList(front);
	printf(" + ");
	PrintListWithSuffix(back, "\n");


	// 4 Element test
	DeleteList(&myList);
	myList = BuildOneTwoThree();    // Start with {1, 2, 3}
	Push(&myList, 42);  // build {42, 1, 2, 3}
	printf("\tRunning 4 element test.\n");
	printf("\t");
	PrintList(myList);
	FrontBackSplit(myList, &front, &back);

	printf(" -> ");
	PrintList(front);
	printf(" + ");
	PrintListWithSuffix(back, "\n");

	// 5 Element test
	DeleteList(&myList);
	myList = BuildOneTwoThree();    // Start with {1, 2, 3}
	Push(&myList, 42);  // build {42, 1, 2, 3}
	Push(&myList, 13);  // build {13, 42, 1, 2, 3}

	printf("\tRunning 5 element test.\n");
	printf("\t");
	PrintList(myList);
	FrontBackSplit(myList, &front, &back);

	printf(" -> ");
	PrintList(front);
	printf(" + ");
	PrintListWithSuffix(back, "\n");

	// 6 Element test
	DeleteList(&myList);
	myList = BuildOneTwoThree();    // Start with {1, 2, 3}
	Push(&myList, 42);  // build {42, 1, 2, 3}
	Push(&myList, 13);  // build {13, 42, 1, 2, 3}
	Push(&myList, 5);   // build {5, 13, 42, 1, 2, 3}

	printf("\tRunning 6 element test.\n");
	printf("\t");
	PrintList(myList);
	FrontBackSplit(myList, &front, &back);

	printf(" -> ");
	PrintList(front);
	printf(" + ");
	PrintListWithSuffix(back, "\n");
}

// Test 10.
void RemoveDuplicatesTest()
{
	printf("Running RemoveDuplicatesTest()");

	struct node* got = BuildOneTwoThree();    // Start with {1, 2, 3}

	struct node* want = BuildOneTwoThree();
	SortedInsert(&want, MakeNode(13));
	SortedInsert(&want, MakeNode(42));  // build {1, 2, 3, 13, 42

	Push(&got, 42);
	Push(&got, 13);
	Push(&got, 3);
	Push(&got, 42);
	Push(&got, 1);
	InsertSort(&got);	// build {1, 1, 2, 3, 3, 13, 42, 42}  (test duplicate at beginning, middle and end)
	RemoveDuplicates(got);

	if (CompareList(want, got)) {
		printf("\tPASS\n");
	}
	else {
		printf("\tFAIL\n");
		printf("\tWant ");
		PrintList(want);
		printf(", Got ");
		PrintListWithSuffix(got, "\n");
	}
}

void MoveNodeTest()
{
	printf("Running MoveNodeTest()\t");
	// Here are the expected result data
	int wantAData[] = { 1, 1, 2, 3 };
	int wantBData[] = { 2, 3 };

	struct node *wantA = MakeList(wantAData, 4);
	// PrintListWithSuffix(wantA, "\n");
	struct node *wantB = MakeList(wantBData, 2);
	// PrintListWithSuffix(wantB, "\n");

	// Here are the input test data
	struct node* a = BuildOneTwoThree(); // the list {1, 2, 3}
	struct node* b = BuildOneTwoThree();

	// Perform function
	MoveNode(&a, &b);
	// a == {1, 1, 2, 3}
	// b == {2, 3}

	// Verify results
	if (!CompareList(a, wantA)) {
		// a is not the same as wantA
		printf("FAIL: want ");
		PrintList(wantA);
		printf(", got ");
		PrintListWithSuffix(a, "\n");
	}
	else if (!CompareList(b, wantB)) {
		// b is not the same as wantB
		printf("FAIL: want ");
		PrintList(wantB);
		printf(", got ");
		PrintListWithSuffix(b, "\n");
	}
	else {
		printf("PASS\n");
	}
}

void AlternateSplitTest()
{
	printf("Running AlternateSplitTest()\t");

	int inputData[] = { 0, 1, 2, 3, 4, 5, 6 };
	int wantAData[] = { 6, 4, 2, 0 };	// List is reverse due to using MoveNode() to implement AlternateSplit()
	int wantBData[] = { 5, 3, 1 };
	struct node *input = MakeList(inputData, 7);
	struct node *wantA = MakeList(wantAData, 4);
	struct node *wantB = MakeList(wantBData, 3);

	struct node *a = NULL, *b = NULL;
	AlternatingSplit(input, &a, &b);

	if (!CompareList(wantA, a)) {
		printf("FAIL: want ");
		PrintList(wantA);
		printf(", got ");
		PrintListWithSuffix(a, "\n");
	}
	else if (!CompareList(wantB, b)) {
		printf("FAIL: want ");
		PrintList(wantB);
		printf(", got ");
		PrintListWithSuffix(b, "\n");
	}
	else {
		printf("PASS\n");
	}
}

void ShuffleMergeTest()
{
	printf("Running ShuffleMergeTest()\t");

	int inputDataA[] = { 0, 2, 4, 6, 7 };
	int inputDataB[] = { 1, 3, 5 };
	struct node* a = MakeList(inputDataA, 5);
	struct node* b = MakeList(inputDataB, 3);

	int wantData[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
	struct node* want = MakeList(wantData, 8);
	struct node* got = ShuffleMerge(a, b);

	// Verify results
	if (!CompareList(got, want)) {
		printf("FAIL: want ");
		PrintList(want);
		printf(", got ");
		PrintListWithSuffix(got, "\n");
	}
	else {
		printf("PASS: want ");
		PrintList(want);
		printf(", got ");
		PrintListWithSuffix(got, "\n");
	}

}

void RecursiveReverseTest()
{
	printf("Running RecursiveReverseTest()\t");

	int inputData[] = { 0, 1, 2, 3, 4, 5, 6 };
	struct node* got = MakeList(inputData, 7);

	int wantData[] = { 6, 5, 4, 3, 2, 1, 0 };
	struct node *want = MakeList(wantData, 7);

	RecursiveReverse(&got);

	if (!CompareList(want, got)) {
		printf("FAIL: want ");
		PrintList(want);
		printf(", got ");
		PrintListWithSuffix(got, "\n");
	}
	else {
		printf("PASS\n");
	}
}

// Main
int main(int argc, char *argv[])
{
	// BasicsCaller();
	CountTest();	// Problem 1
	GetNthTest();	// Problem 2
	DeleteListTest();	// Problem 3
	PopTest();	// Problem 4
	InsertNthTest();	// Problem 5
	SortedInsertTest();	// Problem 6
	InsertSortTest();	// Problem 7
	AppendTest();	// Problem 8
	FrontBackSplitTest();	// Problem 9
	RemoveDuplicatesTest();	// Problem 10
	MoveNodeTest();	// Problem 11
	AlternateSplitTest();	// Problem 12
	ShuffleMergeTest();		// Problem 13
	RecursiveReverseTest();	// Problem 18

	return 0;
}
