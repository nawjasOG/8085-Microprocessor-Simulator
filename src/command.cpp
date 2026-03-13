/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

/* standard c++ includes */
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <string>

/* project specific c++ includes */
#include "include/utils.hpp"
#include "include/command.hpp"

// =============================================================================
//                       ICommand Impl
// =============================================================================
ICommand::ICommand(const std::string& instruction)
    : __instruction(utils::normalize_string(instruction)) {}

std::shared_ptr<ICommand> ICommand::get_command(std::string instruction) {
    std::string command = utils::get_first_word(instruction);
    if (command == "ADD") {
        return std::make_shared<ADD>(instruction);
    }
    throw std::invalid_argument("invalid command");
}

uint8_t ICommand::get_opcode() {
    return __opcode;
}

void ICommand::parse(Parser parser) {
    switch (parser) {
        case Parser::NoOperand:
            break;
        case Parser::SingleOperand:
            size_t start_pos = __instruction.find(' ') + 1;
            std::string operand = __instruction.substr(start_pos);
            // set_opcode(128 + (uint8_t)(operand[0]-'A'));
            _operands.push_back(operand);
            break;
    }
    setup_opcode_table();
    set_opcode(lookup_opcode());
}

uint8_t ICommand::lookup_opcode() {
    std::string key{};
    for (auto &operand : _operands) {
        // if (is_address(operand)) {
        //     key += "Z";
        //     continue;
        // }
        key += operand;
    }
    return resolve_opcode(key);
}

uint8_t ICommand::resolve_opcode(const std::string& key) {
    return _opcode_db.at(key);
}

void ICommand::set_opcode(const uint8_t opcode) {
    __opcode = opcode;
}

// =============================================================================
//                       ADD Impl
// =============================================================================
ADD::ADD(const std::string& instruction) : ICommand(instruction) {
    parse(Parser::SingleOperand);
}

bool ADD::execute() {
    return true;
}

void ADD::undo() {
}

void ADD::setup_opcode_table() {
    _opcode_db = {
        {"B", 0x80},
        {"C", 0x81},
        {"D", 0x82},
        {"E", 0x83},
        {"H", 0x84},
        {"L", 0x85},
        {"M", 0x86},
        {"A", 0x87},
    };
}
