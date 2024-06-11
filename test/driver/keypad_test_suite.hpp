#pragma once

#include <arabica/emulator/emulator.hpp>
#include <gtest/gtest.h>

#define arabica_keypad_test(test_case_name, test_case_body) \
  TEST(keypad_test_suite, test_case_name) {                 \
    arabica::Emulator emulator;                             \
    test_case_body                                          \
  }
// ----

// clang-format off

arabica_keypad_test(test_keya_down,
  emulator.cpu.irq = true;
  emulator.keypad.keydown_code = 0xA;
  emulator.memory.write(emulator.cpu.pc, 0xF0);
  emulator.memory.write(emulator.cpu.pc + 1, 0x0A);
  emulator.cpu.run(emulator.memory, emulator.keypad);
  ASSERT_EQ(emulator.cpu.registers[0], 10);
)

arabica_keypad_test(test_key4_down,
  emulator.cpu.irq = true;
  emulator.keypad.keydown_code = 0x4;
  emulator.memory.write(emulator.cpu.pc, 0xF0);
  emulator.memory.write(emulator.cpu.pc + 1, 0x0A);
  emulator.cpu.run(emulator.memory, emulator.keypad);
  ASSERT_EQ(emulator.cpu.registers[0], 4);
)