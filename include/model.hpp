/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef __MODEL_HPP__
#define __MODEL_HPP__

/* standard c++ includes */
#include <array>
#include <cstdint>
#include <map>
#include <string>
#include <string_view>
#include <vector>

/* project specific c++ includes */
#include "utils.hpp"

#define MEMORY_SIZE 65536

// =============================================================================
//                       Memory Class for 8085 RAM
// =============================================================================
class Memory {
 public:
    uint8_t get_memory(uint16_t address);
    void set_memory(uint16_t address, uint8_t value);

 private:
    std::array<uint8_t, MEMORY_SIZE> __memory;
};

// =============================================================================
//                       CpuRegister Class for 8085 registers
// =============================================================================
class CpuRegisters {
 public:
    explicit CpuRegisters(Memory& memory);

    uint8_t accumulator() const;
    void set_accumulator(uint8_t value);
    uint8_t get_register(const std::string& register_name) const;
    void set_register(const std::string& register_name, uint8_t value);
    std::vector<uint8_t> get_all_registers() const;

 private:
    uint16_t __get_hl_address() const;

    std::map<std::string_view, uint8_t> __registers = {
        {mpu::ACCUMULATOR, 0}, {mpu::REG_B, 0}, {mpu::REG_C, 0},
        {mpu::REG_D, 0}, {mpu::REG_E, 0}, {mpu::REG_H, 0}, {mpu::REG_L, 0}
    };

    Memory& __memory;
};

class CpuFlags {
 public:
    CpuFlags();

    void reset_sign();
    void set_sign();
    void reset_zero();
    void set_zero();
    void reset_auxl_carry();
    void set_auxl_carry();
    void reset_parity();
    void set_parity();
    uint8_t get_carry();
    void reset_carry();
    void set_carry();
    std::vector<uint8_t> get_all_flags() const;

 private:
    uint8_t get_flag(const std::string_view& flag_name);
    void reset_flag(const std::string_view& flag_name);
    void set_flag(const std::string_view& flag_name);

    std::map<std::string_view, uint8_t> __flags;
    std::vector<std::string_view> __flags_order = {
        mpu::SIGN_FLAG, mpu::ZERO_FLAG, mpu::AUXIL_CARRY_FLAG, mpu::PARITY_FLAG,
        mpu::CARRY_FLAG };
};

// =============================================================================
//                       Model Class
// =============================================================================
class Model {
 public:
    Model() : memory(), registers(memory), flags() {}

    Memory memory;
    CpuRegisters registers;
    CpuFlags flags;
};

#endif  // __MODEL_HPP__
