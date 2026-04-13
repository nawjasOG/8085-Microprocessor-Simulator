/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

/* standard c++ includes */
#include <algorithm>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>

/* project specific c++ includes */
#include "model.hpp"
#include "instructions/command.hpp"
#include "instructions/inx.hpp"

// =============================================================================
//                       INX Impl
// =============================================================================
INX::INX(const std::string& instruction) : ICommand(instruction) {
    parse(Parser::SingleOperand);
}

bool INX::execute(Model& model) {
    const std::string& first_register = _operands.at(0);
    size_t index = std::find(_registers.begin(), _registers.end(),
                        first_register) - _registers.begin();
    const std::string& second_register = std::string(_registers[index+1]);
    uint16_t address = model.registers.get_pair_address(first_register,
                            second_register);
    uint8_t value = model.memory.get_memory(address);
    uint8_t result = value + 1;
    model.memory.set_memory(address, result);

    // TODO: also do some book-keeping for undo to work
    return true;
}

void INX::undo(Model& model) {
}

void INX::setup_opcode_table() {
    uint8_t current_opcode = 0x03;
    for (size_t index = 0; index < 6; index += 2) {
        const std::string_view& first_register = _registers[index];
        std::string key = std::string(first_register);
        _opcode_db[key] = current_opcode;
        current_opcode += 16;
    }
}
