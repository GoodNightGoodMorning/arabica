#include <arabica/emulator/emulator.hpp>
#include <iostream>
#include <fmt/core.h>

namespace arabica {

bool Emulator::load(const std::string& rom) {
  return memory.load(rom);
}

int cycle = 0;

void Emulator::run() {
  fmt::print("[emulator log] PC = {:x}\n", cpu.pc);
  cpu.run(memory, keypad, display, delay);

  fmt::print("[emulator log] The current cycle is {}\n", cycle);
  cycle++;
}

} // namespace arabica
