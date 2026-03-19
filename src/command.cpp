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
#include <stdexcept>
#include <string>
#include <vector>

/* project specific c++ includes */
#include "include/utils.hpp"
#include "include/model.hpp"
#include "include/command.hpp"

// =============================================================================
//                       ICommand Impl
// =============================================================================
ICommand::ICommand(const std::string& instruction)
    : __instruction(utils::normalize_string(instruction)) {}

std::shared_ptr<ICommand> ICommand::get_command(std::string instruction) {
    std::string command = utils::get_first_word(instruction);
    try {
        if (command == "ADD") {
            return std::make_shared<ADD>(instruction);
        } else if (command == "MOV") {
            return std::make_shared<MOV>(instruction);
        } else if (command == "MVI") {
            return std::make_shared<MVI>(instruction);
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

// =============================================================================
//                       InvalidCommand Impl
// =============================================================================
InvalidCommand::InvalidCommand() : ICommand("InvalidCommand") {
    set_opcode(0x10);
    update_machine_code();
}

bool InvalidCommand::execute(Model& model) {
    dont_call_me(__PRETTY_FUNCTION__);
    return false;
}

void InvalidCommand::undo(Model& model) {
    dont_call_me(__PRETTY_FUNCTION__);
}

void InvalidCommand::setup_opcode_table() {
    dont_call_me(__PRETTY_FUNCTION__);
}

void InvalidCommand::dont_call_me(const std::string& caller) {
    std::string error = caller + " restricted";
    throw std::runtime_error(error);
}

// =============================================================================
//                       ADD Impl
// =============================================================================
ADD::ADD(const std::string& instruction) : ICommand(instruction) {
    parse(Parser::SingleOperand);
}

bool ADD::execute(Model& model) {
    std::string& register_name = _operands.at(0);
    uint8_t accumulator = model.registers.accumulator();
    uint8_t value = model.registers.get_register(register_name);
    uint8_t result = accumulator + value;

    // setting different flags based on the result
    result < accumulator ? model.flags.set_carry() : model.flags.reset_carry();
    result == 0 ? model.flags.set_zero() : model.flags.reset_zero();
    __builtin_popcount(result)%2 ?
        model.flags.reset_parity() : model.flags.set_parity();
    (result>>7)&1 ? model.flags.set_sign() : model.flags.reset_sign();
    model.registers.set_accumulator(result);

    // book-keeping for undo operation
    // TODO: also do some book-keeping for undo to work
    return true;
}

void ADD::undo(Model& model) {
}

void ADD::setup_opcode_table() {
    uint8_t current_opcode = 0x80;
    for (auto& first_register : _registers) {
        std::string key(first_register);
        _opcode_db[key] = current_opcode++;
    }
}

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
