//
// Created by wan on 2024/11/19.
//

#include <stdio.h>
#include <stdlib.h>

#include "t_string.h"


void getCommand(redisClient *c) {
    int args = c->argc;
    char **argv = c->argv;
    if (args < 2) {
        printf("wrong number of arguments\n");
        sendERR(c, "ERR");
    }
    dict *db = c->curDb;
    char *key = argv[1];

    dictEntry *entry = dictFind(db, key);

    if (entry == NULL) {
        sendNil(c);
        return;
    }
    void *data = entry->v.val;
    sendBulkStr(c, data);
}

void setCommand(redisClient *c) {
    int args = c->argc;
    char **argv = c->argv;
    if (args <= 2) {
        printf("wrong number of arguments\n");
        sendERR(c, "ERR");
    }
    dict *db = c->curDb;
    char *key = argv[1];
    char *value = argv[2];
    dictAdd(db, key, value);
    sendStr(c, "OK");
}

void expireCommand(redisClient *c) {
    int args = c->argc;
    char **argv = c->argv;

}

void delCommand(redisClient *c) {
    int args = c->argc;
    char **argv = c->argv;

}