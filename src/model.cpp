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
#include <string_view>
#include <utility>
#include <vector>

/* project specific c++ includes */
#include "model.hpp"
#include "utils.hpp"

// =============================================================================
//                       Memory Impl
// =============================================================================
uint8_t Memory::get_memory(uint16_t address) {
    return __memory[address];
}

void Memory::set_memory(uint16_t address, uint8_t value) {
    __memory[address] = value;
}

// =============================================================================
//                       CpuRegisters Impl
// =============================================================================
CpuRegisters::CpuRegisters(Memory& memory) : __memory(memory) {}

uint8_t CpuRegisters::accumulator() const {
    return get_register(std::string(mpu::ACCUMULATOR));
}
void CpuRegisters::set_accumulator(uint8_t value) {
    set_register(std::string(mpu::ACCUMULATOR), value);
}

uint8_t CpuRegisters::get_register(const std::string& register_name) const {
    if (register_name == "M") {
        return __memory.get_memory(__get_hl_address());
    }
    assert(__registers.count(register_name));
    return __registers.at(register_name);
}

void CpuRegisters::set_register(const std::string& register_name,
                                uint8_t value) {
    if (register_name == "M") {
        __memory.set_memory(__get_hl_address(), value);
        return;
    }
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

uint16_t CpuRegisters::__get_hl_address() const {
    uint8_t lower = __registers.at(mpu::REG_L);
    uint8_t higher = __registers.at(mpu::REG_H);
    uint16_t address = (higher << 8) | lower;
    return address;
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

uint8_t CpuFlags::get_carry() {
    return get_flag(mpu::CARRY_FLAG);
}

void CpuFlags::reset_carry() {
    reset_flag(mpu::CARRY_FLAG);
}

void CpuFlags::set_carry() {
    set_flag(mpu::CARRY_FLAG);
}

uint8_t CpuFlags::get_flag(const std::string_view& flag_name) {
    assert(__flags.count(flag_name));
    return __flags[flag_name];
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
