#include <arabica/ui/window.hpp>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#include <fstream>
#include <sstream>
#else
#include <fmt/core.h>
#endif

#ifdef __EMSCRIPTEN__
std::string read_file_from_browser(const std::string& file_name) {
  std::ifstream file(file_name);
  if (!file.is_open()) {
    // fmt::print("Failed to open file: {}\n", file_name);
    std::exit(1);
  }
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}
#endif

int main(int argc, char* argv[]) {
#ifdef __EMSCRIPTEN__
  std::string rom_file = "/roms/Tetris_Fran_Dachille_1991.ch8";
#else
  if (argc != 2) {
    // fmt::print("Usage: ./arabica.out rom-file\n");
    return 1;
  }
  std::string rom_file = argv[1];
#endif
  arabica::Window window("Arabica Emulator", 640, 320, rom_file);
  window.execute();
  return 0;
}
