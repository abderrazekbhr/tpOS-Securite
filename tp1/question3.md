# Linked List Operations Summary

This project provides functions to manipulate both singly and doubly linked lists in C. Below is an overview of the key functions implemented:

### 1. [**sizeLinkedList**](https://github.com/abderrazekbhr/tpOS-Securite/blob/f64a8f4a9f631443f3b5283d10f1886b3ab278e2/tp1/question3.c#L12)
   - **Description**: Returns the size (number of elements) of the linked list.
   - **Prototype**: `int sizeLinkedList(struct linkedList *head)`
   - **Parameters**: 
     - `head`: Pointer to the first element of the linked list.
   - **Returns**: The size of the linked list as an integer.

### 2. [**displayLinkedList**](https://github.com/abderrazekbhr/tpOS-Securite/blob/f64a8f4a9f631443f3b5283d10f1886b3ab278e2/tp1/question3.c#L24)
   - **Description**: Displays the values and memory addresses of the elements in the linked list.
   - **Prototype**: `void displayLinkedList(struct linkedList *head)`
   - **Parameters**:
     - `head`: Pointer to the first element of the linked list.
   - **Output**: Prints each element’s value and memory address.

### 3. [**removeFirstElement**](https://github.com/abderrazekbhr/tpOS-Securite/blob/f64a8f4a9f631443f3b5283d10f1886b3ab278e2/tp1/question3.c#L34)
   - **Description**: Removes the first element from a singly linked list.
   - **Prototype**: `void removeFirstElement(struct linkedList **head)`
   - **Parameters**:
     - `head`: Double pointer to the first element of the linked list.
   - **Effect**: Deletes the first element, freeing its memory.

### 4. [**removeFirstElementInDoubleLinkedList**](https://github.com/abderrazekbhr/tpOS-Securite/blob/f64a8f4a9f631443f3b5283d10f1886b3ab278e2/tp1/question3.c#L47)
   - **Description**: Removes the first element from a doubly linked list.
   - **Prototype**: `void removeFirstElementInDoubleLinkedList(struct linkedList **head)`
   - **Parameters**:
     - `head`: Double pointer to the first element of the doubly linked list.
   - **Effect**: Deletes the first element and adjusts the previous pointer of the next element.

### 5. [**removeLastElement**](https://github.com/abderrazekbhr/tpOS-Securite/blob/f64a8f4a9f631443f3b5283d10f1886b3ab278e2/tp1/question3.c#L66)
   - **Description**: Removes the last element from a singly linked list.
   - **Prototype**: `void removeLastElement(struct linkedList **head)`
   - **Parameters**:
     - `head`: Double pointer to the first element of the linked list.
   - **Effect**: Deletes the last element, freeing its memory.

### 6. [**removeLastElementInDoubleLinkedList**](https://github.com/abderrazekbhr/tpOS-Securite/blob/f64a8f4a9f631443f3b5283d10f1886b3ab278e2/tp1/question3.c#L88)
   - **Description**: Removes the last element from a doubly linked list.
   - **Prototype**: `void removeLastElementInDoubleLinkedList(struct linkedList **head)`
   - **Parameters**:
     - `head`: Double pointer to the first element of the doubly linked list.
   - **Effect**: Deletes the last element, adjusting the pointers appropriately.

### 7. [**addAtFirstPos**](https://github.com/abderrazekbhr/tpOS-Securite/blob/f64a8f4a9f631443f3b5283d10f1886b3ab278e2/tp1/question3.c#L112)
   - **Description**: Adds a new element at the beginning of a singly linked list.
   - **Prototype**: `void addAtFirstPos(struct linkedList **head, int val)`
   - **Parameters**:
     - `head`: Double pointer to the first element of the linked list.
     - `val`: The value to add.
   - **Effect**: Inserts a new element at the start of the list.

### 8. [**addAtFirstPosInDoubleLinkedList**](https://github.com/abderrazekbhr/tpOS-Securite/blob/f64a8f4a9f631443f3b5283d10f1886b3ab278e2/tp1/question3.c#L121)
   - **Description**: Adds a new element at the beginning of a doubly linked list.
   - **Prototype**: `void addAtFirstPosInDoubleLinkedList(struct linkedList **head, int val)`
   - **Parameters**:
     - `head`: Double pointer to the first element of the doubly linked list.
     - `val`: The value to add.
   - **Effect**: Inserts a new element at the start of the doubly linked list, updating the `previous` pointer.

### 9. [**addAtLastPos**](https://github.com/abderrazekbhr/tpOS-Securite/blob/f64a8f4a9f631443f3b5283d10f1886b3ab278e2/tp1/question3.c#L135)
   - **Description**: Adds a new element at the end of a singly linked list.
   - **Prototype**: `void addAtLastPos(struct linkedList **head, int val)`
   - **Parameters**:
     - `head`: Double pointer to the first element of the linked list.
     - `val`: The value to add.
   - **Effect**: Inserts a new element at the end of the list.

### 10. [**addAtLastPosInDoubleLinkedList**](https://github.com/abderrazekbhr/tpOS-Securite/blob/f64a8f4a9f631443f3b5283d10f1886b3ab278e2/tp1/question3.c#L156)
   - **Description**: Adds a new element at the end of a doubly linked list.
   - **Prototype**: `void addAtLastPosInDoubleLinkedList(struct linkedList **head, int val)`
   - **Parameters**:
     - `head`: Double pointer to the first element of the doubly linked list.
     - `val`: The value to add.
   - **Effect**: Inserts a new element at the end, updating the `previous` pointer.

### 11. [**concatenateLinkedList**](https://github.com/abderrazekbhr/tpOS-Securite/blob/f64a8f4a9f631443f3b5283d10f1886b3ab278e2/tp1/question3.c#L179)
   - **Description**: Concatenates two singly linked lists.
   - **Prototype**: `void concatenateLinkedList(struct linkedList **head1, struct linkedList **head2)`
   - **Parameters**:
     - `head1`: Double pointer to the first linked list.
     - `head2`: Double pointer to the second linked list.
   - **Effect**: Appends the second list to the end of the first list.

### 12. [**changeLinkedListByFunction**](https://github.com/abderrazekbhr/tpOS-Securite/blob/f64a8f4a9f631443f3b5283d10f1886b3ab278e2/tp1/question3.c#L201)
   - **Description**: Modifies the value of each element in the list using a function.
   - **Prototype**: `void changeLinkedListByFunction(struct linkedList *head, int (*f)(int))`
   - **Parameters**:
     - `head`: Pointer to the first element of the list.
     - `f`: Function pointer that takes an integer and returns a modified integer.
   - **Effect**: Applies the function to each value in the list.

### 13. [**square / cube**](https://github.com/abderrazekbhr/tpOS-Securite/blob/f64a8f4a9f631443f3b5283d10f1886b3ab278e2/tp1/question3.c#L211)
   - **Description**: Functions used to square or cube the value of an integer.
   - **Prototypes**: 
     - `int square(int x)`
     - `int cube(int x)`
   - **Effect**: Returns the square or cube of the input value.

### 14. [**freeLinkedList**](https://github.com/abderrazekbhr/tpOS-Securite/blob/f64a8f4a9f631443f3b5283d10f1886b3ab278e2/tp1/question3.c#L223)
   - **Description**: Frees the memory allocated for the entire linked list.
   - **Prototype**: `void freeLinkedList(struct linkedList **head)`
   - **Parameters**:
     - `head`: Double pointer to the first element of the list.
   - **Effect**: Frees the memory for all elements of the list.

### [Main Program Example](https://github.com/abderrazekbhr/tpOS-Securite/blob/f64a8f4a9f631443f3b5283d10f1886b3ab278e2/tp1/question3.c#L235)
The main program demonstrates:
- Creating singly and doubly linked lists.
- Adding, removing, and modifying elements.
- Displaying list contents.
- Concatenating lists.
- Converting a doubly linked list into a circular list.
