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
#include "instructions/mvi.hpp"

// =============================================================================
//                       MVI Impl
// =============================================================================
MVI::MVI(const std::string& instruction) : ICommand(instruction) {
    parse(Parser::DualOperand);
}

bool MVI::execute(Model& model) {
    std::string& destination = _operands.at(0);
    uint8_t source = 0;
    std::string& operand = _operands.at(1);
    NumberType type = ICommand::is_address(operand);
    switch (type) {
        case NumberType::Invalid:
            break;
        case NumberType::Integer:
            source = std::stoi(operand, nullptr, 10);
            break;
        case NumberType::Hex:
            source = std::stoi(operand, nullptr, 16);
            break;
    }
    model.registers.set_register(destination, source);
    // TODO: also do some book-keeping for undo to work
    return true;
}

void MVI::undo(Model& model) {
}

void MVI::setup_opcode_table() {
    uint8_t current_opcode = 0x06;
    for (auto& first_register : _registers) {
        std::string key =
            std::string(first_register) + std::string(mpu::DATA_X);
        _opcode_db[key] = current_opcode;
        current_opcode += 8;
    }
}
