#include <stdio.h>
#include <stdlib.h>

struct linkedList
{
    int val;
    struct linkedList *next;
};

// 2. Fonction qui renvoie la taille d'une liste.
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

// 3. Fonction qui affiche les éléments d'une liste ainsi que l'adresse.
void displayLinkedList(struct linkedList *head)
{
    while (head != NULL)
    {
        printf("adresse : %p -- valeur : %d\n", (void *)head, head->val);
        head = head->next;
    }
}

// 4. Retirer le premier élément.
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
        fprintf(stderr, "liste vide\n");
    }
}

// 5. Retirer le dernier élément (en supplément, renvoie le dernier élément).
void removeLastElement(struct linkedList **head)
{
    if (*head == NULL)
    {
        fprintf(stderr, "liste vide\n");
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

// 6. Ajouter un élément au début.
void addAtFirstPos(struct linkedList **head, int val)
{
    struct linkedList *newNode = (struct linkedList *)malloc(sizeof(struct linkedList));
    newNode->val = val;
    newNode->next = *head;
    *head = newNode;
}

// 7. Ajouter un élément à la fin.
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

// 8. concatinate two linkedList
void concateneLinkedList(struct linkedList **head1, struct linkedList **head2)
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

// 9. Change the val of element val**2
void changeLinkedListByFunction(struct linkedList *head, int (*f)(int))
{
    while (head != NULL)
    {
        head->val = f(head->val);
        head = head->next;
    }
}
int square(int x)
{
    return x * x;
}
int qube(int x)
{
    return x * x * x;
}



int main()
{
    // 1. Créer une liste chaînée contenant les entiers de 1 à n dans l'ordre croissant.
    int n = 10;
    struct linkedList *head = NULL;
    struct linkedList *lHead = NULL;

    for (int i = 1; i <= n; i++)
    {
        addAtLastPos(&head, i);
    }
    for (int i = 1; i <= n; i++)
    {
        addAtLastPos(&lHead, i * 5);
    }

    printf("Taille de la linkedList: %d\n", sizeLinkedList(head));
    displayLinkedList(head);

    // Retirer le premier élément
    removeFirstElement(&head);
    printf("Taille de la linkedList après effacer premier élément: %d\n", sizeLinkedList(head));
    displayLinkedList(head);

    // Retirer le dernier élément
    removeLastElement(&head);
    printf("Taille de la linkedList après effacer dernier élément: %d\n", sizeLinkedList(head));
    displayLinkedList(head);

    // conacatinate head and lHead
    concateneLinkedList(&head, &lHead);
    printf("Taille de la linkedList après concatination: %d\n", sizeLinkedList(head));
    displayLinkedList(head);

    return 0;
}
