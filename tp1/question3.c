#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
struct linkedList
{
    int val;
    struct linkedList *next;
    struct linkedList *previous; // add previous pointer for doubly linked list
};

// 2. Function that returns the size of a linked list.
int sizeLinkedList(struct linkedList *head)
{
    int size = 0;
    while (head != NULL)
    {
        size++;
        head = head->next;
    }
    return size;
}

// 3. Function that displays the elements of a linked list along with the address.
void displayLinkedList(struct linkedList *head)
{
    while (head != NULL)
    {
        printf("address : %p -- value : %d\n", (void *)head, head->val);
        head = head->next;
    }
}

// 4. Remove the first element of the linked list.
void removeFirstElement(struct linkedList **head)
{
    if (*head)
    {
        struct linkedList *temp = *head;
        *head = (*head)->next;
        free(temp);
    }
    else
    {
        fprintf(stderr, "list is empty\n");
    }
}
void removeFirstElementInDoubleLinkedList(struct linkedList **head)
{
    if (*head)
    {
        struct linkedList *temp = *head;
        *head = (*head)->next;
        if (*head)
        {
            (*head)->previous = NULL;
        }
        free(temp);
    }
    else
    {
        fprintf(stderr, "list is empty\n");
    }
}

// 5. Remove the last element (in addition, returns the last element).
void removeLastElement(struct linkedList **head)
{
    if (*head == NULL)
    {
        fprintf(stderr, "list is empty\n");
        return;
    }
    if ((*head)->next == NULL)
    {
        free(*head);
        *head = NULL;
        return;
    }

    struct linkedList *temp = *head;
    while (temp->next->next != NULL)
    {
        temp = temp->next;
    }
    free(temp->next);
    temp->next = NULL;
}
void removeLastElementInDoubleLinkedList(struct linkedList **head)
{
    if (*head == NULL)
    {
        fprintf(stderr, "list is empty\n");
        return;
    }
    if ((*head)->next == NULL)
    {
        free(*head);
        *head = NULL;
        return;
    }

    struct linkedList *temp = *head;
    while (temp->next->next != NULL)
    {
        temp = temp->next;
    }
    free(temp->next);
    temp->next = NULL;
}

// 6. Add an element at the beginning of the list.
void addAtFirstPos(struct linkedList **head, int val)
{
    struct linkedList *newNode = (struct linkedList *)malloc(sizeof(struct linkedList));
    newNode->val = val;
    newNode->next = *head;
    *head = newNode;
}

// Version for doubly linked list: Add an element at the beginning.
void addAtFirstPosInDoubleLinkedList(struct linkedList **head, int val)
{
    struct linkedList *newNode = (struct linkedList *)malloc(sizeof(struct linkedList));
    newNode->val = val;
    newNode->previous = NULL;
    newNode->next = *head;
    if (*head)
    {
        (*head)->previous = newNode;
    }
    *head = newNode;
}

// 7. Add an element at the end of the list.
void addAtLastPos(struct linkedList **head, int val)
{
    struct linkedList *newNode = (struct linkedList *)malloc(sizeof(struct linkedList));
    newNode->val = val;
    newNode->next = NULL;

    if (*head == NULL)
    {
        *head = newNode;
        return;
    }

    struct linkedList *temp = *head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = newNode;
}

// Version for doubly linked list: Add an element at the end.
void addAtLastPosInDoubleLinkedList(struct linkedList **head, int val)
{
    struct linkedList *newNode = (struct linkedList *)malloc(sizeof(struct linkedList));
    newNode->val = val;
    newNode->next = NULL;

    if (*head == NULL)
    {
        *head = newNode;
        return;
    }

    struct linkedList *temp = *head;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }

    temp->next = newNode;
    newNode->previous = temp;
}

// 8. Concatenate two linked lists.
void concatenateLinkedList(struct linkedList **head1, struct linkedList **head2)
{
    if (*head1 == NULL)
    {
        *head1 = *head2;
        return;
    }
    if (*head2 == NULL)
    {
        return;
    }

    struct linkedList *temp = *head1;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }
    temp->next = *head2;
    *head2 = NULL;
}

// 9. Modify the value of each element using a function (e.g., square or cube).
void changeLinkedListByFunction(struct linkedList *head, int (*f)(int))
{
    while (head != NULL)
    {
        head->val = f(head->val);
        head = head->next;
    }
}

// Function to square a value.
int square(int x)
{
    return x * x;
}

// Function to cube a value.
int cube(int x)
{
    return x * x * x;
}

// 10. Free the memory allocated for a linked list.
void freeLinkedList(struct linkedList **head)
{
    struct linkedList *temp;
    while (*head != NULL)
    {
        temp = *head;
        *head = (*head)->next;
        temp->next = NULL;
        free(temp);
    }
}

int main()
{
    // 1. Create a linked list containing integers from 1 to n in ascending order.
    int n = 10;
    struct linkedList *head = NULL;
    struct linkedList *lHead = NULL;

    for (int i = 1; i <= n; i++)
    {
        addAtLastPos(&head, i);
    }
    for (int i = 1; i <= n; i++)
    {
        addAtFirstPos(&lHead, i * 5);
    }

    printf("---- Size of the linked list: %d ----\n", sizeLinkedList(head));
    displayLinkedList(head);

    // Remove the first element.
    removeFirstElement(&head);
    printf("---- Size of the linked list after removing the first element: %d ----\n", sizeLinkedList(head));
    displayLinkedList(head);

    // Remove the last element.
    removeLastElement(&head);
    printf("---- Size of the linked list after removing the last element: %d ----\n", sizeLinkedList(head));
    displayLinkedList(head);

    // Concatenate head and lHead.
    concatenateLinkedList(&head, &lHead);
    printf("---- Size of the linked list after concatenation: %d ----\n", sizeLinkedList(head));
    displayLinkedList(head);

    // Change the value of each element using the square function.
    printf("---- Modify the value of each element using the square function ----\n");
    changeLinkedListByFunction(head, square);
    displayLinkedList(head);

    // Generate a doubly linked list.
    struct linkedList *doubleLinkedList = NULL;
    for (int i = 1; i <= n; i++)
    {
        addAtFirstPosInDoubleLinkedList(&doubleLinkedList, i * 7);
        addAtLastPosInDoubleLinkedList(&doubleLinkedList, i * 7);
    }
    printf("---- Size of the doubly linked list: %d ----\n", sizeLinkedList(doubleLinkedList));
    displayLinkedList(doubleLinkedList);

    // Remove the first element in the doubly linked list.
    removeFirstElementInDoubleLinkedList(&doubleLinkedList);
    printf("---- Size of the doubly linked list after removing the first element: %d ----\n", sizeLinkedList(doubleLinkedList));
    displayLinkedList(doubleLinkedList);

    // Remove the last element in the doubly linked list.
    removeLastElementInDoubleLinkedList(&doubleLinkedList);
    printf("---- Size of the doubly linked list after removing the last element: %d ----\n", sizeLinkedList(doubleLinkedList));
    displayLinkedList(doubleLinkedList);

    // Transform the doubly linked to Cercular
    struct linkedList *temp = doubleLinkedList;
    while (temp->next != NULL)
    {
        temp = temp->next;
    }

    temp->next = doubleLinkedList;
    doubleLinkedList->previous = temp;

    // Display the circular doubly linked list
    printf("---- Display the circular doubly linked list ----\n");
    temp = doubleLinkedList;
    for (int i = 0; i < 2 * n - 3; i++)
    {
        printf("address : %p -- value : %d\n", (void *)temp, temp->val);
        temp = temp->next;
    }
    
    // Free the linked lists.
    freeLinkedList(&head);
    freeLinkedList(&lHead);
    freeLinkedList(&doubleLinkedList);

    return 0;
}
