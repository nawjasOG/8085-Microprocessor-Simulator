/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

/* standard c++ includes */
#include <memory>
#include <stdexcept>
#include <string>

/* project specific c++ includes */
#include "include/utils.hpp"
#include "include/command.hpp"

// =============================================================================
//                       ICommand Impl
// =============================================================================
std::shared_ptr<ICommand> ICommand::get_command(std::string instruction) {
    std::string command = utils::get_first_word(instruction);
    if (command == "ADD") {
        return std::make_shared<ADD>();
    }
    throw std::invalid_argument("invalid command");
}

void ICommand::set_opcode(const uint8_t opcode) {
    __opcode = opcode;
}

uint8_t ICommand::get_opcode() {
    return __opcode;
}

// =============================================================================
//                       ADD Impl
// =============================================================================
ADD::ADD() {
    set_opcode(80);
}

bool ADD::execute() {
    return true;
}

void ADD::undo() {
}
