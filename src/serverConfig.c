//
// Created by wan on 2024/11/18.
//

#include <stdlib.h>

#include "serverConfig.h"
#include "log.h"
#include "dict.h"


ProxyConfig *genTestConfig();

ProxyConfig *loadConfig(char *path) {
    Log(LOG_INFO, "start to load  cluster config from:%s", path);
    ProxyConfig *proxyConfig = genTestConfig();
    return proxyConfig;
}

ProxyConfig *genTestConfig() {
    Instance *masterInstance = malloc(sizeof(Instance));
    masterInstance->port = 6379;
    masterInstance->host = "127.0.0.1";
    masterInstance->role = MASTER;
    masterInstance->tmp = "{}";

    Instance *slaveInstance = malloc(sizeof(Instance));
    slaveInstance->port = 6379;
    slaveInstance->host = "127.0.0.1";
    slaveInstance->role = SLAVE;
    slaveInstance->tmp = "{}";


    Cluster *cluster = (Cluster *) malloc(sizeof(Cluster));
    cluster->name = "test-cluster";
    cluster->hashType = 1;
    cluster->type = REDIS;
    cluster->nodeNum = 1;

    cluster->nodes = (Node **) malloc(cluster->nodeNum * sizeof(Node));
    for (int i = 0; i < cluster->nodeNum; i++) {
        Node *node = malloc(sizeof(Node));
        node->nodeName = "node-1";
        node->master = masterInstance;
        node->slave = slaveInstance;
        node->index = 0;
        node->start = 0;
        node->end = 1000000L;

        cluster->nodes[i] = node;
    }


    ProxyConfig *proxyConfig = (ProxyConfig *) malloc(sizeof(ProxyConfig));
    proxyConfig->logFile = NULL;
    proxyConfig->listen = "0.0.0.0";
    proxyConfig->port = 9527;
    proxyConfig->maxClient = 1024;
    proxyConfig->cluster = cluster;
    return proxyConfig;
}

void dumpConfig(char *targetPath, ProxyConfig *config) {


}