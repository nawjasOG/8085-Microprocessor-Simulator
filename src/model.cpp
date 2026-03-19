/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

/* standard c++ includes */
#include <cassert>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

/* project specific c++ includes */
#include "include/model.hpp"
#include "include/utils.hpp"

// =============================================================================
//                       CpuRegisters Impl
// =============================================================================
uint8_t CpuRegisters::accumulator() const {
    return get_register(std::string(mpu::ACCUMULATOR));
}
void CpuRegisters::set_accumulator(uint8_t value) {
    set_register(std::string(mpu::ACCUMULATOR), value);
}

uint8_t CpuRegisters::get_register(const std::string& register_name) const {
    assert(__registers.count(register_name));
    return __registers.at(register_name);
}

void CpuRegisters::set_register(const std::string& register_name,
                                uint8_t value) {
    assert(__registers.count(register_name));
    __registers[register_name] = value;
}

std::vector<uint8_t> CpuRegisters::get_all_registers() const {
    std::vector<uint8_t> values;
    for (auto& item  : __registers) {
        values.push_back(item.second);
    }
    return values;
}

// =============================================================================
//                       CpuFlags Impl
// =============================================================================
CpuFlags::CpuFlags() {
    for (auto& flag : __flags_order) {
        __flags.insert(std::make_pair(flag, 0));
    }
}

void CpuFlags::reset_sign() {
    reset_flag(mpu::SIGN_FLAG);
}

void CpuFlags::set_sign() {
    set_flag(mpu::SIGN_FLAG);
}

void CpuFlags::reset_zero() {
    reset_flag(mpu::ZERO_FLAG);
}

void CpuFlags::set_zero() {
    set_flag(mpu::ZERO_FLAG);
}

void CpuFlags::reset_auxl_carry() {
    reset_flag(mpu::AUXIL_CARRY_FLAG);
}

void CpuFlags::set_auxl_carry() {
    set_flag(mpu::AUXIL_CARRY_FLAG);
}

void CpuFlags::reset_parity() {
    reset_flag(mpu::PARITY_FLAG);
}

void CpuFlags::set_parity() {
    set_flag(mpu::PARITY_FLAG);
}

void CpuFlags::reset_carry() {
    reset_flag(mpu::CARRY_FLAG);
}

void CpuFlags::set_carry() {
    set_flag(mpu::CARRY_FLAG);
}

void CpuFlags::reset_flag(const std::string_view& flag_name) {
    assert(__flags.count(flag_name));
    __flags[flag_name] = 0;
}

void CpuFlags::set_flag(const std::string_view& flag_name) {
    assert(__flags.count(flag_name));
    __flags[flag_name] = 1;
}

std::vector<uint8_t> CpuFlags::get_all_flags() const {
    std::vector<uint8_t> values;
    for (auto& flag  : __flags_order) {
        values.push_back(__flags.at(flag));
    }
    return values;
}

// =============================================================================
//                       Memory Impl
// =============================================================================
void Memory::set_memory(uint16_t address, uint8_t value) {
    __memory[address] = value;
}
