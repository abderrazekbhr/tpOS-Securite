# Linked List Operations Summary

This project provides functions to manipulate both singly and doubly linked lists in C. Below is an overview of the key functions implemented:

### 1. **sizeLinkedList**
   - **Description**: Returns the size (number of elements) of the linked list.
   - **Prototype**: `int sizeLinkedList(struct linkedList *head)`
   - **Parameters**: 
     - `head`: Pointer to the first element of the linked list.
   - **Returns**: The size of the linked list as an integer.

### 2. **displayLinkedList**
   - **Description**: Displays the values and memory addresses of the elements in the linked list.
   - **Prototype**: `void displayLinkedList(struct linkedList *head)`
   - **Parameters**:
     - `head`: Pointer to the first element of the linked list.
   - **Output**: Prints each element’s value and memory address.

### 3. **removeFirstElement**
   - **Description**: Removes the first element from a singly linked list.
   - **Prototype**: `void removeFirstElement(struct linkedList **head)`
   - **Parameters**:
     - `head`: Double pointer to the first element of the linked list.
   - **Effect**: Deletes the first element, freeing its memory.

### 4. **removeFirstElementInDoubleLinkedList**
   - **Description**: Removes the first element from a doubly linked list.
   - **Prototype**: `void removeFirstElementInDoubleLinkedList(struct linkedList **head)`
   - **Parameters**:
     - `head`: Double pointer to the first element of the doubly linked list.
   - **Effect**: Deletes the first element and adjusts the previous pointer of the next element.

### 5. **removeLastElement**
   - **Description**: Removes the last element from a singly linked list.
   - **Prototype**: `void removeLastElement(struct linkedList **head)`
   - **Parameters**:
     - `head`: Double pointer to the first element of the linked list.
   - **Effect**: Deletes the last element, freeing its memory.

### 6. **removeLastElementInDoubleLinkedList**
   - **Description**: Removes the last element from a doubly linked list.
   - **Prototype**: `void removeLastElementInDoubleLinkedList(struct linkedList **head)`
   - **Parameters**:
     - `head`: Double pointer to the first element of the doubly linked list.
   - **Effect**: Deletes the last element, adjusting the pointers appropriately.

### 7. **addAtFirstPos**
   - **Description**: Adds a new element at the beginning of a singly linked list.
   - **Prototype**: `void addAtFirstPos(struct linkedList **head, int val)`
   - **Parameters**:
     - `head`: Double pointer to the first element of the linked list.
     - `val`: The value to add.
   - **Effect**: Inserts a new element at the start of the list.

### 8. **addAtFirstPosInDoubleLinkedList**
   - **Description**: Adds a new element at the beginning of a doubly linked list.
   - **Prototype**: `void addAtFirstPosInDoubleLinkedList(struct linkedList **head, int val)`
   - **Parameters**:
     - `head`: Double pointer to the first element of the doubly linked list.
     - `val`: The value to add.
   - **Effect**: Inserts a new element at the start of the doubly linked list, updating the `previous` pointer.

### 9. **addAtLastPos**
   - **Description**: Adds a new element at the end of a singly linked list.
   - **Prototype**: `void addAtLastPos(struct linkedList **head, int val)`
   - **Parameters**:
     - `head`: Double pointer to the first element of the linked list.
     - `val`: The value to add.
   - **Effect**: Inserts a new element at the end of the list.

### 10. **addAtLastPosInDoubleLinkedList**
   - **Description**: Adds a new element at the end of a doubly linked list.
   - **Prototype**: `void addAtLastPosInDoubleLinkedList(struct linkedList **head, int val)`
   - **Parameters**:
     - `head`: Double pointer to the first element of the doubly linked list.
     - `val`: The value to add.
   - **Effect**: Inserts a new element at the end, updating the `previous` pointer.

### 11. **concatenateLinkedList**
   - **Description**: Concatenates two singly linked lists.
   - **Prototype**: `void concatenateLinkedList(struct linkedList **head1, struct linkedList **head2)`
   - **Parameters**:
     - `head1`: Double pointer to the first linked list.
     - `head2`: Double pointer to the second linked list.
   - **Effect**: Appends the second list to the end of the first list.

### 12. **changeLinkedListByFunction**
   - **Description**: Modifies the value of each element in the list using a function.
   - **Prototype**: `void changeLinkedListByFunction(struct linkedList *head, int (*f)(int))`
   - **Parameters**:
     - `head`: Pointer to the first element of the list.
     - `f`: Function pointer that takes an integer and returns a modified integer.
   - **Effect**: Applies the function to each value in the list.

### 13. **square / cube**
   - **Description**: Functions used to square or cube the value of an integer.
   - **Prototypes**: 
     - `int square(int x)`
     - `int cube(int x)`
   - **Effect**: Returns the square or cube of the input value.

### 14. **freeLinkedList**
   - **Description**: Frees the memory allocated for the entire linked list.
   - **Prototype**: `void freeLinkedList(struct linkedList **head)`
   - **Parameters**:
     - `head`: Double pointer to the first element of the list.
   - **Effect**: Frees the memory for all elements of the list.

### Main Program Example
The main program demonstrates:
- Creating singly and doubly linked lists.
- Adding, removing, and modifying elements.
- Displaying list contents.
- Concatenating lists.
- Converting a doubly linked list into a circular list.
