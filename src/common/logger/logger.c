
#include "logger.h"

#include <stdarg.h>

// threadsafeではないので、書き込みは起動時に1回のみにすること
static log_mode mode;

extern void set_log_mode(log_mode m) { mode = m; }

extern void debug_print(const char *file, int line, const char *func_name, const char *format, ...) {
  if (mode != LOG_MODE_DEBUG) {
    return;
  }

  va_list args;
  va_start(args, format);
  printf("[DEBUG] %s:%d (%s):", file, line, func_name);
  vprintf(format, args);
  va_end(args);
}

extern void info_print(const char *file, int line, const char *func_name, const char *format, ...) {
  if (mode != LOG_MODE_DEBUG && mode != LOG_MODE_STAGE) {
    return;
  }

  va_list args;
  va_start(args, format);
  printf("[INFO] %s:%d (%s):", file, line, func_name);
  vprintf(format, args);
  va_end(args);
}

extern void warn_print(const char *file, int line, const char *func_name, const char *format, ...) {
  va_list args;
  va_start(args, format);
  printf("[WARN] %s:%d (%s):", file, line, func_name);
  vprintf(format, args);
  va_end(args);
}

extern void error_print(const char *file, int line, const char *func_name, const char *format, ...) {
  va_list args;
  va_start(args, format);
  printf("[ERROR] %s:%d (%s):", file, line, func_name);
  vprintf(format, args);
  va_end(args);
}
