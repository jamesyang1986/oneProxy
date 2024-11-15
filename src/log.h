//
// Created by james on 24-11-15.
//

#ifndef LOG_H
#define LOG_H


#define LOG_MAX_LEN  1024

#define LOG_DEBUG 0
#define LOG_INFO 1
#define LOG_WARN 2
#define LOG_ERROR 3

void Log(int level, const char *fmt, ...);

#endif //LOG_H
