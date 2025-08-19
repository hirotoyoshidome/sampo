#include "logger.h"
#include <stdio.h>
#include <time.h>

Logger global_logger;

void init_logger(LoggerPtr logger, char *_log_paht, const LogLevel _level) {
    logger->log_path = _log_paht;
    logger->level = _level;
}

static int write_log(LoggerPtr logger, char *msg, const LogLevel _level) {
    FILE *f = fopen(logger->log_path, "a");
    if (!f) {
        return 0;
    }

    if (logger->level > _level) {
        return 0;
    }

    char *level_str;
    switch (_level) {
    case INFO:
        level_str = "INFO";
        break;
    case WARN:
        level_str = "WARN";
        break;
    case FETAL:
        level_str = "FETAL";
        break;
    }

    // current time(JST)
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info);

    fprintf(f, "[%s] [%s] %s\n", level_str, time_str, msg);

    fclose(f);

    return 1;
}

void fetal(LoggerPtr logger, char *msg) {
    int res = write_log(logger, msg, FETAL);
    if (res != 1) {
        return;
    }
}

void warn(LoggerPtr logger, char *msg) {
    int res = write_log(logger, msg, WARN);
    if (res != 1) {
        return;
    };
}

void info(LoggerPtr logger, char *msg) {
    int res = write_log(logger, msg, INFO);
    if (res != 1) {
        return;
    }
}
