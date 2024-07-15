#pragma once

#include <SDL2/SDL.h>

namespace arabica {

class Keypad {
public:
  void on_keydown(const uint8_t keycode) {
    keys_pressed_status[keycode] = true;
    last_key_pressed_code        = keycode;
  }

  void on_keyup(const uint8_t keycode) {
    keys_pressed_status[keycode] = false;
  }

  bool is_keypressed(const uint8_t keycode) const {
    return keys_pressed_status[keycode];
  }

  int get_last_keypressed_code() const {
    return last_key_pressed_code;
  }

  int last_key_pressed_code = -1;

private:
  std::array<bool, 16> keys_pressed_status{};
};

} // namespace arabica