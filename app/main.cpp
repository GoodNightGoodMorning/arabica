#include <arabica/ui/window.hpp>
#include <arabica/log/log.hpp>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#include <fstream>
#include <sstream>
#endif

std::string rom_file(int argc, char* argv[]) {
#ifdef __EMSCRIPTEN__
  const char* const rom = "/roms/Tetris_Fran_Dachille_1991.ch8";
  return rom;
#else
  if (argc != 2) {
    ARABICA_LOG_INFO("Usage: ./arabica.out rom-file\n");
    std::exit(1);
  }
  return argv[1];
#endif
}

int main(int argc, char* argv[]) {
  arabica::Window window("Arabica Emulator", 640, 320, rom_file(argc, argv));
  window.execute();
  return 0;
}
