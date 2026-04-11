/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef __INVALID_HPP__
#define __INVALID_HPP__

/* standard c++ includes */
#include <string>

/* project specific c++ includes */
#include "command.hpp"

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

#endif  // __INVALID_HPP__
