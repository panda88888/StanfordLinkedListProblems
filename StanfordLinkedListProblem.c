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
    head = malloc( sizeof(struct node) );
    head->data = 1;

    tail = head;
    for(i = 1; i < 3; i++) {
        tail->next = malloc( sizeof(struct node) );
        tail->next->data = i + 1;
        tail = tail->next;
    }
    return head;
}

void Push(struct node** headRef, int newData)
{
    // Standard 3-step-link-in
    // 1. Create new node
    struct node* newHead = malloc( sizeof(struct node) );
    newHead->data = newData;
    // 2. Set its .next to the current head
    newHead->next = *headRef;
    // 3. Change the head point to the new node
    *headRef = newHead;
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
    
    int i;
    struct node *current = list;

    while (current != NULL) {
        printf("%d ", current->data);
        current = current->next;
    }
    printf("}\n");
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

    temp = malloc( sizeof(struct node) );
    temp->data = data;
    
    // Traverse the list to (index-1)th node
    for (i = 0; i < index - 1; i ++) {
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
    } else {
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

// Test Function
void SectionBreak()
{
    printf("\n--------------------\n\n");
}

void BasicsCaller()
{
    printf("Begin BasicCaller()\n");
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
    
    int i;
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
    int got, want = 1;
    List *myList = BuildOneTwoThree();   // build {1, 2, 3}
    got = Count(myList, 2);
    
    // Ceck output
    if (got != want) {
        printf("FAIL\tCountTest(): Got %d, Want %d\n", got, want);
    } else {
        printf("PASS\tCountTest()\n");
    }
    DeleteList(&myList);
}

void GetNthTest()
{
    int got, want = 3;

    List *myList = BuildOneTwoThree();  // Build {1, 2, 3}
    got = GetNth(myList, 2);   // Returns the value 3 
    // Ceck output
    if (got != want) {
        printf("FAIL\tGetNthTestTest(): Got %d, Want %d\n", got, want);
    } else {
        printf("PASS\tGetNthTest()\n");
    }
    DeleteList(&myList);
}

void DeleteListTest()
{
    int got, want = 0;

    List *myList = BuildOneTwoThree();  // Building {1, 2, 3}
    DeleteList(&myList);  // Deletes the three nodes and sets myList to NULL
    got = Length(myList);

    // Ceck output
    if (got != want) {
        printf("FAIL\tGetNthTest(): Got %d, Want %d\n", got, want);
    } else {
        printf("PASS\tGetNthTest()\n");
    }
}

void PopTest()
{
    int got, want;

    List *myList = BuildOneTwoThree();  // Build {1, 2, 3}
    
    want = 1;
    got = Pop(&myList);     // Deletes "1" node and returns 1
    if (got != want) {
        printf("FAIL\tPopTest(): Got %d, Want %d\n", got, want);
    }
    want = 2;
    got = Pop(&myList);     // Deletes "1" node and returns 2
    if (got != want) {
        printf("FAIL\tPopTest(): Got %d, Want %d\n", got, want);
    }
    want = 3;
    got = Pop(&myList);     // Deletes "3" node and returns 3
    if (got != want) {
        printf("FAIL\tPopTest(): Got %d, Want %d\n", got, want);
    }
    want = 0;
    got = Length(myList);   // The list is now empty, so len == 0
    if (got != want) {
        printf("FAIL\tPopTest(): Got %d, Want %d\n", got, want);
    }

    // All test cases passed, so pass
    printf("PASS\tPopTest()\n");
}

void InsertNthTest()
{
    int got, want;

    List *myList = NULL;    // Start with the empty list

    InsertNth(&myList, 0, 13);  // build {13}
    InsertNth(&myList, 1, 42);  // build {13, 42}
    InsertNth(&myList, 1, 5);   // build {13, 5, 42}

    want = 13;
    got = GetNth(myList, 0);
    if (got != want) {
        printf("FAIL\tInsertNthTest(): Got %d, Want %d\n", got, want);
    }
    want = 5;
    got = GetNth(myList, 1);
    if (got != want) {
        printf("FAIL\tInsertNthTest(): Got %d, Want %d\n", got, want);
    }
    want = 42;
    got = GetNth(myList, 2);
    if (got != want) {
        printf("FAIL\tInsertNthTest(): Got %d, Want %d\n", got, want);
    }
    DeleteList(&myList);    // Clean up after ourselves
    
    printf("PASS\tInsertNthTest()\n");
}

void SortedInsertTest()
{

    int got, want;

    List *myList = NULL;    // Start with the empty list
    struct node* temp;

    Push(&myList, 42);  // build {42}
    Push(&myList, 13);  // build {13, 42}
    Push(&myList, 5);   // build {5, 13, 42}
    
    want = 20;
    temp = malloc( sizeof(struct node) );
    temp->data = want;
    SortedInsert(&myList, temp);  // build {5, 13, 20, 42}
    got = GetNth(myList, 2);
    if (got != want) {
        printf("FAIL\tSortedInsertTest(): Got %d, Want %d\n", got, want);
    }
    want = 50;
    temp = malloc( sizeof(struct node) );
    temp->data = want;
    SortedInsert(&myList, temp);  // build {5, 13, 20, 42}
    got = GetNth(myList, 4);
    if (got != want) {
        printf("FAIL\tSortedInsertTest(): Got %d, Want %d\n", got, want);
    }
    want = 1;
    temp = malloc( sizeof(struct node) );
    temp->data = want;
    SortedInsert(&myList, temp);  // build {5, 13, 20, 42}
    got = GetNth(myList, 0);
    if (got != want) {
        printf("FAIL\tSortedInsertTest(): Got %d, Want %d\n", got, want);
    }

    printf("PASS\tSortedInsertTest()\n");
    DeleteList(&myList);
}

void InsertSortTest()
{
    int i;
    List *myList = NULL;
    
    Push(&myList, 42);
    Push(&myList, 5);
    Push(&myList, -2);
    Push(&myList, 50);
    Push(&myList, 13);

    InsertSort(&myList);

    for (i = 0; i < Length(myList) - 1; i++) {
        if (GetNth(myList, i) > GetNth(myList, i+1) ) {
            printf("FAIL\tInsertSortTest(): Got non-sorted list ");
            PrintList(myList);
            return;
        }
    }

    printf("PASS\tInsertSortTest(): Got sorted list ");
    PrintList(myList);
    DeleteList(&myList); // Clean up
}

#define LEN 8

void AppendTest()
{
    //const int LEN = 8;  // Must be greater than 3
    int values[LEN];
    int i = 0, want = 0, got = 0;

    for (i = 0; i < LEN; i++) {
        values[i] = rand() % 50 - 25;
    }

    List  *a = NULL, *b = NULL;
    for (i = 0; i < 3; i++) { // Build the list backward since Push() inserts data at index 0
        Push(&b, values[LEN-i-1]);
    }
    for (i = Length(b) ; i < LEN; i++) {
        Push(&a, values[LEN-i-1]);
    }

    Append(&a, &b);

    // Test Case: check length
    want = LEN;
    got = Length(a);
    if (want != got) {
        printf("FAIL\tAppendTest(): Incorrect length. Got %d, Want %d\n", got, want);
        return;
    }
    // Test Case: check data
    for (i = 0; i < LEN; i++) {
        want = values[i];
        got = GetNth(a, i);
        if (want != got) {
            printf("FAIL\tAppendTest(): Index = %d. Got %d, Want %d\n", i, got, want);
            return;
        }
    }
    // Test Case: check b is NULL
    if (b != NULL) {
        printf("FAIL\tAppendTest(): b is not set to NULL\n");
        return;
    }

    DeleteList(&a);
    printf("PASS\tAppendTest()\n");
}

// Main
int main(int argc, char *argv[])
{
    // BasicsCaller();
    CountTest();
    GetNthTest();
    DeleteListTest();
    PopTest();
    InsertNthTest();
    SortedInsertTest();
    InsertSortTest();
    AppendTest();

    return 0;
}
