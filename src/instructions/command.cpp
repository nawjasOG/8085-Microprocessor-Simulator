/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

/* standard c++ includes */
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <regex>
#include <string>
#include <vector>

/* project specific c++ includes */
#include "utils.hpp"
#include "instructions/command.hpp"
#include "instructions/invalid.hpp"
#include "instructions/adc.hpp"
#include "instructions/add.hpp"
#include "instructions/dcr.hpp"
#include "instructions/dcx.hpp"
#include "instructions/inr.hpp"
#include "instructions/inx.hpp"
#include "instructions/mov.hpp"
#include "instructions/mvi.hpp"
#include "instructions/sbb.hpp"
#include "instructions/sub.hpp"

// =============================================================================
//                       ICommand Impl
// =============================================================================
ICommand::ICommand(const std::string& instruction)
    : __instruction(utils::normalize_string(instruction)) {}

std::shared_ptr<ICommand> ICommand::get_command(std::string instruction) {
    std::string command = utils::get_first_word(instruction);
    try {
        if (command == "ADC") {
            return std::make_shared<ADC>(instruction);
        } else if (command == "ADD") {
            return std::make_shared<ADD>(instruction);
        } else if (command == "DCR") {
            return std::make_shared<DCR>(instruction);
        } else if (command == "DCX") {
            return std::make_shared<DCX>(instruction);
        } else if (command == "INR") {
            return std::make_shared<INR>(instruction);
        } else if (command == "INX") {
            return std::make_shared<INX>(instruction);
        } else if (command == "MOV") {
            return std::make_shared<MOV>(instruction);
        } else if (command == "MVI") {
            return std::make_shared<MVI>(instruction);
        } else if (command == "SBB") {
            return std::make_shared<SBB>(instruction);
        } else if (command == "SUB") {
            return std::make_shared<SUB>(instruction);
        }
    }
    catch(...) {
        // does nothing, final return happens below
    }
    return std::make_shared<InvalidCommand>();
}

uint16_t ICommand::get_address() {
    return __address;
}

std::vector<uint8_t> ICommand::get_machine_code() {
    return __machine_code;
}

size_t ICommand::machine_code_size() {
    return __machine_code.size();
}

uint8_t ICommand::get_opcode() {
    return __opcode;
}

void ICommand::set_address(uint16_t address) {
    __address = address;
}

void ICommand::parse(Parser parser) {
    size_t start_pos = __instruction.find(INSTR_SEPARATOR) + 1;
    std::string operand_part = __instruction.substr(start_pos);
    size_t length;
    std::string operand;
    switch (parser) {
        case Parser::NoOperand:
            break;
        case Parser::SingleOperand:
            _operands.push_back(operand_part);
            break;
        case Parser::DualOperand:
            length = operand_part.find(OPERAND_SEPARATOR);
            operand = operand_part.substr(0, length);
            _operands.push_back(operand);
            start_pos = length + 1;
            operand = utils::normalize_string(operand_part.substr(start_pos));
            _operands.push_back(operand);
            break;
    }
    setup_opcode_table();
    set_opcode(lookup_opcode());
    update_machine_code();
}

NumberType ICommand::is_address(const std::string& str) {
    static const std::regex hex_regex(R"(^0[xX][0-9a-fA-F]+$)");
    static const std::regex num_regex(R"(^[0-9]+$)");
    if (std::regex_match(str, hex_regex)) return NumberType::Hex;
    if (std::regex_match(str, num_regex)) return NumberType::Integer;
    return NumberType::Invalid;
}

uint8_t ICommand::lookup_opcode() {
    std::string key{};
    for (auto& operand : _operands) {
        if (ICommand::is_address(operand) != NumberType::Invalid) {
            key += std::string(mpu::DATA_X);
            continue;
        }
        key += operand;
    }
    if (key.empty()) key = std::string(mpu::NO_OPERAND);
    return _opcode_db.at(key);
}

std::vector<uint8_t> ICommand::get_operand_codes() {
    std::vector<uint8_t> operand_code;
    for (auto& operand : _operands) {
        NumberType type = ICommand::is_address(operand);
        switch (type) {
            case NumberType::Invalid:
                break;
            case NumberType::Integer:
                operand_code.push_back(std::stoi(operand, nullptr, 10));
                break;
            case NumberType::Hex:
                if (operand.size() == 6) {
                    // WARNING: the code inside the block is not tested
                    const std::string first_half = operand.substr(0, 4);
                    operand_code.push_back(std::stoi(first_half, nullptr, 16));
                    const std::string second_half = "0x" + operand.substr(4);
                    operand_code.push_back(std::stoi(second_half, nullptr, 16));
                } else {
                    operand_code.push_back(std::stoi(operand, nullptr, 16));
                }
                break;
        }
    }

    return operand_code;
}

void ICommand::set_opcode(const uint8_t opcode) {
    __opcode = opcode;
}

void ICommand::update_machine_code() {
    __machine_code.push_back(get_opcode());
    std::vector<uint8_t> operand_code = get_operand_codes();
    __machine_code.insert(__machine_code.end(), operand_code.begin(),
                          operand_code.end());
}
