//
// Created by wan on 2024/11/19.
//

#ifndef ONEPROXY_T_STRING_H
#define ONEPROXY_T_STRING_H
#include "redisCommand.h"

void getCommand(redisClient *c);

void setCommand(redisClient *c);

void expireCommand(redisClient *c);

void delCommand(redisClient *c);

#endif //ONEPROXY_T_STRING_H
