#include "common.h"
#include <cstdarg>
#include <cstdio>

namespace GB {
void CustomizedOutput(const char *filename, const char *funcname, int line,
                      const char *format, ...) {
  char buffer[4096];
  va_list args;
  va_start(args, format);
  vsnprintf(buffer, sizeof(buffer), format, args);
  va_end(args);
  printf("<%s:%d> (%s) %s\n", filename, line, funcname, buffer);
}
}