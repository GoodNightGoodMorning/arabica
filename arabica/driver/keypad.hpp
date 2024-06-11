#pragma once

#include <arabica/type/noncopyable.hpp>
#include <SDL2/SDL.h>
#include <fmt/core.h>
#include <string_view>

namespace arabica {

class Keypad : public noncopyable {
public:
  Keypad()  = default;
  ~Keypad() = default;

  Keypad(Keypad&&)            = default;
  Keypad& operator=(Keypad&&) = default;

  static int keydown_event(void* data);

  static int keydown_code;
};

} // namespace arabica