#ifndef SAMPO_LOGGER_H
#define SAMPO_LOGGER_H

typedef enum { INFO, WARN, FETAL } LogLevel;

typedef struct {
    char *log_path;
    LogLevel level;
} Logger;

typedef Logger *LoggerPtr;

void init_logger(LoggerPtr logger, char *_log_paht, const LogLevel _level);

void fetal(LoggerPtr logger, char *msg);
void warn(LoggerPtr logger, char *msg);
void info(LoggerPtr logger, char *msg);

extern Logger global_logger;

#endif // SAMPO_LOGGER_H
