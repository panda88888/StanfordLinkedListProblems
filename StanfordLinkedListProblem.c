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
#include <stdarg.h>

// Definiton of a node in the linked list
typedef struct node {
	int data;
	struct node* next;
} List;

// Utility function forward declaration
// Get information about list(s)
int Length(struct node* head);
int CompareList(struct node* a, struct node* b);
int Count(struct node* head, int searchFor);
// Create lists
struct node* MakeNode(int data);
struct node* MakeList(int data[], int len);
struct node* BuildOneTwoThree();
struct node* BuildList(int len, ...);
// Working with lists
void Push(struct node** headRef, int newData);
void RecursiveReverse(struct node **headRef);   // actually solution to problem 18, but useful
void Reverse(struct node **headRef);    // actually solution to problem 17, but useful
// Displaying list
void PrintList(struct node* list);
void PrintListWithSuffix(struct node *list, char *suffix);
void PrintListDetail(struct node *list);

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

int Count(struct node* head, int searchFor)
{
	int count = 0;
	while (head != NULL) {
		if (head->data == searchFor)    count++;
		head = head->next;
	}
	return count;
}

struct node* MakeNode(int data)
{
	struct node* newNode = malloc(sizeof(struct node));

	newNode->data = data;
	newNode->next = NULL;

	return newNode;
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
}

struct node* BuildList(int len, ...)
{
	assert(len >= 0);
	
	if (len == 0) {
		return NULL;
	}
	
	int i, data;
	struct node *head = NULL, *cur = NULL;
	
	va_list ap;
	va_start(ap, len);

	// Create head node
	data = va_arg(ap, int);
	head = malloc(sizeof(struct node));
	// printf("\tBuildList(): head = %d\n", head);
	head->data = data;
	head->next = NULL;
	cur = head;
	
	for (i = 1; i < len; i++) {
		data = va_arg(ap, int);
		cur->next = malloc(sizeof(struct node));
		cur->next->data = data;
		cur->next->next = NULL;
		cur = cur->next;
	}
	
	va_end(ap);
	
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

void PrintListDetail(struct node *list)
{
	struct node *cur = list;
	while(cur != NULL) {
		printf("Address: %d\tData: %d\n", (int)cur, cur->data);
		cur = cur->next;
	}
}

// Given a list and an index, return the data
// in the nth node of the list.  The nodes are numbered from 0.
// Assert fails if the index is invalud (outsde 0..length-1).
int GetNth(struct node* head, int index)
{
	// Check for negative index
	assert(index >= 0);
	int i;
	for (i = 0; i < index; i++) {
		assert(head != NULL);
		head = head->next;
	}
	return head->data;
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
 Takes two lists sorted in increasing order, and
 splices their nodes together to make one big
 sorted list which is returned.
*/
struct node* SortedMerge(struct node* a, struct node* b)
{
	// Start by building the head node
	struct node *head = NULL;
	struct node *temp = NULL;
	
	if (a != NULL && b != NULL) {
		if (a->data < b->data) {
			head = a;
			a = a->next;
			head->next = NULL;
		} else { // b < a
			head = b;
			b = b->next;
			head->next = NULL;
		}
	} else if (a == NULL) {
		return b;
	} else if (b == NULL) {
		return a;
	}

	temp = head;
	while (a != NULL && b != NULL) {
		// Compare a and b, and append the node with small data.
		// Continue until either list is empty
		if (a->data < b->data) {
			temp->next = a;
			a = a->next;
		} else { // b < a
			temp->next = b;
			b = b->next;
		}
		// Advance temp node
		temp = temp->next;
		temp->next = NULL;
	}
	// At this point a is NULL xor b is NULL
	// Append the non-NULL list to temp
	if (a == NULL) {
		temp->next = b;
	} else if (b == NULL) {
		temp->next = a;
	}
	
	return head;
}

void MergeSort(struct node** headRef)
{
	// Implementation of the classic merge sort using 
	// FrontBackSplit() and SortedMerge() 

	// Split the list if it at least 2 elements
	// Merge sort these two sub-lists
	assert(headRef != NULL);

	struct node *front = NULL, *back = NULL;
	if (*headRef != NULL && (*headRef)->next != NULL) {
		FrontBackSplit(*headRef, &front, &back);
//		printf("\tMergeSorting list ");
//		PrintListWithSuffix(front, ", ");
//		PrintListWithSuffix(back, "\n");
		MergeSort(&front);
		MergeSort(&back);
//		printf("\tMergeSorted list ");
//		PrintListWithSuffix(front, ", ");
//		PrintListWithSuffix(back, "\n");
		// The front and back should now be in sorted order
		*headRef = SortedMerge(front, back);
//		printf("\tMergedList ");
//		PrintListWithSuffix(*headRef, "\n");
	}
}

/* Probelm 16: Compute a new sorted list that represents the intersection
of the two given sorted lists.
*/
struct node* SortedIntersect(struct node* a, struct node* b)
{
	// Since the lists are in ascending order, we only need to advance
	// the pointer of the list with smaller data value.
	// When nodes of equal data value are found, by default advance
	// node both lists a and b.
	struct node *output = NULL;

	while (a != NULL && b != NULL) {
		// Check for intersection
		if (a->data == b->data) {
			// intersection found.  Create new node and advance both lists
			Push(&output, a->data);
			a = a->next;
			b = b->next;
		}
		else if (a->data <= b->data) {
			// Advance list a, since it has smaller data value
			a = a->next;
		}
		else {
			// at this point a->data > b->data
			b = b->next;
		}
	}
	RecursiveReverse(&output);		// Reverse the list, since it's in descending order
	return output;
}

/*
Problem 17: Write an iterative Reverse() function that reverses a list by rearranging all the .next
pointers and the head pointer. Reverse() should only need to make one pass of the list. 
*/
void Reverse(struct node **headRef)
{
	assert(headRef != NULL);
	if (*headRef == NULL)	return;	// Nothing to reverse

	struct node *cur = (*headRef)->next;
	struct node *prev = *headRef;
	struct node *next = NULL;
	prev->next = NULL;

	while (cur != NULL) {
		next = cur->next;
		cur->next = prev;	// Reverse list
		prev = cur;		// Move one node down the list
		cur = next;
		next = (next == NULL) ? NULL : next->next;	// Check for end of list and assign accordingly
	}
	*headRef = prev;
}
/*
Recursively reverses the given linked list by changing its .next
pointers and its head pointer in one pass of the list.
*/
void RecursiveReverse(struct node** headRef)
{
	assert(headRef != NULL);
	if (*headRef == NULL)	return;	// Nothing to reverse

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
	
	int fail = 0;
	struct node *want = NULL, *got = NULL;

	got = BuildList(3, 5, 13, 42);    // build {5, 13, 42}

	// Test Case 1: insert in middle of list
	SortedInsert(&got, MakeNode(20));  // build {5, 13, 20, 42}
	want = BuildList(4, 5, 13, 20, 42);
	if (!CompareList(want, got)) {
		fail = 1;
		printf("FAIL: Got ");
        PrintList(got);
        printf(", want ");
        PrintListWithSuffix(want, "\n");
	}
	// Test Case 2: insert at end of list
	SortedInsert(&got, MakeNode(50));  // build {5, 13, 20, 42, 50}
	DeleteList(&want);
        want = BuildList(5, 5, 13, 20, 42, 50);
        if (!CompareList(want, got)) {
            fail = 1;
            printf("FAIL: Got ");
            PrintList(got);
            printf(", want ");
            PrintListWithSuffix(want, "\n");
        }
	// Test Case 3: insert at front of list
	SortedInsert(&got, MakeNode(3));  // build {3, 5, 13, 20, 42, 50}
	DeleteList(&want);
	want = BuildList(6, 3, 5, 13, 20, 42, 50);
	if (!CompareList(want, got)) {
		fail = 1;
        printf("FAIL: Got ");
        PrintList(got);
        printf(", want ");
        PrintListWithSuffix(want, "\n");
	}

	if (!fail) {
		printf("PASS\n");
		DeleteList(&want);
		DeleteList(&got);
	}
}

void InsertSortTest()
{
	printf("Running InsertSortTest()\t");

	int i;
	List *myList = BuildList(5, 42, 5, -2, 50, 13);

	InsertSort(&myList);

	for (i = 0; i < Length(myList) - 1; i++) {
		if (GetNth(myList, i) > GetNth(myList, i + 1)) {
			printf("FAIL: Got non-sorted list ");
			PrintListWithSuffix(myList, "\n");
			return;
		}
	}

	printf("PASS\n");
	DeleteList(&myList); // Clean up
}

void AppendTest()
{
	printf("Running AppendTest()\t");
	const int LEN = 8;  // Must be greater than 3
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
    
    int fail = 0;
    struct node *input = NULL, *wantFront = NULL, *wantBack = NULL;
    struct node *gotFront = NULL, *gotBack = NULL;
    
	// Null element list
    FrontBackSplit(input, &gotFront, &gotBack);
    if (! (CompareList(wantFront, gotFront) && CompareList(wantBack, gotBack)) ) {
        fail = 1;
        printf("FAIL: 0 element test. Want ");
        PrintListWithSuffix(wantFront, ", ");
        PrintList(wantBack);
        printf("; got ");
        PrintListWithSuffix(gotFront, ", ");
        PrintListWithSuffix(gotBack, "\n");
    }
    
    // 1 Element test
    input = BuildList(1, 42);
    wantFront = BuildList(1, 42);
    wantBack = NULL;
    FrontBackSplit(input, &gotFront, &gotBack);
    if (! (CompareList(wantFront, gotFront) && CompareList(wantBack, gotBack)) ) {
        fail = 1;
        printf("FAIL: 1 element test. Want ");
        PrintListWithSuffix(wantFront, ", ");
        PrintList(wantBack);
        printf("; got ");
        PrintListWithSuffix(gotFront, ", ");
        PrintListWithSuffix(gotBack, "\n");
    }
    DeleteList(&wantFront);
    DeleteList(&gotFront);
    
    // 2 Element test
    input = BuildList(2, 1, 2);
    wantFront = BuildList(1, 1);
    wantBack = BuildList(1, 2);
    FrontBackSplit(input, &gotFront, &gotBack);
    if (! (CompareList(wantFront, gotFront) && CompareList(wantBack, gotBack)) ) {
        fail = 1;
        printf("FAIL: 2 element test. Want ");
        PrintListWithSuffix(wantFront, ", ");
        PrintList(wantBack);
        printf("; got ");
        PrintListWithSuffix(gotFront, ", ");
        PrintListWithSuffix(gotBack, "\n");
    }
    DeleteList(&wantFront);
    DeleteList(&wantBack);
    DeleteList(&gotFront);
    DeleteList(&gotBack);
    
    // 3 element test
    input = BuildList(3, 1, 2, 3);
    wantFront = BuildList(2, 1, 2);
    wantBack = BuildList(1, 3);
    FrontBackSplit(input, &gotFront, &gotBack);
    if (! (CompareList(wantFront, gotFront) && CompareList(wantBack, gotBack)) ) {
        fail = 1;
        printf("FAIL: 3 element test. Want ");
        PrintListWithSuffix(wantFront, ", ");
        PrintList(wantBack);
        printf("; got ");
        PrintListWithSuffix(gotFront, ", ");
        PrintListWithSuffix(gotBack, "\n");
    }
    DeleteList(&wantFront);
    DeleteList(&wantBack);
    DeleteList(&gotFront);
    DeleteList(&gotBack);
    
    // 4 element test
    input = BuildList(4, 1, 2, 3, 4);
    wantFront = BuildList(2, 1, 2);
    wantBack = BuildList(2, 3, 4);
    FrontBackSplit(input, &gotFront, &gotBack);
    if (! (CompareList(wantFront, gotFront) && CompareList(wantBack, gotBack)) ) {
        fail = 1;
        printf("FAIL: 4 element test. Want ");
        PrintListWithSuffix(wantFront, ", ");
        PrintList(wantBack);
        printf("; got ");
        PrintListWithSuffix(gotFront, ", ");
        PrintListWithSuffix(gotBack, "\n");
    }
    DeleteList(&wantFront);
    DeleteList(&wantBack);
    DeleteList(&gotFront);
    DeleteList(&gotBack);
    
    // 5 element test
    input = BuildList(5, 1, 2, 3, 4, 5);
    wantFront = BuildList(3, 1, 2, 3);
    wantBack = BuildList(2, 4, 5);
    FrontBackSplit(input, &gotFront, &gotBack);
    if (! (CompareList(wantFront, gotFront) && CompareList(wantBack, gotBack)) ) {
        fail = 1;
        printf("FAIL: 5 element test. Want ");
        PrintListWithSuffix(wantFront, ", ");
        PrintList(wantBack);
        printf("; got ");
        PrintListWithSuffix(gotFront, ", ");
        PrintListWithSuffix(gotBack, "\n");
    }
    DeleteList(&wantFront);
    DeleteList(&wantBack);
    DeleteList(&gotFront);
    DeleteList(&gotBack);
    
    // 6 element test
    input = BuildList(6, 1, 2, 3, 4, 5, 6);
    wantFront = BuildList(3, 1, 2, 3);
    wantBack = BuildList(3, 4, 5, 6);
    FrontBackSplit(input, &gotFront, &gotBack);
    if (! (CompareList(wantFront, gotFront) && CompareList(wantBack, gotBack)) ) {
        fail = 1;
        printf("FAIL: 6 element test. Want ");
        PrintListWithSuffix(wantFront, ", ");
        PrintList(wantBack);
        printf("; got ");
        PrintListWithSuffix(gotFront, ", ");
        PrintListWithSuffix(gotBack, "\n");
    }
    DeleteList(&wantFront);
    DeleteList(&wantBack);
    DeleteList(&gotFront);
    DeleteList(&gotBack);
    
    // Verify if all test cases passed
    if (!fail) {
        printf("PASS\n");
    }
}

// Test 10.
void RemoveDuplicatesTest()
{
	printf("Running RemoveDuplicatesTest()");

	struct node* want = BuildList(5, 1, 2, 3, 13, 42);    // Want {1, 2, 3, 13, 42}
	struct node* got = BuildList(8, 1, 1, 2, 3, 3, 13, 42, 42);  // build {1, 1, 2, 3, 3, 13, 42, 42}  (test duplicate at beginning, middle and end)

	RemoveDuplicates(got);

	if (!CompareList(want, got)) {
        printf("\tFAIL\n");
        printf("\tWant ");
        PrintList(want);
        printf(", Got ");
        PrintListWithSuffix(got, "\n");
	}
	else {
        printf("\tPASS\n");
	}
    // Clean up
    DeleteList(&want);
    DeleteList(&got);
}

void MoveNodeTest()
{
	printf("Running MoveNodeTest()\t");
 
    int fail = 0;
	// Here are the expected result data
    struct node *wantA = BuildList(4, 1, 1, 2, 3);
    struct node *wantB = BuildList(2, 2, 3);
	// Here are the input test data
	struct node* gotA = BuildOneTwoThree(); // the list {1, 2, 3}
	struct node* gotB = BuildOneTwoThree();

	// Perform function
	MoveNode(&gotA, &gotB);
	// a == {1, 1, 2, 3}
	// b == {2, 3}

	// Verify results
	if (!CompareList(gotA, wantA)) {
		// a is not the same as wantA
        fail = 1;
		printf("FAIL: want ");
		PrintList(wantA);
		printf(", got ");
		PrintListWithSuffix(gotA, "\n");
	}
	if (!CompareList(gotB, wantB)) {
		// b is not the same as wantB
        fail = 1;
		printf("FAIL: want ");
		PrintList(wantB);
		printf(", got ");
		PrintListWithSuffix(gotB, "\n");
	}
	// Check if all test cases passed
    if (!fail) {
		printf("PASS\n");
	}
    
    // Clean up
    DeleteList(&wantA);
    DeleteList(&wantB);
    DeleteList(&gotA);
    DeleteList(&gotB);
}

void AlternateSplitTest()
{
	printf("Running AlternateSplitTest()\t");
    
    int fail = 0;

    struct node *input = BuildList(7, 0, 1, 2, 3, 4, 5, 6);
    // List is reverse due to using MoveNode() to implement AlternateSplit()
    struct node *wantA = BuildList(4, 6, 4, 2, 0);
    struct node *wantB = BuildList(3, 5, 3, 1);

	struct node *a = NULL, *b = NULL;
	AlternatingSplit(input, &a, &b);

	if (!CompareList(wantA, a)) {
        fail = 1;
		printf("FAIL: want ");
		PrintList(wantA);
		printf(", got ");
		PrintListWithSuffix(a, "\n");
	}
	else if (!CompareList(wantB, b)) {
        fail = 1;
		printf("FAIL: want ");
		PrintList(wantB);
		printf(", got ");
		PrintListWithSuffix(b, "\n");
	}
	
    if (!fail) {
		printf("PASS\n");
	}
    // Clean up
    DeleteList(&wantA);
    DeleteList(&wantB);
    DeleteList(&a);
    DeleteList(&b);
}

void ShuffleMergeTest()
{
	printf("Running ShuffleMergeTest()\t");

    int fail = 0;
    struct node* a = BuildList(5, 0, 2, 4, 6, 7);
    struct node* b = BuildList(3, 1, 3, 5);
    struct node* want = BuildList(8, 0, 1, 2, 3, 4, 5, 6, 7);

	struct node* got = ShuffleMerge(a, b);

	// Verify results
	if (!CompareList(got, want)) {
        fail = 1;
		printf("FAIL: want ");
		PrintList(want);
		printf(", got ");
		PrintListWithSuffix(got, "\n");
	}
	
    if (!fail) {
        printf("PASS\n");
	}

}

void SortedMergeTest()
{
	printf("Running SortedMergeTest()\t");
	int fail = 0;
	
	// Test Case 1: 2 non-NULL list, one ends 2 node early during merge
	struct node* inputA = BuildList(4, -2, 4, 13, 35);
	struct node* inputB = BuildList(5, -5, 13, 14, 18, 44);
	struct node* want = BuildList(9, -5, -2, 4, 13, 13, 14, 35, 18, 44);
	struct node* got = SortedMerge(inputA, inputB);
	
	if (!CompareList(want, got)) {
		fail = 1;
		printf("FAIL: want ");
		PrintList(want);
		printf(", got ");
		PrintListWithSuffix(got, "\n");
	}
	DeleteList(&want);
	DeleteList(&got);
	
	// Test Case 2, 3: 1 non-NULL, 1 NULL list
	inputA = BuildOneTwoThree();
	inputB = NULL;
	want = BuildOneTwoThree();
	got = SortedMerge(inputA, inputB);
		
	if (!CompareList(want, got)) {
		fail = 1;
		printf("FAIL: want ");
		PrintList(want);
		printf(", got ");
		PrintListWithSuffix(got, "\n");
	}
	DeleteList(&want);
	DeleteList(&got);
	
	// Test Case 3: 1 NULL, 1 non-NULL list (reverse arg order of test case 2)
	inputA = NULL;
	inputB = BuildOneTwoThree();
	want = BuildOneTwoThree();
	got = SortedMerge(inputA, inputB);
		
	if (!CompareList(want, got)) {
		fail = 1;
		printf("FAIL: want ");
		PrintList(want);
		printf(", got ");
		PrintListWithSuffix(got, "\n");
	}
	DeleteList(&want);
	DeleteList(&got);
	
	// Test Case 4: Both NULL list
	inputA = NULL;
	inputB = NULL;
	want = NULL;
	got = SortedMerge(inputA, inputB);
		
	if (!CompareList(want, got)) {
		fail = 1;
		printf("FAIL: want ");
		PrintList(want);
		printf(", got ");
		PrintListWithSuffix(got, "\n");
	}
	
	if (!fail) {
		printf("PASS\n");
	}
	
}


void MergeSortTest()
{
	printf("Running MergeSortTest()\t");
	
	int fail = 0;
	struct node *want = BuildList(9, 0, 1, 2, 3, 4, 5, 6, 7, 8);
	struct node *got = BuildList(9, 8, 4, 3, 6, 7, 0, 1, 2, 5);

	MergeSort(&got);

	if (!CompareList(want, got)) {
		fail = 1;
		printf("FAIL: want ");
		PrintList(want);
		printf(", got ");
		PrintListWithSuffix(got, "\n");
	}

	if (!fail) {
		printf("PASS\n");
	}
}

void SortedIntersectTest()
{
	printf("Running SortedIntersectTest()\t");
	int fail = 0;
	struct node *a, *b, *want, *got;

	// Test Case 1: list a, b not NULL, intersection not NULL
	//				intersection found at beginning, middle, and end of list
	a = BuildList(5, 1, 2, 6, 7, 8);
	b = BuildList(7, -3, 2, 5, 7, 9, 12, 100);
	want = BuildList(2, 2, 7);
	got = SortedIntersect(a, b);
	if (!CompareList(want, got)) {
		fail = 1;
		printf("FAIL: want ");
		PrintList(want);
		printf(", got ");
		PrintListWithSuffix(got, "\n");
	}
	DeleteList(&a);
	DeleteList(&b);
	DeleteList(&want);
	if(!got)	DeleteList(&got);	// Delete only if not NULL

	// Test Case 2: list a, b not NULL, intersection NULL
	a = BuildList(5, 1, 2, 6, 7, 8);
	b = BuildList(7, -3, 3, 5, 9, 10, 12, 100);
	want = NULL;
	got = SortedIntersect(a, b);
	if (!CompareList(want, got)) {
		fail = 1;
		printf("FAIL: want ");
		PrintList(want);
		printf(", got ");
		PrintListWithSuffix(got, "\n");
	}
	DeleteList(&a);
	DeleteList(&b);
	if (!got)	DeleteList(&got);

	// Test Case 3: list a NULL, b not NULL
	a = NULL;
	b = BuildOneTwoThree();
	want = NULL;
	got = SortedIntersect(a, b);
	if (!CompareList(want, got)) {
		fail = 1;
		printf("FAIL: want ");
		PrintList(want);
		printf(", got ");
		PrintListWithSuffix(got, "\n");
	}
	DeleteList(&b);
	if (!got)	DeleteList(&got);

	// Test Case 4: list a not NULL, b NULL
	a = BuildOneTwoThree();
	b = NULL;
	want = NULL;
	got = SortedIntersect(a, b);
	if (!CompareList(want, got)) {
		fail = 1;
		printf("FAIL: want ");
		PrintList(want);
		printf(", got ");
		PrintListWithSuffix(got, "\n");
	}
	DeleteList(&a);
	if (!got)	DeleteList(&got);

	// Test Case 5: list a, b both NULL
	a = NULL;
	b = NULL;
	want = NULL;
	got = SortedIntersect(a, b);
	if (!CompareList(want, got)) {
		fail = 1;
		printf("FAIL: want ");
		PrintList(want);
		printf(", got ");
		PrintListWithSuffix(got, "\n");
	}
	if (!got)	DeleteList(&got);

	// Verify pass/fail 
	if (!fail) {
		printf("PASS\n");
	}
}

void ReverseTest() {
	printf("Running ReverseTest()\t");

	int fail = 0;
	struct node* got;
	struct node *want;

	// Test case 1: 5 elements
	got = BuildList(5, 1, 2, 3, 4, 5);
	want = BuildList(5, 5, 4, 3, 2, 1);
	Reverse(&got);
	if (!CompareList(got, want)) {
		fail = 1;
		printf("\tFAIL: want ");
		PrintList(want);
		printf(", got ");
		PrintListWithSuffix(got, "\n");
	}
	DeleteList(&got); // clean up after ourselves
	DeleteList(&want);

	// Test case 2: 3 elements
	got = BuildOneTwoThree();
	want = BuildList(3, 3, 2, 1);
	Reverse(&got);
	if (!CompareList(got, want)) {
		fail = 1;
		printf("\tFAIL: want ");
		PrintList(want);
		printf(", got ");
		PrintListWithSuffix(got, "\n");
	}
	DeleteList(&got); // clean up after ourselves
	DeleteList(&want);

	// Test case 3: 2 elements
	got = BuildList(2, 1, 2);
	want = BuildList(2, 2, 1);
	Reverse(&got);
	if (!CompareList(got, want)) {
		fail = 1;
		printf("\tFAIL: want ");
		PrintList(want);
		printf(", got ");
		PrintListWithSuffix(got, "\n");
	}
	DeleteList(&got); // clean up after ourselves
	DeleteList(&want);

	// Test case 4: 1 elements
	got = BuildList(1, 1);
	want = BuildList(1, 1);
	Reverse(&got);
	if (!CompareList(got, want)) {
		fail = 1;
		printf("\tFAIL: want ");
		PrintList(want);
		printf(", got ");
		PrintListWithSuffix(got, "\n");
	}
	DeleteList(&got); // clean up after ourselves
	DeleteList(&want);

	// Test case 5: 0 elements (NULL)
	got = NULL;
	want = NULL;
	Reverse(&got);
	if (!CompareList(got, want)) {
		fail = 1;
		printf("\tFAIL: want ");
		PrintList(want);
		printf(", got ");
		PrintListWithSuffix(got, "\n");
	}

	if (!fail) {
		printf("\tPASS\n");
	}
}

void RecursiveReverseTest()
{
	int fail = 0;
	
	printf("Running RecursiveReverseTest()");
	struct node* want = BuildList(6, 0, 1, 2, 3, 4, 5);
	struct node* got = BuildList(6, 5, 4, 3, 2, 1, 0);
	
	RecursiveReverse(&got);

	if (!CompareList(want, got)) {
		fail  = 1;
		printf("\tFAIL: want ");
		PrintList(want);
		printf(", got ");
		PrintListWithSuffix(got, "\n");
	}
	DeleteList(&want);
	DeleteList(&got);
	
	if (!fail) {
		printf("\tPASS\n");
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
	SortedMergeTest();	// Problem 14
	MergeSortTest();	// Problem 15
	SortedIntersectTest();	// Problem 16
	ReverseTest();	// Problem 17
	RecursiveReverseTest();	// Problem 18

	return 0;
}
