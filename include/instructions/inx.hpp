/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef __INX_HPP__
#define __INX_HPP__

/* standard c++ includes */
#include <string>

/* project specific c++ includes */
#include "command.hpp"

// =============================================================================
//                       INX Class for INX instruction
// =============================================================================
class INX : public ICommand {
 public:
    explicit INX(const std::string& instruction);

    bool execute(Model& model) final;
    void undo(Model& model) final;

 private:
    void setup_opcode_table() final;
};

#endif  // __INX_HPP__
