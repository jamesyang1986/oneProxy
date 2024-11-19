cmake_minimum_required(VERSION 3.22)
project(oneProxy C)

set(CMAKE_C_STANDARD 99)

add_executable(oneProxy src/server.c src/net.c src/ae.c src/ae_kqueue.c
        src/net.c
        src/redisCommand.c
        src/dict.c
        src/list.c
        src/log.c
        src/connection.c src/serverConfig.h src/serverConfig.c src/t_string.h src/t_string.c)
