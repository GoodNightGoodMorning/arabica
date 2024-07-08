#include <arabica/emulator/emulator.hpp>
#include <cstdint>
#include <cstdlib>
#include <vector>
#include <random>
#include <fmt/core.h>

namespace arabica {

template<typename T>
inline T random(T range_from, T range_to) {
  std::random_device               rand_dev;
  std::mt19937                     generator(rand_dev());
  std::uniform_int_distribution<T> distr(range_from, range_to);
  return distr(generator);
}

bool Emulator::load(const std::string& rom) {
  return memory.load(rom);
}

int cycle = 0;

void Emulator::execute() {
  fmt::print("[emulator log] PC = {:x}\n", cpu.pc);
  single_step();

  fmt::print("[emulator log] The current cycle is {}\n", cycle);
  cycle++;
}

void Emulator::single_step() {
  cpu.instruction = memory[cpu.pc] << 8 | memory[cpu.pc + 1];
  uint16_t prefix = cpu.instruction & 0xF000;
  cpu.opcode      = static_cast<OP_CODE>(prefix);

  fmt::print("[cpu log] cpu.instruction is {0:x}\n", cpu.instruction);

  switch (prefix) {
    case 0x0: {
      switch (cpu.instruction & 0x00FF) {
        case 0xE0: cpu.opcode = OP_CODE::CLS; break;
        case 0xEE: cpu.opcode = OP_CODE::RET; break;
        default: break;
      }
    } break;
    case 0x8000: {
      switch (cpu.instruction & 0x000F) {
        case 0x0: cpu.opcode = OP_CODE::LD_Vx_Vy; break;
        case 0x1: cpu.opcode = OP_CODE::OR_Vx_Vy; break;
        case 0x2: cpu.opcode = OP_CODE::AND_Vx_Vy; break;
        case 0x3: cpu.opcode = OP_CODE::XOR_Vx_Vy; break;
        case 0x4: cpu.opcode = OP_CODE::ADD_Vx_Vy; break;
        case 0x5: cpu.opcode = OP_CODE::SUB_Vx_Vy; break;
        case 0x6: cpu.opcode = OP_CODE::SHR_Vx; break;
        case 0x7: cpu.opcode = OP_CODE::SUBN_Vx_Vy; break;
        case 0xE: cpu.opcode = OP_CODE::SHL_Vx; break;
        default: break;
      }
    } break;
    case 0xE000: {
      switch (cpu.instruction & 0x00FF) {
        case 0x9E: cpu.opcode = OP_CODE::SKP_Vx; break;
        case 0xA1: cpu.opcode = OP_CODE::SKNP_Vx; break;
        default: break;
      }
    }
    case 0xF000: {
      switch (cpu.instruction & 0x00FF) {
        case 0x07: cpu.opcode = OP_CODE::LD_Vx_DT; break;
        case 0x0A: cpu.opcode = OP_CODE::LD_Vx_K; break;
        case 0x15: cpu.opcode = OP_CODE::LD_DT_Vx; break;
        case 0x18: cpu.opcode = OP_CODE::LD_ST_Vx; break;
        case 0x1E: cpu.opcode = OP_CODE::ADD_I_Vx; break;
        case 0x29: cpu.opcode = OP_CODE::LD_F_Vx; break;
        case 0x33: cpu.opcode = OP_CODE::LD_B_Vx; break;
        case 0x55: cpu.opcode = OP_CODE::LD_I_Vx; break;
        case 0x65: cpu.opcode = OP_CODE::LD_Vx_I; break;
        default: break;
      }
    } break;
    default: break;
  }

  switch (cpu.opcode) {
    case OP_CODE::SYS_addr: {
      uint16_t target = cpu.instruction & 0x0FFF;
      cpu.pc          = target;
    } break;
    case OP_CODE::CLS: {
      display.clear();
      display.flag       = true;
      cpu.registers[0xF] = 0;
      cpu.advance_pc();
    } break;
    case OP_CODE::JP_addr: {
      uint16_t target = cpu.instruction & 0x0FFF;
      cpu.pc          = target;
    } break;
    case OP_CODE::CALL_addr: {
      cpu.stack.push(cpu.pc + 2);
      uint16_t target = cpu.instruction & 0x0FFF;
      cpu.pc          = target;
    } break;
    case OP_CODE::RET: {
      if (!cpu.stack.empty()) {
        cpu.pc = cpu.stack.top();
        cpu.stack.pop();
      }
    } break;
    case OP_CODE::SE_Vx_byte: {
      uint8_t x      = (cpu.instruction & 0x0F00) >> 8;
      uint8_t byte_v = cpu.instruction & 0x00FF;

      if (cpu.registers[x] == byte_v) {
        cpu.advance_pc();
      }
      cpu.advance_pc();
    } break;
    case OP_CODE::SNE_Vx_byte: {
      uint8_t x      = (cpu.instruction & 0x0F00) >> 8;
      uint8_t byte_v = cpu.instruction & 0x00FF;

      if (cpu.registers[x] != byte_v) {
        cpu.advance_pc();
      }
      cpu.advance_pc();
    } break;
    case OP_CODE::SE_Vx_Vy: {
      uint8_t x = (cpu.instruction & 0x0F00) >> 8;
      uint8_t y = (cpu.instruction & 0x00F0) >> 4;

      if (cpu.registers[x] == cpu.registers[y]) {
        cpu.advance_pc();
      }
      cpu.advance_pc();
    } break;
    case OP_CODE::LD_Vx_byte: {
      uint8_t byte_v                                 = cpu.instruction & 0x00FF;
      cpu.registers[(cpu.instruction & 0x0F00) >> 8] = byte_v;
      cpu.advance_pc();
    } break;
    case OP_CODE::ADD_Vx_byte: {
      uint8_t byte_v = cpu.instruction & 0x00FF;
      uint8_t x      = (cpu.instruction & 0x0F00) >> 8;

      cpu.registers[x] = cpu.registers[x] + byte_v; // it will be wrapped if overflow occurs
      cpu.advance_pc();
    } break;
    case OP_CODE::LD_Vx_Vy: {
      uint8_t x = (cpu.instruction & 0x0F00) >> 8;
      uint8_t y = (cpu.instruction & 0x00F0) >> 4;

      cpu.registers[x] = cpu.registers[y];
      cpu.advance_pc();
    } break;
    case OP_CODE::OR_Vx_Vy: {
      uint8_t x = (cpu.instruction & 0x0F00) >> 8;
      uint8_t y = (cpu.instruction & 0x00F0) >> 4;

      cpu.registers[x] |= cpu.registers[y];
      cpu.advance_pc();
    } break;
    case OP_CODE::AND_Vx_Vy: {
      uint8_t x = (cpu.instruction & 0x0F00) >> 8;
      uint8_t y = (cpu.instruction & 0x00F0) >> 4;

      cpu.registers[x] &= cpu.registers[y];
      cpu.advance_pc();
    } break;
    case OP_CODE::XOR_Vx_Vy: {
      uint8_t x = (cpu.instruction & 0x0F00) >> 8;
      uint8_t y = (cpu.instruction & 0x00F0) >> 4;

      cpu.registers[x] ^= cpu.registers[y];
      cpu.advance_pc();
    } break;
    case OP_CODE::ADD_Vx_Vy: {
      uint8_t x = (cpu.instruction & 0x0F00) >> 8;
      uint8_t y = (cpu.instruction & 0x00F0) >> 4;

      cpu.registers[x]   = cpu.registers[x] + cpu.registers[y];
      cpu.registers[0xF] = cpu.registers[y] > 255 - cpu.registers[x];

      cpu.advance_pc();
    } break;
    case OP_CODE::SUB_Vx_Vy: {
      uint8_t x = (cpu.instruction & 0x0F00) >> 8;
      uint8_t y = (cpu.instruction & 0x00F0) >> 4;

      cpu.registers[0xF] = cpu.registers[x] > cpu.registers[y];
      cpu.registers[x]   = cpu.registers[x] - cpu.registers[y];

      cpu.advance_pc();
    } break;
    case OP_CODE::SHR_Vx: {
      // Remark: historically, the semantics is "right shift V[x] by V[y] amount
      // and store the result to V[x]" in the original chip8 implementation, however, most of the game
      // after 90s follows the buggy implementation of HP which ignoring V[y], so we just follow the same for now.
      uint8_t x = (cpu.instruction & 0x0F00) >> 8;

      cpu.registers[0xF] = cpu.registers[x] & 1;
      cpu.registers[x]   = cpu.registers[x] >> 1;

      cpu.advance_pc();
    } break;
    case OP_CODE::SUBN_Vx_Vy: {
      uint8_t x = (cpu.instruction & 0x0F00) >> 8;
      uint8_t y = (cpu.instruction & 0x00F0) >> 4;

      cpu.registers[0xF] = cpu.registers[y] > cpu.registers[x];
      cpu.registers[x]   = cpu.registers[y] - cpu.registers[x];

      cpu.advance_pc();
    } break;
    case OP_CODE::SHL_Vx: {
      // Remark: historically, the semantics is "left shift V[x] by V[y] amount
      // and store the result to V[x]" in the original chip8 implementation, however, most of the game
      // after 90s follows the buggy implementation of HP which ignoring V[y], so we just follow the same for now.
      uint8_t x = (cpu.instruction & 0x0F00) >> 8;

      cpu.registers[0xF] = (cpu.registers[x] >> 7) & 1;
      cpu.registers[x]   = cpu.registers[x] << 1;

      cpu.advance_pc();
    } break;
    case OP_CODE::SNE_Vx_Vy: {
      uint8_t x = (cpu.instruction & 0x0F00) >> 8;
      uint8_t y = (cpu.instruction & 0x00F0) >> 4;

      if (cpu.registers[x] != cpu.registers[y]) {
        cpu.advance_pc();
      }
      cpu.advance_pc();
    } break;
    case OP_CODE::LD_I_addr: {
      uint16_t data = (cpu.instruction & 0x0FFF);
      cpu.reg_I     = data;
      cpu.advance_pc();
    } break;
    case OP_CODE::JP_V0_addr: {
      uint16_t base_address = (cpu.instruction & 0x0FFF);
      cpu.pc                = base_address;
      cpu.advance_pc(cpu.registers[0]);
    } break;
    case OP_CODE::RND_Vx_byte: {
      uint8_t x         = (cpu.instruction & 0x0F00) >> 8;
      uint8_t kk        = (cpu.instruction & 0x00FF);
      uint8_t rand_byte = random(0, 255);

      cpu.registers[x] = rand_byte & kk;

      cpu.advance_pc();
    } break;
    case OP_CODE::LD_Vx_DT: {
      uint8_t x        = (cpu.instruction & 0x0F00) >> 8;
      cpu.registers[x] = delay.get();
      cpu.advance_pc();
    } break;
    case OP_CODE::LD_DT_Vx: {
      uint8_t x = (cpu.instruction & 0x0F00) >> 8;
      delay.set(cpu.registers[x]);
      cpu.advance_pc();
    } break;
    case OP_CODE::LD_ST_Vx: {
      uint8_t x     = (cpu.instruction & 0x0F00) >> 8;
      cpu.reg_sound = cpu.registers[x];
      cpu.advance_pc();
    } break;
    case OP_CODE::ADD_I_Vx: {
      uint8_t x = (cpu.instruction & 0x0F00) >> 8;
      cpu.reg_I = cpu.reg_I + cpu.registers[x];
      cpu.advance_pc();
    } break;
    case OP_CODE::LD_Vx_K: {
      uint8_t x = (cpu.instruction & 0x0F00) >> 8;

      if (keypad.keydown_code != -1) {
        fmt::print("[cpu log] pressed key is {}\n", keypad.keydown_code);
        cpu.registers[x]    = keypad.keydown_code;
        keypad.keydown_code = -1;
        fmt::print("[cpu log] reg{} is {}\n", x, cpu.registers[x]);
        cpu.advance_pc();
      }
    } break;
    case OP_CODE::SKP_Vx: {
      uint8_t x = (cpu.instruction & 0x0F00) >> 8;

      if (keypad.keydown_code == cpu.registers[x]) {
        cpu.advance_pc();
      }
      cpu.advance_pc();
    } break;
    case OP_CODE::SKNP_Vx: {
      uint8_t x = (cpu.instruction & 0x0F00) >> 8;

      if (keypad.keydown_code != cpu.registers[x]) {
        cpu.advance_pc();
      }
      cpu.advance_pc();
    } break;
    case OP_CODE::DRW_Vx_Vy_nibble: {
      uint8_t vx     = (cpu.instruction & 0x0F00) >> 8;
      uint8_t vy     = (cpu.instruction & 0x00F0) >> 4;
      uint8_t nibble = cpu.instruction & 0x000F;

      std::vector<uint8_t> sprite_data;
      for (int i = 0; i < nibble; ++i) {
        sprite_data.push_back(memory.read(cpu.reg_I + i));
      }

      int vertical_offset = display.scale * (display.height - nibble) / 5;
      for (int y = 0; y < nibble; ++y) {
        uint8_t sprite_row = sprite_data[y];
        for (int x = 0; x < 8; ++x) {
          uint8_t pixel_value = (sprite_row >> (7 - x)) & 0x01;
          int     screen_x    = (cpu.registers[vx] + x) % display.width;
          int     screen_y    = (cpu.registers[vy] + y) % display.height;

          cpu.registers[0xF] = 0;
          if (pixel_value == 1) {
            if (display.get_pixel(screen_x * display.scale, (screen_y * display.scale) + vertical_offset)) {
              cpu.registers[0xF] = 1;
            }
            for (int dy = 0; dy < display.scale; ++dy) {
              for (int dx = 0; dx < display.scale; ++dx) {
                const auto cx    = screen_x * display.scale + dx;
                const auto cy    = screen_y * display.scale + dy + vertical_offset;
                const auto pixel = display.get_pixel(cx, cy);
                display.set_pixel(cx, cy, pixel != 0 ? pixel : 255);
              }
            }
          }
        }
      }
      display.flag = true;
      fmt::print("[cpu log] display flag = {}.\n", display.flag);
      cpu.advance_pc();
    } break;
    case OP_CODE::LD_F_Vx: {
      uint8_t vx = (cpu.instruction & 0x0F00) >> 8;
      cpu.reg_I  = cpu.registers[vx] * 0x5;
      cpu.advance_pc();
    } break;
    case OP_CODE::LD_B_Vx: {
      const uint8_t vx = (cpu.instruction & 0x0F00) >> 8;

      const auto& rx        = cpu.registers[vx];
      memory[cpu.reg_I + 0] = (rx % 1000) / 100;
      memory[cpu.reg_I + 1] = (rx % 100) / 10;
      memory[cpu.reg_I + 2] = (rx % 10);
      cpu.advance_pc();
    } break;
    case OP_CODE::LD_I_Vx: {
      const uint8_t vx = (cpu.instruction & 0x0F00) >> 8;

      for (int i = 0; i <= vx; i++) {
        memory[cpu.reg_I + i] = cpu.registers[i];
      }
      cpu.reg_I += vx + 1;
      cpu.advance_pc();
    } break;
    case OP_CODE::LD_Vx_I: {
      const uint8_t vx = (cpu.instruction & 0x0F00) >> 8;

      for (int i = 0; i <= vx; i++) {
        cpu.registers[i] = memory[cpu.reg_I + i];
      }
      cpu.reg_I += vx + 1;
      cpu.advance_pc();
    } break;
    default: {
      fmt::print("[cpu log] Unknown opcode: 0x{:X}\n", static_cast<uint16_t>(cpu.opcode));
    } break;
  }
}

} // namespace arabica
