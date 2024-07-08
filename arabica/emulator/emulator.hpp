#pragma once

#include <arabica/cpu/cpu.hpp>
#include <arabica/memory/memory.hpp>
#include <arabica/driver/keypad.hpp>
#include <arabica/driver/display.hpp>
#include <arabica/driver/sound.hpp>
#include <arabica/driver/delay.hpp>

namespace arabica {

class Emulator {
public:
  Emulator()
    : cpu(memory) {
  }

  bool load(const std::string& rom);
  void single_step();
  void execute();

  CPU     cpu;
  Memory  memory;
  Keypad  keypad;
  Display display;
  Sound   sound;
  Delay   delay;
};

} // namespace arabica