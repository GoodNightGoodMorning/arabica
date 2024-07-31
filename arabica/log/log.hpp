#pragma once

#define ARABICA_IS_ENABLE_LOG 0

#ifdef __EMSCRIPTEN__
#else
#include <fmt/core.h>
#endif

#if ARABICA_IS_ENABLE_LOG
#define ARABICA_LOG_INFO(fmt, ...) arabica::log_info(fmt, __VA_ARGS__)
#else
#define ARABICA_LOG_INFO(fmt, ...)
#endif

namespace arabica {

template<typename... Args>
inline void log_info(const char* const format, const Args&... args) {
#ifdef __EMSCRIPTEN__
#else
  fmt::print("[emulator log] ");
  fmt::print(format, args...);
#endif
}

} // namespace arabica