/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef __MODEL_HPP__
#define __MODEL_HPP__

/* standard c++ includes */
#include <cstdint>
#include <map>
#include <string>
#include <vector>

// =============================================================================
//                       CpuRegister Class for 8085 registers
// =============================================================================
class CpuRegisters {
 public:
    void set_register(std::string register_name);

 private:
    std::map<std::string, uint8_t> __registers = {
        {"A", 0}, {"B", 0}, {"C", 0}, {"D", 0}, {"E", 0}, {"H", 0}, {"L", 0}
    };
};

// =============================================================================
//                       Memory Class for 8085 RAM
// =============================================================================
class Memory {
 public:
    void set_memory(uint16_t address, uint8_t value);

 private:
    std::vector<uint8_t> __memory;
};

// =============================================================================
//                       Model Class
// =============================================================================
class Model {
 public:
    CpuRegisters registers;
    const Memory memory;
};

#endif  // __MODEL_HPP__
