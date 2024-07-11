#pragma once

#include <arabica/emulator/emulator.hpp>
#include <gtest/gtest.h>

#define arabica_memory_test(test_case, address, value) \
  TEST(memory_test_suite, test_case) {                 \
    arabica::Emulator emulator;                        \
    write_and_assert(address, value);                  \
  }

void write_and_assert(const arabica::Memory::address_t address, const arabica::Memory::value_t value) {
  arabica::Memory memory;
  memory.write(address, value);
  ASSERT_EQ(memory.read(address), value);
}

arabica_memory_test(pc_write_001, emulator.cpu.pc, 0x61);
arabica_memory_test(pc_write_002, emulator.cpu.pc, 0x00);

arabica_memory_test(memory_write_001, 0x300, 0x11);