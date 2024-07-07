#pragma once

namespace arabica {

class Display {
public:
  ~Display() {
    if (pixels != nullptr) {
      delete[] pixels;
    }
  }

  void init(int w, int h, int s) {
    width  = w;
    height = h;
    scale  = s;
    pixels = new uint32_t[(width * scale) * (height * scale)];
    clear();
  }

  void set_pixel(int x, int y, uint32_t color) {
    if (x >= 0 && x < width * scale && y >= 0 && y < height * scale) {
      pixels[y * width * scale + x] = color;
    }
  }

  uint32_t get_pixel(int x, int y) const {
    if (x >= 0 && x < width * scale && y >= 0 && y < height * scale) {
      return pixels[y * width * scale + x];
    }
    return 0;
  }

  void clear() {
    for (int i = 0; i < width * scale * height * scale; ++i) {
      pixels[i] = 0;
    }
  }

  uint32_t* pixels{nullptr};
  int       width{0};
  int       height{0};
  int       scale{1};
  bool      flag{false};
};

} // namespace arabica