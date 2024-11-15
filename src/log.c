#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdarg.h>


#include "log.h"
#include "server.h"


extern Server *server;

void serverLogRaw(int level, const char *msg) {
    const int syslogLevelMap[] = { LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR };
    const char *c = ".-*#";
    FILE *fp;
    char buf[64];
    int rawmode = (level);
    int log_to_stdout = (server->logfile == NULL);

    level &= 0xff; /* clear flags */
    if (level < server->verbosity) return;

    fp = log_to_stdout ? stdout : fopen(server->logfile,"a");
    if (!fp) return;

    if (rawmode) {
        fprintf(fp,"%s",msg);
    } else {
        int off;
        struct timeval tv;
        int role_char;
        pid_t pid = getpid();

        gettimeofday(&tv,NULL);
        off = strftime(buf,sizeof(buf),"%d %b %H:%M:%S.",localtime(&tv.tv_sec));
        snprintf(buf+off,sizeof(buf)-off,"%03d",(int)tv.tv_usec/1000);

        fprintf(fp,"%d:%c %s %c %s\n",
            (int)getpid(),role_char, buf,c[level],msg);
    }
    fflush(fp);

    if (!log_to_stdout) fclose(fp);
}


void Log(int level, const char *fmt, ...) {
    va_list ap;
    char msg[LOG_MAX_LEN];

    if (level < server->verbosity) return;

    va_start(ap, fmt);
    vsnprintf(msg, sizeof(msg), fmt, ap);
    va_end(ap);

    serverLogRaw(level,msg);
}


