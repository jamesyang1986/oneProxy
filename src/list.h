//
// Created by james on 24-9-11.
//

#ifndef TEST_H
#define TEST_H
int size(char *str);

typedef struct ListNode {
    void *data;
    struct ListNode *next;
    struct ListNode *prev;
}ListNode;

typedef struct List {
    int size;
    struct ListNode *head;
    struct ListNode *tail;
}List;

List* createList();
void insertTolistTail(List *list, void  *data);
void insertTolistHead(List *list, void  *data);
void freeList(List *list);
ListNode* searchTolist(List *list, void *data);
void deleteTolist(List *list, void *data);
ListNode* listIndex(List *list, int index);
ListNode* popListTail(List * list);


#endif //TEST_H
