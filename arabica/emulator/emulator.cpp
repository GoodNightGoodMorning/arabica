#include <arabica/emulator/emulator.hpp>

namespace arabica {

void Emulator::run() {
  memory.write(cpu.pc + 0, 0xF0);
  memory.write(cpu.pc + 1, 0x0A);
  memory.write(cpu.pc + 2, 0xD0);
  memory.write(cpu.pc + 3, 0x14);

  cpu.run(memory, keypad, display);
  cpu.run(memory, keypad, display);

  cpu.reset();
}

} // namespace arabica