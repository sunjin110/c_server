#include <stdio.h>

#ifndef C_SERVER_LOGGER
#define C_SERVER_LOGGER

typedef enum _log_mode { LOG_MODE_DEBUG, LOG_MODE_STAGE, LOG_MODE_PRODUCTION } log_mode;

#define DEBUG_PRINT(...) debug_print(__FILE__, __LINE__, __func__, __VA_ARGS__)
#define DEBUG_PRINTLN(...) DEBUG_PRINT(__VA_ARGS__), printf("\n");

#define INFO_PRINT(...) info_print(__FILE__, __LINE__, __func__, __VA_ARGS__)
#define INFO_PRINTLN(...) INFO_PRINT(__VA_ARGS__), printf("\n")

#define WARN_PRINT(...) warn_print(__FILE__, __LINE__, __func__, __VA_ARGS__)
#define WARN_PRINTLN(...) WARN_PRINT(__VA_ARGS__), printf("\n")

#define ERR_PRINT(...) error_print(__FILE__, __LINE__, __func__, __VA_ARGS__)
#define ERR_PRINTLN(...) ERR_PRINT(__VA_ARGS__), printf("\n")

extern void set_log_mode(log_mode m);
extern void debug_print(const char *file, int line, const char *func_name, const char *format, ...);
extern void info_print(const char *file, int line, const char *func_name, const char *format, ...);
extern void warn_print(const char *file, int line, const char *func_name, const char *format, ...);
extern void error_print(const char *file, int line, const char *func_name, const char *format, ...);

#endif
