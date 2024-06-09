#include <arabica/cpu/cpu.hpp>
#include <fmt/core.h>

namespace arabica {

inline static constexpr void advance_pc(uint16_t& pc, uint16_t offset = 2) {
  pc += offset;
}

void CPU::run(const Memory& memory) {
  instruction     = memory[pc] << 8 | memory[pc + 1];
  uint16_t prefix = instruction & 0xF000;
  opcode          = static_cast<OP_CODE>(prefix);

  switch (prefix) {
    case 0x0: {
      switch (instruction & 0x00FF) {
        case 0xEE: opcode = OP_CODE::RET; break;
        default: break;
      }
    } break;
    case 0x8000: {
      switch (instruction & 0x000F) {
        case 0x0: opcode = OP_CODE::LD_Vx_Vy; break;
        case 0x1: opcode = OP_CODE::OR_Vx_Vy; break;
        case 0x2: opcode = OP_CODE::AND_Vx_Vy; break;
        case 0x3: opcode = OP_CODE::XOR_Vx_Vy; break;
        case 0x4: opcode = OP_CODE::ADD_Vx_Vy; break;
        case 0x5: opcode = OP_CODE::SUB_Vx_Vy; break;
        case 0x6: opcode = OP_CODE::SHR_Vx; break;
        case 0x7: opcode = OP_CODE::SUBN_Vx_Vy; break;
        case 0xE: opcode = OP_CODE::SHL_Vx; break;
        default: break;
      }
    } break;
    default: break;
  }

  switch (opcode) {
    case OP_CODE::JP_addr: {
      uint16_t target = instruction & 0x0FFF;
      pc              = target;
    } break;
    case OP_CODE::CALL_addr: {
      stack.push(pc + 2);
      uint16_t target = instruction & 0x0FFF;
      pc              = target;
    } break;
    case OP_CODE::RET: {
      if (!stack.empty()) {
        pc = stack.top();
        stack.pop();
      } else {
        // ToDo: raise interrupt? how to test the failed case?
      };
    } break;
    case OP_CODE::LD_Vx_byte: {
      uint8_t byte_v                         = instruction & 0x00FF;
      registers[(instruction & 0x0F00) >> 8] = byte_v;

      advance_pc(pc);
    } break;
    case OP_CODE::ADD_Vx_byte: {
      uint8_t byte_v = instruction & 0x00FF;
      uint8_t x      = (instruction & 0x0F00) >> 8;
      registers[x]   = registers[x] + byte_v; // it will be wrapped if overflow occurs

      advance_pc(pc);
    } break;
    case OP_CODE::LD_Vx_Vy: {
      uint8_t x    = (instruction & 0x0F00) >> 8;
      uint8_t y    = (instruction & 0x00F0) >> 4;
      registers[x] = registers[y];

      advance_pc(pc);
    } break;
    case OP_CODE::OR_Vx_Vy: {
      uint8_t x = (instruction & 0x0F00) >> 8;
      uint8_t y = (instruction & 0x00F0) >> 4;
      registers[x] |= registers[y];

      advance_pc(pc);
    } break;
    case OP_CODE::AND_Vx_Vy: {
      uint8_t x = (instruction & 0x0F00) >> 8;
      uint8_t y = (instruction & 0x00F0) >> 4;
      registers[x] &= registers[y];

      advance_pc(pc);
    } break;
    case OP_CODE::XOR_Vx_Vy: {
      uint8_t x = (instruction & 0x0F00) >> 8;
      uint8_t y = (instruction & 0x00F0) >> 4;
      registers[x] ^= registers[y];

      advance_pc(pc);
    } break;
    case OP_CODE::ADD_Vx_Vy: {
      uint8_t x      = (instruction & 0x0F00) >> 8;
      uint8_t y      = (instruction & 0x00F0) >> 4;
      registers[x]   = registers[x] + registers[y];
      registers[0xF] = registers[y] > 255 - registers[x];

      advance_pc(pc);
    } break;
    case OP_CODE::SUB_Vx_Vy: {
      uint8_t x = (instruction & 0x0F00) >> 8;
      uint8_t y = (instruction & 0x00F0) >> 4;

      registers[0xF] = registers[x] > registers[y];
      registers[x]   = registers[x] - registers[y];

      advance_pc(pc);
    } break;
    case OP_CODE::SHR_Vx: {
      uint8_t x      = (instruction & 0x0F00) >> 8;
      registers[0xF] = registers[x] & 1;
      registers[x]   = registers[x] >> 1;

      advance_pc(pc);
    } break;
    case OP_CODE::SUBN_Vx_Vy: {
      uint8_t x = (instruction & 0x0F00) >> 8;
      uint8_t y = (instruction & 0x00F0) >> 4;

      registers[0xF] = registers[y] > registers[x];
      registers[x]   = registers[y] - registers[x];

      advance_pc(pc);
    } break;
    case OP_CODE::SHL_Vx: {
      uint8_t x      = (instruction & 0x0F00) >> 8;
      registers[0xF] = (registers[x] >> 7) & 1;
      registers[x]   = registers[x] << 1;

      advance_pc(pc);
    } break;
    default: {
      fmt::print("Unknown opcode: 0x{:X}\n", static_cast<uint16_t>(opcode));
    } break;
  }
}

} // namespace arabica
