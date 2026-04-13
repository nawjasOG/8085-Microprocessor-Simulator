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
#include "instructions/dcr.hpp"

// =============================================================================
//                       DCR Impl
// =============================================================================
DCR::DCR(const std::string& instruction) : ICommand(instruction) {
    parse(Parser::SingleOperand);
}

bool DCR::execute(Model& model) {
    const std::string& register_name = _operands.at(0);
    uint8_t value = model.registers.get_register(register_name);
    uint8_t result = value - 1;

    // setting different flags based on the result
    result == 0 ? model.flags.set_zero() : model.flags.reset_zero();
    __builtin_popcount(result)%2 ?
        model.flags.reset_parity() : model.flags.set_parity();
    (result>>7)&1 ? model.flags.set_sign() : model.flags.reset_sign();
    model.registers.set_register(register_name, result);
    // TODO: also do some book-keeping for undo to work
    return true;
}

void DCR::undo(Model& model) {
}

void DCR::setup_opcode_table() {
    uint8_t current_opcode = 0x05;
    for (auto& first_register : _registers) {
        std::string key = std::string(first_register);
        _opcode_db[key] = current_opcode;
        current_opcode += 8;
    }
}
