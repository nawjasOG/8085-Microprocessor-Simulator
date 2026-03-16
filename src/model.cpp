/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

/* standard c++ includes */
#include <cstdint>
#include <string>

/* project specific c++ includes */
#include "include/model.hpp"

// =============================================================================
//                       CpuRegisters Impl
// =============================================================================

uint8_t CpuRegisters::accumulator() {
    return get_register("A");
}
void CpuRegisters::set_accumulator(uint8_t value) {
    set_register("A", value);
}

uint8_t CpuRegisters::get_register(const std::string& register_name) {
    // TODO: put some asserts for validation
    return __registers.at(register_name);
}

void CpuRegisters::set_register(const std::string& register_name,
                                uint8_t value) {
    // TODO: put some asserts for validation
    __registers[register_name] = value;
}

// =============================================================================
//                       Memory Impl
// =============================================================================
void Memory::set_memory(uint16_t address, uint8_t value) {
    __memory[address] = value;
}
