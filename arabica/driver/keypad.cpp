#include <arabica/driver/keypad.hpp>

namespace arabica {

volatile int Keypad::keydown_code{-1};

int Keypad::keydown_event(void* data) {
  if (data == nullptr)
    return -1;

  auto* irq = reinterpret_cast<bool*>(data);
  *irq      = true;
  return 0;
}

} // namespace arabica