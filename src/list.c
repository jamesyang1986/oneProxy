//
// Created by james on 24-9-11.
//
#include <stdio.h>
#include "list.h"

#include <stdlib.h>

ListNode *createListNode(void *data) {
    ListNode *node = (ListNode *) malloc(sizeof(ListNode));
    node->data = data;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

List *createList() {
    List *list = (List *) malloc(sizeof(List));
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
    return list;
}

void insertTolistTail(List *list, void *data) {
    if (list == NULL)
        return;
    ListNode *node = createListNode(data);
    if (list->tail == NULL) {
        list->head = list->tail = node;
    } else {
        list->tail->next = node;
        node->prev = list->tail;
        list->tail = node;
    }

    list->size++;
}

void insertTolistHead(List *list, void *data) {
    if (list == NULL)
        return;
    ListNode *node = createListNode(data);
    if (list->head == NULL) {
        list->head = list->tail = node;
    } else {
        list->head->next = node;
        node->prev = list->head;
        list->head = node;
    }
    list->size++;
}

void freeList(List *list) {
    if (list == NULL)
        return;
    ListNode *node = list->head;
    while (node != NULL) {
        ListNode *next = node->next;
        free(node);
        node = next;
    }
    free(list);
}

ListNode *searchTolist(List *list, void *data) {
    if (list == NULL)
        return NULL;
    ListNode *node = list->head;
    while (node != NULL) {
        if (node->data == data)
            return node;
        node = node->next;
    }
    return NULL;
}

void deleteTolist(List *list, void *data) {
    if (list == NULL)
        return;
    ListNode *node = list->head;
    while (node != NULL) {
        if (node->data == data) {
            ListNode *next = node->next;
            node->prev->next = next;
            next->next->prev = node->prev;
            list->size--;
            free(node);
            return;
        }
        node = node->next;
    }
}


ListNode *listIndex(List *list, int index) {
    if (list == NULL)
        return NULL;
    ListNode *node = list->head;
    while (node != NULL && index--) {
        node = node->next;
    }
    return node;
}

ListNode *popListTail(List *list) {
    if (list == NULL || list->size == 0)
        return NULL;
    ListNode *node = list->tail;
    if(node->prev == NULL) {
        list->head = list->tail = NULL;
        list->size--;
        return node;
    }

    node->prev->next = NULL;
    list->tail = node->prev;
    list->size--;
    return node;
}
