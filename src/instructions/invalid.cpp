/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

/* standard c++ includes */
#include <cstdlib>
#include <stdexcept>
#include <string>

/* project specific c++ includes */
#include "model.hpp"
#include "instructions/command.hpp"
#include "instructions/invalid.hpp"

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
