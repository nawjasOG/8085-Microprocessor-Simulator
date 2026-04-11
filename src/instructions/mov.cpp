/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

/* standard c++ includes */
#include <cstdint>
#include <cstdlib>
#include <string>
#include <vector>

/* project specific c++ includes */
#include "model.hpp"
#include "instructions/command.hpp"
#include "instructions/mov.hpp"

// =============================================================================
//                       MOV Impl
// =============================================================================
MOV::MOV(const std::string& instruction) : ICommand(instruction) {
    parse(Parser::DualOperand);
}

bool MOV::execute(Model& model) {
    std::string& destination = _operands.at(0);
    std::string& source = _operands.at(1);
    uint8_t value = model.registers.get_register(source);
    model.registers.set_register(destination, value);
    // TODO: also do some book-keeping for undo to work
    return true;
}

void MOV::undo(Model& model) {
}

void MOV::setup_opcode_table() {
    uint8_t current_opcode = 0x40;
    for (auto& first_register : _registers) {
        for (auto& second_register : _registers) {
            const std::string key =
                std::string(first_register) + std::string(second_register);
            _opcode_db[key] = current_opcode++;
        }
    }
    current_opcode = 0x77;
    _opcode_db["MA"] = current_opcode++;
    for (auto& second_register : _registers) {
        const std::string key =
            std::string(mpu::ACCUMULATOR) + std::string(second_register);
        _opcode_db[key] = current_opcode++;
    }
}
