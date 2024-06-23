#pragma once

namespace arabica {

class Display {
public:
  ~Display() {
    if (_pixels != nullptr) {
      delete[] _pixels;
    }
  }

  uint32_t* _pixels{nullptr};
  bool      flag{false};
};

} // namespace arabica