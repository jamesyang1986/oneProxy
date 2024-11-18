//
// Created by wan on 2024/11/18.
//

#ifndef SERVERCONFIG_H
#define SERVERCONFIG_H

#include <time.h>

enum {
    REDIS = 0, MYSQL = 1
};

enum {
    MASTER = 1, SLAVE = 2
};

typedef struct Instance {
    char *host;
    int port;
    int role;
    int status;
    char *tmp;
} Instance;

typedef struct Node {
    char *nodeName;
    Instance *master;
    Instance *slave;
    int readStrategy;
    int writeStrategy;
    int slaveNum;
    int index;
    long start;
    long end;
} Node;

typedef struct Cluster {
    char *name;
    int type;
    int hashType;
    Node **nodes;
    int nodeNum;
} Cluster;

typedef struct ProxyConfig {
    char *logFile;
    char *listen;
    int port;
    int type;
    Cluster *cluster;
    int maxClient;
} ProxyConfig;

ProxyConfig *loadConfig(char *path);

void dumpConfig(char *targetPath, ProxyConfig *config);

#endif //SERVERCONFIG_H
