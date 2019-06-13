namespace GB {
void CustomizedOutput(const char *filename, const char *funcname, int line,
                      const char *format, ...);
}
#define debug_log(format, ...)                                                 \
  CustomizedOutput(__FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)

#define IS_BIT_SET(data, bit) (((data) & (1 << (bit))) != 0)

#define SET_BIT(data, bit)                                                     \
  do {                                                                         \
    (data) |= (1 << (bit));                                                    \
  } while (0)

#define CLEAR_BIT(data, bit)                                                   \
  do {                                                                         \
    (data) &= ~(1 << (bit));                                                   \
  } while (0)

#define UPtr std::unique_ptr
#define SPtr std::shared_ptr