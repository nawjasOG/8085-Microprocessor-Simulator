/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

/* standard c++ includes */
#include <cstdint>
#include <string>
#include <vector>

/* project specific c++ includes */
#include "include/model.hpp"

// =============================================================================
//                       CpuRegisters Impl
// =============================================================================

uint8_t CpuRegisters::accumulator() const {
    return get_register("A");
}
void CpuRegisters::set_accumulator(uint8_t value) {
    set_register("A", value);
}

uint8_t CpuRegisters::get_register(const std::string& register_name) const {
    // TODO: put some asserts for validation
    return __registers.at(register_name);
}

void CpuRegisters::set_register(const std::string& register_name,
                                uint8_t value) {
    // TODO: put some asserts for validation
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
//                       Memory Impl
// =============================================================================
void Memory::set_memory(uint16_t address, uint8_t value) {
    __memory[address] = value;
}
