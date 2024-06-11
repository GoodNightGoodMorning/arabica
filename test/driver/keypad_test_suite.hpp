#pragma once

#include <arabica/emulator/emulator.hpp>
#include <gtest/gtest.h>

#define arabica_onekey_press_test(test_case_name, test_case_body) \
  TEST(keypad_test_suite, test_case_name) {                       \
    arabica::Emulator emulator;                                   \
    test_case_body                                                \
  }
// ----

#define arabica_allkey_press_test(i)                    \
  arabica_onekey_press_test(test_key##i##_down, {       \
    emulator.keypad.keydown_code = 0x##i;               \
    emulator.memory.write(emulator.cpu.pc, 0xF0);       \
    emulator.memory.write(emulator.cpu.pc + 1, 0x0A);   \
    emulator.cpu.run(emulator.memory, emulator.keypad); \
    ASSERT_EQ(emulator.cpu.registers[0], 0x##i);        \
  });                                                   \
  // ----

arabica_allkey_press_test(0);
arabica_allkey_press_test(1);
arabica_allkey_press_test(2);
arabica_allkey_press_test(3);
arabica_allkey_press_test(4);
arabica_allkey_press_test(5);
arabica_allkey_press_test(6);
arabica_allkey_press_test(7);
arabica_allkey_press_test(8);
arabica_allkey_press_test(9);
arabica_allkey_press_test(a);
arabica_allkey_press_test(b);
arabica_allkey_press_test(c);
arabica_allkey_press_test(d);
arabica_allkey_press_test(e);
arabica_allkey_press_test(f);