#pragma once

#include <SDL2/SDL.h>
#include <fmt/core.h>

namespace arabica {

class Keypad {
public:
  static int keydown_code;
};

inline int Keypad::keydown_code{-1};

} // namespace arabica