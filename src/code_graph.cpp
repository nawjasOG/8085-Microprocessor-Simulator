/*
 * Copyright (c) 2026, Shubham Rana
 * All rights reserved.
 * This source code is licensed under the MIT-style license found in the
 * LICENSE file in the root directory of this source tree.
 */

/* standard c++ includes */
#include <cassert>
#include <memory>
#include <vector>

/* third-party c++ includes */

/* project specific c++ includes */
#include "code_graph.hpp"
#include "instructions/command.hpp"

// =============================================================================
//                       CodeGraph Impl
// =============================================================================
CodeGraph::CodeGraph(const std::vector<CodeLine>& source_code, Model& model)
        : __model(model) {
    InstrNode_h prev = nullptr;
    for (auto& codeline : source_code) {
        if (INVALID_INSTR == codeline.cmd->get_opcode()) {
            __valid_program = false;
            return;
        }
        InstrNode_h instr = std::make_shared<InstrNode>(InstrNode{
            .cmd = codeline.cmd,
            .address = codeline.cmd->get_address(),
            .next = nullptr,
            .next_if_false = nullptr,
        });
        if (!__root) __root = instr;
        if (prev) prev->next = instr;
        prev = instr;
    }
    if (!__root) __valid_program = false;
}

bool CodeGraph::is_valid_program() {
    return __valid_program;
}

InstrNode_h CodeGraph::get_program_counter() {
    return __root;
}

InstrNode_h CodeGraph::run_next_instr(InstrNode_h current) {
    assert(current);
    current->cmd->execute(__model);
    return current->next;
}
