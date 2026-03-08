/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef __COMMAND_HPP__
#define __COMMAND_HPP__

#include <cstdint>
#include <memory>
#include <sstream>
#include <string>

std::string get_first_word(const std::string& str);

class ICommand {
 public:
    static std::shared_ptr<ICommand> get_command(std::string instruction);

    virtual bool execute() = 0;
    virtual void undo() = 0;

    void set_opcode(const uint8_t opcode);
    uint8_t get_opcode();

 private:
    uint8_t __opcode;
};

class ADD : public ICommand {
 public:
    ADD();
    bool execute() final;
    void undo() final;
};

#endif  // __COMMAND_HPP__
