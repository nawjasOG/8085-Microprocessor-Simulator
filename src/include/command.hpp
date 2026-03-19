/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef __COMMAND_HPP__
#define __COMMAND_HPP__

/* standard c++ includes */
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

/* project specific c++ includes */
#include "model.hpp"
#include "utils.hpp"

// =============================================================================
//                       Macros for instructions
// =============================================================================
#define INSTR_SEPARATOR 0x20
#define OPERAND_SEPARATOR 0x2c

// =============================================================================
//                       Enum class for type of parsers
// =============================================================================
enum class Parser {
    NoOperand,
    SingleOperand,
    DualOperand,
};

// =============================================================================
//                       Enum class for type of numbers/addresses
// =============================================================================
enum class NumberType {
    Invalid,
    Hex,
    Integer,
};

// =============================================================================
//                       Command Interface Class
// =============================================================================
class ICommand {
 public:
    explicit ICommand(const std::string& instruction);

    static std::shared_ptr<ICommand> get_command(std::string instruction);
    virtual bool execute(Model& model) = 0;
    virtual void undo(Model& model) = 0;
    uint16_t get_address();
    std::vector<uint8_t> get_machine_code();
    uint8_t get_opcode();
    void set_address(uint16_t address);

 protected:
    static NumberType is_address(const std::string& str);
    void parse(Parser parser);
    void set_opcode(const uint8_t opcode);
    void update_machine_code();

    std::vector<std::string> _operands;
    std::unordered_map<std::string, int> _opcode_db;
    const std::vector<std::string_view> _registers = {
        mpu::REG_B, mpu::REG_C, mpu::REG_D, mpu::REG_E, mpu::REG_H, mpu::REG_L,
        mpu::REG_M, mpu::ACCUMULATOR };

 private:
    virtual void setup_opcode_table() = 0;
    virtual uint8_t lookup_opcode();
    virtual std::vector<uint8_t> get_operand_codes();

    std::uint16_t __address;
    std::string __instruction;
    std::vector<uint8_t> __machine_code;
    uint8_t __opcode;
};

// =============================================================================
//                       InvalidCommand Class
// =============================================================================
class InvalidCommand: public ICommand {
 public:
    InvalidCommand();

    bool execute(Model& model) final;
    void undo(Model& model) final;
 private:
    void setup_opcode_table() final;
    void dont_call_me(const std::string& caller);
};

// =============================================================================
//                       ADD Class for ADD instruction
// =============================================================================
class ADD : public ICommand {
 public:
    explicit ADD(const std::string& instruction);

    bool execute(Model& model) final;
    void undo(Model& model) final;

 private:
    void setup_opcode_table() final;
};

// =============================================================================
//                       MOV Class for MOV instruction
// =============================================================================
class MOV : public ICommand {
 public:
    explicit MOV(const std::string& instruction);

    bool execute(Model& model) final;
    void undo(Model& model) final;

 private:
    void setup_opcode_table() final;
};

// =============================================================================
//                       MVI Class for MVI instruction
// =============================================================================
class MVI : public ICommand {
 public:
    explicit MVI(const std::string& instruction);

    bool execute(Model& model) final;
    void undo(Model& model) final;

 private:
    void setup_opcode_table() final;
};

#endif  // __COMMAND_HPP__
